#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#define RAND 10
pthread_mutex_t lockStudent;
sem_t checkQuesEnd;
sem_t checkAnsEnd;
int id;


void QuesStart(int studentId)
{
	printf("Student %d started asking question\n", studentId);
	id = studentId;
	return;
}

void QuesEnd(int studentId)
{
	//question ends
	sem_post(&checkQuesEnd);

	//waiting for professor to finish answering
 	sem_wait(&checkAnsEnd);
 	printf("Student %d question ends\n", studentId);
	return;
}

void* students(void* arg)
{
	int stud = *((int*)arg);
	printf("Student number %d enter the class\n",stud);
	while(1)
	{
		//acquiring mutex lock so that one student can ask question only
		pthread_mutex_lock(&lockStudent);
 		
 		//student start asking question
 		QuesStart(stud);

 		//asking question
 		sleep(1);
 		
 		//student end asking question
 		QuesEnd(stud);

 		//releasing mutex lock for other student
 		pthread_mutex_unlock(&lockStudent);
 		//random sleep so that each student can ask question randomly
 		sleep((rand())%RAND);
	}
}

void AnsStart()
{
	//waiting for student to finish asking question
	sem_wait(&checkQuesEnd);
	printf("Professor started answering question of student %d\n",id );
	return;
}

void AnsEnd()
{
	printf("Professor ended answering question of student %d\n",id );

	//professor finished answering question
	sem_post(&checkAnsEnd);
	return;	
}
int main()
{
	srand(time(NULL));

    //mutex for only single student to ask question
    pthread_mutex_init(&lockStudent,NULL);
    //Semaphore to check whether question ends or ans ends
    sem_init(&checkQuesEnd,0,0);
    sem_init(&checkAnsEnd,0,0);

    //Taking number of student from user
	int numStudent;
	printf("Enter the number of students present in professor class\n");
	scanf("%d",&numStudent);

	//creating as many number of thread as student
	pthread_t threadId[numStudent+1];

	//professor thread is main thread
    printf("Professor enter the class\n");
	for(int student=1;student<=numStudent;student++)
	{

		//creating thread
		int iret = pthread_create(&threadId[student],NULL,students,&student);
		sleep(0.001);
	    if(iret == -1)
	    {
   	   		 printf("Thread creation failed for student %d\n",student);
   	   	}	
	}

	while(1)
	{
		
		//professor start answering question
		AnsStart();
		
		//answering question
		sleep(1);
  		
  		//professor ends answering question
		AnsEnd();
		
	}
}