#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* threadFunc(void* arg)
{
   char* msg = (char*) arg;	
   pthread_t id = pthread_self();
   printf("%s :\n CSE OS LAB, by Thread ID: %lld\n",msg,(long long)id);
   return NULL;
}

int main()
{
	pthread_t thread1,thread2,thread3;
	char* message1 = "Thread1";
	char* message2 = "Thread2";
	char* message3 = "Thread3";
	int iret1 = pthread_create(&thread1, NULL, threadFunc, (void*) message1);
	int iret2 = pthread_create(&thread2, NULL, threadFunc, (void*) message2);
	int iret3 = pthread_create(&thread3, NULL, threadFunc, (void*) message3);
    pthread_exit(NULL); 
}