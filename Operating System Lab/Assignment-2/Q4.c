#include<stdio.h>
#include <sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/wait.h>

int main(int argc ,char** argv)
{
  
   //Checking for valid input
    if(argc != 2)
    {
    	printf("Invalid Input\n");
    }
    char* c = argv[1];
    int t = atoi(c);
     
    //Creating new process and pid holds the child process id in parent process and
	//0 in child process
	pid_t pid = fork();
    
    if(pid<0)
    {
    	printf("Child Process can't be created\n");
    	return 0;
    }
    //If pid == 0 child process
	if(pid==0)
	{
		//make child process for t second
		sleep(t);
		//getppid() gets process id of parent
		pid_t curid= getpid();
		printf("child Process ID  is %d\n",curid);
		//Listing process inside child process
		system("ps");
		
	}
	else         //Block for parent process
	{
		printf("Parent Process id is %d\n",getpid());
	    //Listing process inside child process
        system("ps");
		wait(NULL);
        // pid holds child process
        printf("Parent Process id is %d\n",getpid());
        //Listing process inside child process
        system("ps");
    }
}