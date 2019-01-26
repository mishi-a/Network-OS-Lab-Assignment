#include<stdio.h>
#include <sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/wait.h>

int main()
{
	// creating new process
	pid_t pid = fork();
	if(pid < 0)
	{
		printf("Child Process can't be created\n");
		return 0;
	}
	//child process
	if(pid == 0)
	{
        sleep(5);
        printf("Parent Process is killed so child process is Orphan process");
        //printing current process
        system("ps -l");
	}
	else  
	{
		// printing child process and parent process id 
		printf("Child Process id is %d\n and Parent process id is %d\n", pid,getpid());
		printf("This is Parent process\n");
		// exiting parent process to make child orphan
		exit(0);
	}
}