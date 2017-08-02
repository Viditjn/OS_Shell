#include <stdio.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "builtin.h"
#include "signal.h"

//int totalCommands = sizeof(commandIncluded)/sizeof(char *);

//">>" Remaining.

char* u;
int totalCommands = 7;
int count;
char background[1000][1000];
int pids[1000];
int currPid;
char currC[1000];

char* commandIncluded[] = {
	"cd",
	"pwd",
	"echo",
	"jobs",
	"killall",
	"kjob",
	"fg"
};
	/*"kjob",
	"fg",
	"killall"
};*/

typedef int (*f)(char**);
f func[7] = {&cd, &pwd, &echo,&jobs,&killall,&kjob,&fg};
//,&kjob,&fg,&killall};

int inputred(char** command,int i)
{
	int fd1,fd2,result,temp=0,t1=0,j=i+1;
	pid_t pid,ppid;
	char* out = ">";
	char* append = ">>";
	while(command[j]!=NULL)  // For checking of further redirction command in given command
	{
		if(strcmp(command[j],append)==0)
		{
			command[j]=NULL;
			temp=1;
			t1=2;
			break;
		}	
		else if(strcmp(command[j],out)==0)
		{
			command[j]=NULL;
			temp=1;
			t1=1;
			break;
		}
		j++;
	}
	if(command[0]==NULL)
		return 1;
	pid = fork();
	if(pid < 0)
	{
		fprintf(stderr,"Failed forking Child Process\n");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0) // Code to execute input Redirection
	{
    	fd1 = open(command[i+1], O_RDONLY);
    	if (fd1 < 0)
    	{
     		perror("Error in opening file1 :");
     		exit(1);
    	}
      	if (dup2(fd1, 0) != 0) 
    	{
     		perror("Error in dup2(file1,0) :");
     		exit(1);
    	}
    	close(fd1);
    	if(temp==1) // For output Redirction along with input redirection 
    	{
    		if(t1==1)
    			fd2 = open(command[j+1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    		if(t1==2)
    			fd2 = open(command[j+1], O_RDWR | O_APPEND | O_CREAT, 0644);
    		//printf("%s\n",command[j+1]);
    		if (fd2 < 0) 
    		{
      			perror("Error in opening file2 :");
      			exit(2);
    		}
  
    		if (dup2(fd2, 1) != 1) 
    		{
      			perror("Error in dup2(file2, 1)");
      			exit(1);
    		}
    		close(fd2);
    	}

    	if(execvp(command[0],command) == -1)
		{
			fprintf(stderr, "Execution failed");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		ppid = waitpid(pid,&result,0);
		while( !WIFEXITED(result) && !WIFSIGNALED(result))
			ppid = waitpid(pid,&result,0);
	}	
	return 1;		
}

int outred(char** command,int i,int mode)
{
	// To handle output Redirection
	int fd2,result;
	pid_t pid,ppid;
	pid = fork();
	if(command[0]==NULL)
		return 1;
	if(pid < 0)
	{
		fprintf(stderr,"Failed forking Child Process\n");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0) 
	{
		// Open file to store output
		if(mode==1)
			fd2 = open(command[i+1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if(mode==2)
			fd2 = open(command[i+1], O_RDWR | O_APPEND | O_CREAT, 0644);
    	if (fd2 < 0)
    	{
      		perror("Error in opening file2 :");
      		exit(2);
    	}
    	if (dup2(fd2, 1) != 1)
   		{
      		perror("Error in dup2(file2, 1)");
      		exit(1);
    	}
    	close(fd2);
    	if(execvp(command[0],command) == -1)
		{
			fprintf(stderr, "Execution failed");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		ppid = waitpid(pid,&result,0);
		while( !WIFEXITED(result) && !WIFSIGNALED(result))
			ppid = waitpid(pid,&result,0);
	}	
	return 1;
}

int execute(char** command)
{
	// Code to execute commands
	pid_t pid,ppid;
	int result,i,mode=0;
	int var = 0;
	char* bg = "&";
	char* in = "<";
	char* out = ">";
	char* append = ">>";
	if(command[1])
	{
		if(strcmp(command[1],bg)==0)
		{
			// Check for &
			command[1]='\0';
			u = command[0];
			var = 1;
		}
	}
	i=0;
	while(command[i]!='\0' || command[i]!=NULL)
	{
		// To check I\O redirection
		if(strcmp(command[i],in)==0)
		{
			command[i]=NULL;
			return inputred(command,i);
		}
		if(strcmp(command[i],out)==0)
		{
			command[i]=NULL;
			mode = 1;
			return outred(command,i,mode);
		}
		if(strcmp(command[i],append)==0)
		{
			command[i]=NULL;
			mode = 2;
			return outred(command,i,mode);
		}
		i++;
	}
	pid =fork();
	currPid = pid;
	strcpy(currC,command[0]);
	if(pid < 0)
	{
		fprintf(stderr,"Failed forking Child Process\n");
		exit(EXIT_FAILURE);
	} 
	else if(pid == 0)
	{
		//printf("%s\n",command[0]);
		if(execvp(command[0],command) == -1)
		{
			fprintf(stderr, "Execution failed");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		if(var==0)
		{
			//IF NOT IN BACKGROUND
			//ppid = waitpid(pid,&result,0);
			ppid = waitpid(pid,&result,WCONTINUED);
			//while( !WIFEXITED(result) && !WIFSIGNALED(result))
			//	ppid = waitpid(pid,&result,WCONTINUED);
			//signal(SIGTSTP,&sigstop);
		}
		/*if(var==0)
			wait(&result);*/
		else
		{
			// IF IN BG
			printf("%d\t:\t%s\n",pid,command[0]);
			strcpy(background[count],command[0]);
			pids[count] = pid;
			count++;
			//signal(SIGTSTP,&sigstop);
			signal(SIGCHLD,sigh);
			var=0;
		}
	}
	return 1;
}

int builtIn(char** command)
{
	// TO HANDLE BACKGROUND COMMANDS
	if(command[0]==NULL)
		return 1; //No command entered

	else
	{
		for(int i =0 ; i<totalCommands ;i++)
		{
			if(strcmp(command[0],commandIncluded[i])==0)
				return func[i](command);
		}
	}
	return execute(command);
}

