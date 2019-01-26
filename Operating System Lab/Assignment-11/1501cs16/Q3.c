#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node
{
	struct node* ptr;
	int val;
};

struct node* arr[100];
char* fileMap[100];
int mapIdx[100];
int sz[100];

void printMenu()
{
	printf("Enter : \n");
	printf("1. To add File\n");
	printf("2. To Print Directory \n");
	printf("3. TO exit\n");

}

int main()
{
	int numBlock;
	printf("Welcome to the linked list File\n");
	printf("Enter the number of Block in disk\n");
	scanf("%d",&numBlock);
	int* disk = malloc(sizeof(int)*numBlock);
	for(int i=0;i<numBlock;i++)
		disk[i]=-1;
	int totFile = 0;
	while(1)
	{
		printMenu();
		int choice;
		scanf("%d",&choice);
		if(choice == 1)
		{
			char* fName = malloc(sizeof(char)*1000);
			int sBlock,tBlock;
			printf("Enter File name,start block and total number of block in file\n");
			scanf("%s%d%d",fName,&sBlock,&tBlock);
			if(sBlock <0 || sBlock >=numBlock ||disk[sBlock]!=-1)
			{
				printf("Start Block is not empty or invalid\n");
				continue;
			}
			int* freeBlock = malloc(sizeof(int)*tBlock);
			int j=0;
			int cnt = 0;
			freeBlock[0] = sBlock;
			cnt++;
			for(int i=1;i<tBlock;i++)
			{
				for(;j<numBlock;j++)
				{
					if(disk[j]==-1)
					{
						freeBlock[i] = j;
						cnt++;
						j++;
						break;
					}
				}
			}
			if(cnt == tBlock)
			{
				disk[sBlock] = totFile;
				//printf("freeBlock\n");
				struct node* head = NULL;
				for(int i=0;i<tBlock;i++)
				{
					struct node* idx = malloc(sizeof(struct node));
					idx->val = freeBlock[i];
					idx->ptr = NULL;
					if(head == NULL)
					{
						arr[totFile] =idx;
						head = idx;
					}
					else
					{
						head->ptr = idx;
						head = idx;
					}
					disk[freeBlock[i]] = totFile;
				}
				printf("\n");
				fileMap[totFile] = fName;				
				sz[totFile]=tBlock;
				totFile++;
			}
			else
			{
				printf("Not Enough free Space\n");
			}
		}
		else if(choice == 2)
		{
			printf("File name     Block Stored\n");
			for(int i=0;i<totFile;i++)
			{
				printf("%s 		",fileMap[i]);
				struct node* head = arr[i];
				while(head!=NULL)
				{
					printf("%d ->",head->val);
					head = head->ptr;
				}
				printf("NULL\n");
			}
		}
		else if(choice == 3)
		{
			break;
		}
		else
		{
			printf("Invalid Input\n");
		}
	}
}