#include <stdio.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "parse.h"

char* home = "/home/vidit/sem3/OS/assg2";


char* get_proc_info(char* pid)
{

  int size = 1000,size_l=100000,i,a=1,d;
  char * buffer = malloc(sizeof(char)*size);
  char * data = malloc(sizeof(char)*size_l);
  char temp;
  sprintf (buffer, "/proc/%s/stat", pid);  
  d = open(buffer,O_RDONLY);
  if(d<0)
  {
    printf("File cannot be open\n");
    return NULL;
  }
  for(i=0;a>=0;i++)
  {
    a = lseek(d,i,SEEK_SET);
    read(d,&temp,1);
    data[i] = temp;   
    if(temp=='\n' || temp == '\0')
      break;
    //printf("%c %d\n",data[i],i );
  }
  close(d);
  return data;
}



void print(char* pid)
{
  char * a = get_proc_info(pid);
  char* delim = " ";
  char ** store = parse(a,delim);
  printf("pid :- %s\n",store[0]);
  printf("status :- %s\n",store[2]);
  printf("Memory :- %s\n",store[22]);

  //char* buf = malloc(sizeof(char)*10000);
  char buf[10000]; 
  char* path = malloc(sizeof(char)*10000);
  int errno =0;
  sprintf(path,"/proc/%s/exe", pid);
 /* if(errno)
    {
        fprintf(stderr, "Error reading symbolic link %s: %s\n", path, strerror(errno));
        return;
    }*/

  readlink(path, buf, 10000);

  int sameChars = 0, baseL = strlen(home);
    for(sameChars = 0; sameChars < baseL; sameChars++)
        if(home[sameChars] != buf[sameChars]) break;
    
    char relPath[10000];
    if(sameChars == baseL) // First part of path is common
    {
        relPath[0] = '~'; relPath[1] = '\0';
        strcat(relPath, (const char *)&buf[baseL]);
    }
    else
    {
        strcpy(relPath, buf);
        relPath[strlen(buf)] = '\0';
    }
  printf("Executable path: %s\n",relPath);
  return;
}



