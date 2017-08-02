#include <stdio.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "builtin.h"
#include "rootDirectory.h"
#include "readCommand.h"
#include "parse.h"
#include "exceute.h"
#include "pinfo.h"
#include "pipe.h"
#include "signal.h"

char* home;
char* u;
char background[1000][1000];
int pids[1000];
int count=0;
char* currPosition; 
char hostname[100];
const char* uName;
int currPid;
char currC[1000];

void sigintHandler(int sig_num) //Ctrl+C
{
	signal(SIGINT, sigintHandler);
	printf("\n");
	//printprompt(hostname, checker, 1);
	printf("%s@%s:%s>",uName,hostname,currPosition);
	fflush(stdout);
}

int main()
{	
	home = rootDirectory();
	char* root = rootDirectory();
	char** commands;
	//char hostname[100];
	gethostname(hostname,sizeof(hostname));
	//char** currCommand;
	struct utsname systemName;
	//uname(&systemName);
	//const char* uName = getenv("USER");
	uName = getenv("USER");
	char* delim1 = ";\n";
	char* delim2 = " ";
	char* delim3 = "|";
	currPosition = getDirectory(root);
	while(1)
	{
		signal(SIGTSTP,&sigstop);
		//signal(SIGCHLD,sigh);
		signal(SIGINT, sigintHandler);
		int stdin_copy = dup(0);
		int stdout_copy = dup(1);
		//char* currPosition = getDirectory(root);
		currPosition = getDirectory(root);
		//printf("%s@%s:%s>",uName,systemName.sysname,currPosition);
		printf("%s@%s:%s>",uName,hostname,currPosition);
		//printf("%d\n",currPid);
		char* command = readCommand();
		//printf("%s\n",command);
		if (strcmp(command,"\0")==0	) // Ctrl+D
		{
		    printf("Closing shell\n");
		    exit(0);
		}
		commands = parse(command,delim1);
		/*int i=0;
		while(commands[i]!=NULL)
		{
			printf("%s ",commands[i]);
			i++;
		}
		//printf("\n%d %s",i,commands[i-1]);
*/		for(int i=0;commands[i]!=NULL;i++)
		{
			int n=0;
			char** pipeCommand;
			pipeCommand = parse(commands[i],delim3);
			while(pipeCommand[n]!=NULL)
			{
				n++;
			}
			if(n>1)
			{
				//printf("%d\n",n);
				pipe_command(pipeCommand,n);
				//printf("level1\n");
			}
			//printf("level2\n");
			else
			{
				char** currCommand;
				currCommand = parse(commands[i],delim2);
				if(strcmp(currCommand[0],"pinfo")==0)
				{
					print(currCommand[1]);
					continue;
				}
				if(strcmp(currCommand[0],"quit")==0)
				{
					printf("BYE\n");
					return 0;
				}
				builtIn(currCommand);
				free(currCommand);
			}
			free(pipeCommand);
		}
		free(commands);
		dup2(stdin_copy, 0);
		dup2(stdout_copy, 1);
		close(stdin_copy);
		close(stdout_copy);
	}
	return 0;
}