#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "parse.h"


int pipe_execute(char** command,int in,int out)
{
	// TO EXECUTE COMMAND IN PIPE BEFORE LAST COMMAND
	int fd1,i=0,temp=0,j=0;
	pid_t pid;
	while(command[j]!=NULL)
	{
		if(strcmp(command[j],"<")==0)
		{
			command[j]=NULL;
			temp=1;
			break;
		}
		j++;
	}
	pid = fork();
	if(pid==0)
	{	
		if(temp!=0)
		{
			// Handle I\O redirection
			fd1 = open(command[j+1], O_RDONLY);
    		if (fd1 < 0)
    		{
     			perror("Error in opening file1 :");
     			exit(1);
    		}
      		if (dup2(fd1, 0) != 0) 
    		{
     			perror("Error in dup2(file1, 0)");
     			exit(1);
    		}
    		close(fd1);
    		if(out!=1)
			{
				dup2(out,1);
				close(out);
			}
		}
		else
		{
			if(in!=0)
			{
				dup2(in,0);
				close(in);
			}
			if(out!=1)
			{
				dup2(out,1);
				close(out);
			}
		}
		if(execvp(command[0],command) == -1)
		{
			fprintf(stderr, "Execution failed");
			exit(EXIT_FAILURE);
		}
		//printf("1");
		return 1;
	}
	return pid;
}

int pipe_command(char** string,int n)
{
	// To create pipe for commands and execute last command
	int fields[2],i=0,in=0,result,fd2,j=0,temp=0;
	char** command;
	char* delim2 = " ";
	pid_t pid,ppid;
	for(i=0;i<n-1;i++)
	{
		pipe(fields);
		command = parse(string[i],delim2);
		pipe_execute(command,in,fields[1]);
		close(fields[1]);
		in = fields[0];
	}
	if(in!=0)
		dup2(in,0);
	//  Code for last Command
	command = parse(string[i],delim2);
	pid =fork();
	if(pid < 0)
	{
		fprintf(stderr,"Failed forking Child Process\n");
		exit(EXIT_FAILURE);
	} 
	else if(pid == 0)
	{
		// TO HANDLE I\O 
		while(command[j]!=NULL)
		{
			if(strcmp(command[j],">")==0)
			{
				temp=1;
				command[j]=NULL;
				break;
			}
			if(strcmp(command[j],">>")==0)
			{
				temp=2;
				command[j]=NULL;
				break;
			}
			j++;
		}
		if(temp!=0)
    	{
    		//printf("hello\n");
    		if(temp==1)
    			fd2 = open(command[j+1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    		if(temp==2)
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

	close(in);
	return 1;
}

