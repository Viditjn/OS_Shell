#include <stdio.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

char * rootDirectory()
{
	size_t size = 100;
	while(1)
	{
		char * buffer = (char*) malloc(size);
		if (getcwd(buffer,size) == buffer)
			return buffer;
		free(buffer);
		//else if(errno != ERANGE)
		//	return 0; 
		size *= 2;
	}
}

char * getDirectory(char * home)
{
	int size = 1000; 
	char* buffer=rootDirectory();
	char* buf = malloc(sizeof(char)*size);
	int i=0,j=1;
	while(buffer[i]==home[i] && i<=strlen(home) && i<=strlen(buffer) && home[i]!='\0' && buffer[i]!='\0')
		i++;
	//printf("%s\n%s\n",home,buffer);
	//printf("%d %c %c %d\n",i,buffer[i-1],home[i-1],strlen(home));
	if(i == strlen(home))
	{
		buf[0]='~';
		while(1)
		{
			if(i==strlen(buffer)+1)
				return buf;
			buf[j] = buffer[i];
			j++;
			i++;
			//printf("%d ",j);
		}
		//printf("%s\n",buf);
		return buf;	
	}
	else
		return buffer;

}