#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <ctype.h>
#include <errno.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include "PageTable.h"
#include<time.h>

int page,frame;
page_table_pointer PageTable;
int diskaccess=0;
int * FrameTable;

//Function to initialize Page Table
void initializePageTable()
{
	for(int i=0;i<page;i++)
	{
		PageTable[i].Valid = 0;
		PageTable[i].Frame = -1;
		PageTable[i].Dirty = 0;
		PageTable[i].Requested = 0;
	}
}

//Function to find Empty frame
int findEmptyFrame()
{
	for(int i=0;i<frame;i++)
	{
		//If entry is -1 means it's empty
		if(FrameTable[i]==-1)
			return i;
	}
	return -1;
}

//Funtion to print page table
void printPageTable()
{
	int i;
	for (i =  0;i < page;i++)
    {
        printf("%2d: Valid=%1d Frame=%2d Dirty=%1d Requested=%1d\n",i,PageTable[i].Valid,PageTable[i].Frame,PageTable[i].Dirty,
PageTable[i].Requested);
    }
}

//Main Function to handle request from MMU 
void ProcessRequest(int signal)
{
	int requestedPage = -1;
	//Finding which page is requested
	for(int i=0;i<page;i++)
	{
		if(PageTable[i].Requested!=0)
		{
			requestedPage = i;
			break;
		}
	}
	//If no request is there
	if(requestedPage == -1)
	{
		printf("The MMU has finished\n");
		printPageTable();
		printf("%d Disk access required\n",diskaccess);
		//Detach shared space from address space of this process
		if (shmdt(PageTable) == -1)
		{
        	perror("ERROR: Error detaching segment\n");
    	}
		exit(EXIT_FAILURE);
	}
	//If page request then Requested field in page table contain id of MMU process
	int MMUID = PageTable[requestedPage].Requested;
	printf("Process %d has requested Page %d\n",MMUID,requestedPage);

	//Find empty frame number
	int availableFrame = findEmptyFrame();
	//If frame empty
	if(availableFrame != -1)
	{
		printf("Put it in free frame %d\n",availableFrame);
		FrameTable[availableFrame] = requestedPage;
		PageTable[requestedPage].Frame = availableFrame;
		PageTable[requestedPage].Valid = 1;

		//Increase disk access
		diskaccess++;
		sleep(1);
		//Send signal to MMU that page has been put in ram
		kill(MMUID,SIGCONT);
	}
	else
	{
		//Random Page Replacement algorithm is used to choose victim page
		srand(time(0));
		int frameToBeReplaced = rand()%frame;
		int pageToBeReplaced = FrameTable[frameToBeReplaced];
		PageTable[pageToBeReplaced].Valid = 0;
		PageTable[pageToBeReplaced].Frame = -1;
		printf("Chose a victim page %d\n",pageToBeReplaced);
		
		//If page to be replaced is dirty write it to the disk
		if(PageTable[pageToBeReplaced].Dirty == 1)
		{
			printf("Victim is dirty, write out\n");
			diskaccess++;
			sleep(1);
		}
		FrameTable[frameToBeReplaced]=requestedPage;
		PageTable[pageToBeReplaced].Dirty = 0;
		PageTable[requestedPage].Frame = frameToBeReplaced;
		PageTable[requestedPage].Valid = 1;
		printf("Put in victim's frame %d\n",frameToBeReplaced);
		diskaccess++;
		sleep(1);
		//Send signal to MMU that Page has been put in ram
		kill(MMUID,SIGCONT);
	}

	//Set Requested Field to zero
	PageTable[requestedPage].Requested = 0;
	printf("Unblock MMU\n");

	//Print Page Table
	printPageTable();
}


int main(int argc ,char** argv)
{
	//Checking for correct Input
	if(argc != 3)
	{
		printf("Invalid input");
		exit(EXIT_FAILURE);
	}  

	//Input from command line
	page = atoi(argv[1]);
	frame = atoi(argv[2]);

	//FrameTable to keep track of pages in frame
    FrameTable = malloc(frame*sizeof(int));
    //initializing frame with -1
    for(int i=0;i<frame;i++)
	{
		FrameTable[i] = -1;
	}
	//getting key for shared Memory to be equal to process Id of this program
	int SharedMemoryKey = getpid();

	int SegmentID;
	
	//Getting address space in shared memory
	SegmentID = shmget(SharedMemoryKey,page*sizeof(page_table_entry),IPC_CREAT | 0666);
	if(SegmentID == -1)
	{
	      perror("ERROR: Could not get page table");
          exit(EXIT_FAILURE);
    }
    //Attaching this process to shared memory
	PageTable = (page_table_pointer)shmat(SegmentID,NULL,0);
	if(PageTable == NULL)
	{
		perror("ERROR: Could not get page table");
        exit(EXIT_FAILURE);
	}
	printf("The shared memory key (PID) is %d\n",SharedMemoryKey);
	//initializing page table
	initializePageTable();
	printf("Initialized page table\n");

	//Waiting for Signal from MMU if received control passes to process Request
	signal(SIGUSR1,ProcessRequest);
	while(1)
	{
		sleep(1);
	}
}
