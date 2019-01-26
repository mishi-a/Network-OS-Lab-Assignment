#include <stdio.h>
#include <stdlib.h>

int *heapSegmentVar;
//variable in bss segment as uninitialized
int bssSegmentVar;

void textSegmentVar()
{
	return;
}

int main()
{
	//variable in data segment
	static int dataSegmentVar = 5;
	//var in stack
	int stackSegmentVar = 10;
    
    //allocation in heap
    heapSegmentVar = (int*) malloc(5*sizeof(int));
    
    //code in text section
    textSegmentVar();
	printf("Address of Variable in text section %p\n",&textSegmentVar);
	printf("Address of Variable in bssSegmentVar %p\n",&bssSegmentVar);
	printf("Address of Variable in stackSegmentVar %p\n",&stackSegmentVar);
	printf("Address of Variable in heapSection %p\n",heapSegmentVar);

	printf("Size of each segment : \n");
	system("size a.out");

}