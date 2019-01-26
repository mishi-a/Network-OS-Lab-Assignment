#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define sz 2


// variuous func
pthread_mutex_t funMutex;
pthread_t threadId[sz];
int lock=0;
int counter =-1;

void* myFunc(void* arg)
{
   int check;

   //converting attribute to type
   int val = (int)arg;	

   //check whether locked or not
   if(lock)	
       printf("pthread%d cannot access function f() lock acquired by pthread%d\n",val+1,counter);
    
   //if lock failed
   if(check = pthread_mutex_lock(&funMutex))
   {
   	 printf("Lock on this critical function Failed\n");
   	 return NULL;
   }
   printf("pthread%d is accessing function f()\n",val+1);

   //lock variable to decide whether locked or not
   lock = 1;
   counter = val+1;
   printf("Lock acquired by pthread%d\n",val+1);
   sleep(5);

   //unlocking this section
   pthread_mutex_unlock(&funMutex);
   printf("Lock released by pthread%d\n",val+1 );

   //making lock variable 0
   lock = 0;
}

int main()
{
  counter = 0;
  int ret;
  //initializing mutex
  if(ret = pthread_mutex_init(&funMutex,NULL))
  {
  	printf("Mutex initialization Failed\n");
  	return 1;
  }
  for(int i=0;i<sz;i++)
  {
  	//creating thread and calling myFunc as the initializer
    int iret = pthread_create(&threadId[i],NULL,myFunc,(void*)i);
    sleep(1);
    
    //If creation failed
    if(iret)
    {
    	printf("Error Thread can't be created");
    }
  }

  //using join to wait for children thread
  pthread_join(threadId[0],NULL);
  pthread_join(threadId[1],NULL);

  //destroying mutex
  pthread_mutex_destroy(&funMutex);
  return 0;
}