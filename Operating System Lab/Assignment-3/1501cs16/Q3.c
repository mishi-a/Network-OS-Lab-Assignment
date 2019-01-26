#include<stdio.h>
#include <sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/wait.h>

int main()
{
	int pip[2];
	//Creating pipe
    pipe(pip);
    //Creating child process
    pid_t pid = fork();
	if(pid < 0)
	{
		printf("Child process can't be created\n");
		return 0;
	}
	if(pid > 0)
	{
		//closing reading end of pipe
		close(pip[0]);
		//Writing to child process hardcoded
		write(pip[1],"Hello Ayush Mishra",sizeof("Hello Ayush Mishra"));
	}
	else
	{
		// creating string buffer
		char string[25];
		// closing writing end in child process
		close(pip[1]);
		// reading from parent process
		read(pip[0],string,19);
		// printing read input
		printf("%s\n",string );
	}
}