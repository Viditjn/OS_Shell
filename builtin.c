#include <stdio.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "rootDirectory.h"

char* home;
int count;
char background[1000][1000];
int pids[1000];
int currPid;
char currC[1000];

int echo(char** command)
{
	int i = 1;
	int size = 1000;
	char * buffer = malloc(sizeof(char)*size);;
	for(i=1;command[i]!=NULL;i++)
	{
		 strcat(buffer,command[i]);
		 strcat(buffer," ");
	}
	int len = strlen(buffer);
	for(i=0;i<len-1;i++)
		if(buffer[i]!='"')
			printf("%c",buffer[i]);
	printf("\n");
}

int cd(char** command)
{
	char* tilda = "~";
	//printf("%s\n",home);
	if(command[1]==NULL || strcmp(command[1],tilda)==0)
	{	//fprintf(stderr, "Path not specified\n");
		//printf("hrllo\n");
		chdir(home);
	}
	else
	{
		int t = chdir(command[1]);
		if(t!=0)
			fprintf(stderr, "Error in opening directory\n");
	}
	return 1;
}

int pwd(char** command)
{
	char* buffer = rootDirectory();
	printf("%s\n",buffer);
}

int jobs(char** command)
{
	//Print all the running backgroud processes
	int i,rank=1;
	for(i=0;i<count;i++)
	{
		if(pids[i]!=-1)
		{
			printf("[%d] %s [%d]\n",i,background[i],pids[i]);
			//rank++;
		}
	}
	return 1;
}

int killall(char** command)
{
	// Used to kill all the running background processes
	int i;
	for(i=0;i<count;i++)
	{
		if(pids[i]!=-1)
		{
			kill(pids[i],SIGKILL);
			pids[i]=-1;
		}
	}
	return 1;
}

int kjob(char** command)
{
	// Use command[1] as jobID and command[2] as signalNumber and works accordingly
	int i,temp=0;
	printf("%s %s\n",command[1],command[2]);
	for(i=0;i<count;i++)
	{
			if(i==atoi(command[1]))
			{
				kill(pids[i],atoi(command[2]));
				if(strcmp(command[2],"9")==0)
					pids[i]=-1;
				temp=1;
				break;
			}
	}
	if(temp==0)
		printf("Invalid Job id\n");
	/*if(i<count)
	{
		printf("%d %s\n",pids[i],background[i]);
		if(strcmp(command[2],"9")==0)
			pids[i]=-1;
		//kill(pids[i],atoi(command[2]));
	}*/
	return 1;
}

int fg(char** command)
{
	// Take command[1] as jobID and run it in foreground
	pid_t ppid;
	int i,temp=0,result;
	for(i=0;i<count;i++)
	{
		if(i==atoi(command[1]))
		{
			//kill(pids[i],SIGCONT);
			currPid = pids[i];
			strcpy(currC,background[i]);
			//printf("%s %d\n",background[i],pids[i]);
			ppid = waitpid(pids[i],&result,WCONTINUED);
			//while( !WIFEXITED(result) && !WIFSIGNALED(result))
			//	ppid = waitpid(pids[i],&result,0);
			pids[i]=-1;
			temp=1;
			break;
		}
	}
	if(temp==0)
		printf("Invalid Job ID\n");
	return 1;	
}

