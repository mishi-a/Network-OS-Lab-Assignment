#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#define sz 5

int globVariable = 5;

void* threadFunc(void* arg)
{
	static int staticVariable = 0;
	char* msg = (char*) arg;
    int localVariable = 0;
    printf("%s: localVariable = %d,staticVariable=%d,globVariable=%d\n",msg,localVariable,staticVariable,globVariable);
    localVariable += 1;
    staticVariable += 1;
    globVariable += 1;
    printf("%s: localVariable = %d,staticVariable=%d,globVariable=%d\n",msg,localVariable,staticVariable,globVariable);
	return NULL;
}

int main()
{
  pthread_t threadID[sz];
  char base[12] = "threadID[";
  char num[1];
  char brace[] = "]";
  for(int i=0;i<sz;i++)
  {
    sprintf(num,"%d",i);
    base[9] = num[0];
    base[10]=']';
    base[11]='\0';
  	int iret = pthread_create(&threadID[i], NULL, threadFunc, base);
  	pthread_join(threadID[i],NULL);
  }
  pthread_exit(NULL);
}