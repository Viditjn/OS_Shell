#include <stdio.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


char * readCommand()
{
	/*int size = 1000;
	char * buffer = malloc(sizeof(char)*size);
	int position=0;

	if (!buffer)
	{
		fprintf(stderr,"Memory not allocated!!\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		while(1)
		{
			int cur = getchar();
			if(cur == '\n' || cur == EOF)
			{
				buffer[position] = '\0';
				return buffer;
			}
			buffer[position] = cur;
			position++;
			if(position >= size)
			{
				size *= 2;
				buffer = realloc(buffer,size); 
				if (!buffer)
				{
					fprintf(stderr,"Memory not allocated!!\n");
					exit(EXIT_FAILURE);
				}
			}
		}
	}*/
	char *line = NULL;
  	ssize_t bufsize = 0; // have getline allocate a buffer for us
  	getline(&line, &bufsize, stdin);
  	return line;	
}