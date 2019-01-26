#include<stdio.h>
#include <sys/types.h>
#include<unistd.h>
#include <sys/wait.h>

int glob = 50;

int main()
{
    //Creating new process and pid holds the child process id in parent process and
	//0 in child process
	pid_t pid = fork();
    
    //if child process can't be created
    if(pid<0)
    {
    	printf("Child Process can't be created\n");
    	return 0;
    }

    // As both process uses separate address space that's why the change in
    // global variabe glob is different.
    //If pid == 0 child process
	if(pid==0)
	{
		//getppid() gets process id of parent
		pid_t curid= getpid();
		printf("child Process ID  is %d\n",curid);
		printf("value of glob currently (c1) is %d \n",glob);
		//updating global variable
		glob = 10;
		printf("value of glob currently after updating (c2) is %d \n",glob);

	}
	else         //Block for parent process
	{
		//wait blocks the current execution of parent process and wait 
		//for child to finish
		wait(NULL);
        // pid holds child process
		printf("Parent Process id is %d\n",getpid());
		printf("value of glob currently (p1) is %d \n",glob);
		//updating global variable
		glob = 13;
		printf("value of glob currently (p2) after updating is %d \n",glob);
	}
	//printf("%d\n",glob);
}