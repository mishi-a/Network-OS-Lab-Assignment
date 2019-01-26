#include<stdio.h>
#include <sys/types.h>
#include<unistd.h>
#include <sys/wait.h>
int factorial()
{
	//Blocks parent till child executes
	wait(NULL);
    //Printing current process id
	printf("Current Process id is %d\n",getpid());
	int n;
	printf("Enter the value of n to find factorial \n");
	//taking input
    scanf("%d",&n);
    if(n==0)
    	return 1;
    //iterative function to return factorial
    int factorial=1;
    for(int i=1;i<=n;i++)
    	factorial = factorial*i;
    return factorial;
}

int main()
{
	//Factorial function call befor fork
	printf("The factorial before fork is %d\n",factorial());
	//Fork() system call to create new process
	fork();
	//Factorial call after fork(),now it will be called twice one for child 
	//and one for parent process
	printf("The factorial after fork is %d\n",factorial());
}