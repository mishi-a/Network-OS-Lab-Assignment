#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#define num_philosopher 5
#define num_Fork 5

int allocationMatrix[num_philosopher][num_philosopher];
int requestMatrix[num_philosopher][num_philosopher];
sem_t checkForkAvail[num_Fork];
pthread_mutex_t lockPrint;
int preemptedId=-1;

//Method to Print allocation matrix and request matrix
void printMatrix()
{
	pthread_mutex_lock(&lockPrint);
	printf("Resource allocationMatrix :\n");
	for(int i=0;i<num_philosopher;i++)
	{
		for(int j=0;j<num_Fork;j++)
			printf("%d ",allocationMatrix[i][j]);
		printf("\n");
	}

	printf("Resource requestMatrix :\n");
	for(int i=0;i<num_philosopher;i++)
	{
		for(int j=0;j<num_Fork;j++)
			printf("%d ",requestMatrix[i][j]);
		printf("\n");
	}
	pthread_mutex_unlock(&lockPrint);
}

//Method to pick requested fork
void pick(int philosopherId,int forkId)
{
  requestMatrix[philosopherId][forkId]=1;
  //Waiting for requested fork
  sem_wait(&checkForkAvail[forkId]);
  requestMatrix[philosopherId][forkId]=0;
  allocationMatrix[philosopherId][forkId]=1;
}

//Method to drop the requested fork
void drop(int philosopherId,int forkId)
{
  allocationMatrix[philosopherId][forkId]=0;
  //releasing the requested fork
  sem_post(&checkForkAvail[forkId]);
}

//Method to simulate all process for particular philosopher
void* dinePhilosopher(void* arg)
{
	int philosopherId = *((int*)arg);
	while(1)
	{
		int leftFork = philosopherId;
		int rightFork = (leftFork+1)%num_philosopher;
		sleep(1);
		pick(philosopherId,leftFork);
		printf("Philosopher %d grab fork %d (left)\n",philosopherId,leftFork);
		sleep(1);
		pick(philosopherId,rightFork);

		//Condition when a philosopher preempts to drop his left fork
		if(!allocationMatrix[philosopherId][leftFork])
		{
          	pick(philosopherId,leftFork);
			printf("Philosopher %d grab fork %d (left)\n",philosopherId,leftFork);
		}

        printf("Philosopher %d grab fork %d (right)\n",philosopherId,rightFork);
		printf("Philosopher %d starts eating.\n",philosopherId);
		sleep(0.5);
 	    printMatrix();
 	    sleep(2);
 		printf("Philosopher %d ends eating and releases forks %d (left) and %d (right)\n",philosopherId,leftFork,rightFork);
		drop(philosopherId,leftFork);
		printf("Philosopher %d starts thinking\n",philosopherId); 
        drop(philosopherId,rightFork);
	}
}

int main()
{
   pthread_t threadId[num_philosopher];
   pthread_mutex_init(&lockPrint,NULL);
   srand(time(0));

   //initializing semaphore for each fork
   for(int i=0;i<num_Fork;i++)
   	   sem_init(&checkForkAvail[i],0,1);

   int philosopher =0;
   for(int philosopher=0;philosopher<num_philosopher;philosopher++)
   {
   	   int iret = pthread_create(&threadId[philosopher],NULL,dinePhilosopher,&philosopher);
   	   if(iret < 0)
   	   {
   	   	 printf("Thread creation failed for philosopher %d\n",philosopher);
   	   }
   	   printf("Philosopher %d starts thinking\n",philosopher); 
   	   sleep(0.001);
   }

   //Loop that check whether there is deadlock or not
   while(1)
   {
   	   bool flag = false;
 	   for(int i=0;i<num_Fork;i++)
	   {
	    	if(!allocationMatrix[i][i])
			   flag = true;
	   }
	   if(flag)
	   	  continue;
	   //Deadlock when all philosopher holds left fork
	   printf("Parent detects deadlock, going to initiate recovery\n");
	   int forkTODrop = rand()%num_Fork;
       printf("Parent preempts Philosopher %d\n",forkTODrop);
       drop(forkTODrop,forkTODrop);
       printMatrix();
       sleep(1);
   }
}