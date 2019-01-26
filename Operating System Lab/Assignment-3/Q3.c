#include<stdio.h>
#include <sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/wait.h>

int main()
{
	int pip[2];
    pipe(pip);
    pid_t pid = fork();
	if(pid < 0)
	{
		printf("Child process can't be created\n");
		return 0;
	}
	if(pid > 0)
	{
		close(pip[0]);
		write(pip[1],"Hello Ayush Mishra",19);
	}
	else
	{
		char string[25];
		close(pip[1]);
		read(pip[0],string,19);
		printf("%s\n",string );
	}
}