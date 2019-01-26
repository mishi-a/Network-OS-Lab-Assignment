#include<stdio.h>
#include <sys/types.h>
#include<unistd.h>

int main()
{
	//Creating new process and pid holds the child process id in parent process and
	//0 in child process
	pid_t pid = fork();
    
    //If process is not created
    if(pid<0)
    {
    	printf("Child Process can't be created\n");
    	return 0;
    }
    //If pid == 0 child process
	if(pid==0)
	{
		//getppid() gets process id of parent
		pid_t parentid= getppid();
		printf("Parent Process ID from child process is %d\n",parentid);
	}
	else         //Block for parent process
	{
        // pid holds child process
		printf("Child Process id from parent process is %d\n",pid);
	}
}