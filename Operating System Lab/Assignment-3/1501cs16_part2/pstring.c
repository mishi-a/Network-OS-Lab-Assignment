#include<stdio.h>
#include <sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/wait.h>
#include <string.h>
int main(int argc,char* argv[])
{
	if(argc>2)
	{
		printf("Invalid input,please mention text file\n");
		return 0;
	}
	//taking input from terminal
    char *s = argv[1];
    char baseCommand[30] = "xterm -hold -e ./dstring ";
    //concating baseCommand and input file name
    strcat(baseCommand,s);
    //system call with command
    system(baseCommand);
} 