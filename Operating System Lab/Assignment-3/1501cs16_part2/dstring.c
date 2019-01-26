#include<stdio.h>
#include <sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/wait.h>

int main(int argc,char* argv[])
{
	char* fileName = argv[1];
	if(argc>2)
	{
		printf("Invalid input,please mention text file\n");
		return 0;
	}
	FILE* ifp = fopen(fileName,"r");
	char iWord[20];
	int cnt = 0;
	while(fscanf(ifp,"%s",iWord)!=EOF)
	{
      cnt++;
      if(cnt%3==0)
      {
      	printf("%s \n",iWord);
      }
	}
}