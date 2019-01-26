#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define sz 5

void* Busy(void* arg)
{
  char* msg = (char*) arg;	
  printf("%s : creating thread = %lld\n",msg,(long long)pthread_self());
  sleep(2);
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
   	int iret = pthread_create(&threadID[i], NULL, Busy, base);
   	if(iret)
   	{
   		printf("ERROR; return code from pthread_create() is %d\n",iret);
   	}
   	int iret1 = pthread_join(threadID[i],NULL);
   	if(iret1)
   	{
        printf("ERROR; return code from pthread_join() is %d\n",iret1 );
   	}
   	else
   	{
   		printf("Completed join with thread %lld having a status of %d\n",(long long)threadID[i],iret1);
   	}
   }
   printf("\n\nExiting from program.\n");
   exit(0);
}