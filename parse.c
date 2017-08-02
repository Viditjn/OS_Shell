#include <stdio.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

char** parse(char* buffer,char* delimiter)
{
	// Code for parsing a\c to the passed delimiter
	int noC = 100;
	char **store = malloc(sizeof(char*)*noC);
	if (!store)
	{
		fprintf(stderr,"Memory not allocated!!\n");
		exit(EXIT_FAILURE);
	}
	int count=1;
	//const char delimiter[2] = ";";
	char* token;
	token = strtok(buffer,delimiter);
	store[0] = token;
	while( token!= NULL)
	{
		//printf("%s\n",token);
		token = strtok(NULL,delimiter);
		store[count]=token;
		count++;
		if(count>noC)
		{
			noC *= 2;
			store = realloc(store,noC);
			if (!store)
			{
				fprintf(stderr,"Memory not allocated!!\n");
				exit(EXIT_FAILURE);
			}
		}

	}
	store[count]=NULL;
	return store; 
}
