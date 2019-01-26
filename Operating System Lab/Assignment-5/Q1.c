#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define sz 2 
sem_t semaphores;

//common variable for reading and writing
int sharedVariable = 0;
 
void* myFunc(void* arg)
{

    // printing current threadID
	printf("Current threadId = %lld\n",(long long)pthread_self());

    /*
      sem_wait() decrements (locks) the semaphore pointed to by sem.
      If the semaphore's value is greater than zero, then the decrement proceeds,
      and the function returns, immediately. If the semaphore currently has the 
      value zero, then the call blocks until either it becomes possible to perform 
      the decrement (i.e., the semaphore value rises above zero), or a signal handler 
      interrupts the call.
    */
    sem_wait(&semaphores);
    sleep(4);
    //Reading shared variable
    printf("ThreadId = %lld,Reading sharedVariable = %d\n",(long long)pthread_self(),sharedVariable);

    printf("Updating sharedVariable by threadId = %lld\n",(long long)pthread_self());
    
    //updating shared variable
    sharedVariable+=1;
    
    //Printing updated shared variable
    printf("ThreadId = %lld,updated sharedVariable = %d\n",(long long)pthread_self(),sharedVariable); 
    
    /*
    The sem_post() function unlocks the specified semaphore by performing a semaphore unlock operation on that
    semaphore.When this operation results in a positive semaphore value, no threads were blocked waiting for the 
    semaphore to be unlocked; the semaphore value is simply incremented.When this operation results in a 
    semaphore value of zero, one of the threads waiting for the semaphore is allowed to return successfully
    from its invocation of the sem_wait() function.
    */
    sem_post(&semaphores);
}
 
 
int main()
{
	//initializing semaphore
    sem_init(&semaphores, 0, 1);

    //creating array for thread
    pthread_t threadId[sz];

    for(int i=0;i<sz;i++)
    {
    	//creating thread 
    	int iret = pthread_create(&threadId[i],NULL,myFunc,NULL);
    	// if creation fails
    	if(iret)
    	{
    		printf("Error Thread can't be created");
   	    }
  	}
  	//using join to wait for child thread
    pthread_join(threadId[0],NULL);
    pthread_join(threadId[1],NULL);

    //destroying semaphore
    sem_destroy(&semaphores);
    return 0;
}