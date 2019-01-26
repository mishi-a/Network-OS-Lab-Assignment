#include<stdio.h>
#include <sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/wait.h>

int main()
{
	//creating new process
	pid_t pid = fork();
	if(pid < 0)
	{
		printf("Child Process can't be created\n");
	}

	if(pid == 0)
	{
	   // if child process exit
       exit(0);
	}
	else
	{
	  // wait for child to become zombie	
      sleep(5);
      //printing process table for zombie
      printf("Child Process Id is %d\n and parent process id %d\n",pid,getpid());
      system("ps -lA | grep '^. Z'");
      wait(NULL); 
      printf("Checking after zombie is cleaned\n");
      //No zombie with process id of child process now
      system("ps -lA | grep '^. Z'");
	}
}