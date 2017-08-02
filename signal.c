#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/utsname.h>
//#include "function.h"

char* u;
int pids[1000];
char background[1000][1000];
int count;
int currPid;
char currC[1000];

void sigstop(int p) // Ctrl + Z
{
	kill (currPid,20); // To stop current process
	kill (currPid,18); // To continue it in background
	pids[count] = currPid;
	strcpy(background[count],currC);
	count++; 
	return;
}

void sigh(int signum) // To print message when a background proceess closes
{
	pid_t wpid;
	int status,i;
	wpid=waitpid(-1,&status,WNOHANG);
	if(wpid>0)
	{
		if(WIFSIGNALED(status)==0)
		{
			for(i=0;i<count;i++)
			{
				if(wpid==pids[i])
				{
					pids[i]=-1;
					break;
				}
			}
			fprintf(stderr,"\n%s with pid %d exited normally\n",background[i],wpid);
		}
	}
}

