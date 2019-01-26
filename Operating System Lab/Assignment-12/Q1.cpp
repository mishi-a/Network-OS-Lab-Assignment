/*************
        Author - am10
             ******************/
#include<bits/stdc++.h>
 
#define sync ios::sync_with_stdio(false);cin.tie(NULL);cout.tie(0);
#define endl '\n'
#define ll long long
#define pb push_back
#define PI acos(-1)
#define pii pair <int,int>
#define FI first
#define SE second

/*
//D-S-U
int root(int v){return par[v] < 0 ? v : (par[v] = root(par[v]));}
void merge(int x,int y){	//	x and y are some tools (vertices)
        if((x = root(x)) == (y = root(y))     return ;
	if(par[y] < par[x])	// balancing the height of the tree
		swap(x, y);
	par[x] += par[y];
	par[y] = x;
}
*/


using namespace std;

int cylinderCount = 200;
//number of request
int sz = 8;
void FCFS(int dHead,int* workQueue)
{
	cout << "FCFS disk Scheduling Algorithm :" << endl;
	cout << "Request Processed Order :" << endl;
	//Total Head movement
	int THM = 0;
	//Serving each request in order of arrival
	for(int i=0;i<sz;i++)
	{
		THM += abs(workQueue[i]-dHead);
		dHead = workQueue[i]; 
		cout << dHead << "--> ";
	}
	cout <<"null"<< endl;
	cout << "Total Head movement is " << THM << " and seek time is " << THM*5 << "ms\n" << endl;
}

void SCAN(int dHead,int* workQueue)
{
	cout << "SCAN disk Scheduling Algorithm: " << endl;
	cout << "Request Processed Order :" << endl;
	int dir = 0; // 0 for left and 1 for right
	int st = INT_MAX;
	//Finding which direction to move 
	for(int i=0;i<sz;i++)
	{
		if(abs(dHead-workQueue[i])<st)
		{
			if(workQueue[i]<dHead)
				dir = 0;
			else
				dir = 1;
			st = abs(workQueue[i]-dHead);
		}
	}
	//marking all request in array
	int* available = new int[200];
	for(int i=0;i<sz;i++)
	{
		available[workQueue[i]]++;
	}
	int tot = 0;
	int THM = 0;
	//processing request at head
	if(available[dHead]==1)
	{
		available[dHead]--;
		cout << dHead << "--> ";
		tot++;
	}
	//if left movement
	if(dir == 0)
	{
		for(int i=dHead-1;i>=0&&tot<sz;i--)
		{
			if(available[i]==1)
			{
				tot++;
				THM+=abs(dHead-i);
				available[i]--;
				dHead = i;
				cout << dHead << "--> ";
			}
		}
		//moving extra left 
		if(tot!=sz)
		{
			THM+=dHead;
			dHead = 0;
		}
		for(int i=0;i<200 && tot < sz;i++)
		{
			if(available[i]==1)
			{
				tot++;
				THM+=abs(dHead-i);
				dHead = i;
				cout << dHead << "--> ";
				available[i]--;
			}
		}
	}
	else
	{
		for(int i=dHead+1;i<200 && tot<sz;i++)
		{
			if(available[i]==1)
			{
				tot++;
				THM+=abs(dHead-i);
				dHead = i;
				available[i]--;
				cout << dHead << "--> ";
			}
		}
		//moving extra right
		if(tot!=sz)
		{
			THM+=abs(199-dHead);
			dHead = 199;
		}
		for(int i=199;i>=0 && tot < sz;i--)
		{
			if(available[i]==1)
			{
				tot++;
				THM+=abs(dHead-i);
				dHead = i;
				available[i]--;
				cout << dHead << "--> ";
			}
		}
	}
	cout<<"null" << endl;
	cout << "Total Head movement is " << THM << " and seek time is " << THM*5 << "ms\n" << endl;

}

void SSTF(int dHead,int* workQueue)
{
	cout << "SSTF disk Scheduling Algorithm: " << endl;
	cout << "Request Processed Order :" << endl;
	int size = sz;
	int THM = 0;
	int pRequest;
	//to check which request has been processed
	int* available = new int[sz];
	for(int i=0;i<sz;i++)
	{
		available[i]=1;
	}
	while(size--)
	{
		int st = INT_MAX;
		//finding minimum seek time request
		for(int i=0;i<sz;i++)
		{
			if(abs(workQueue[i]-dHead)<st && available[i]!=0)
			{
				pRequest = i;
				st = abs(workQueue[i]-dHead);
			}	
		}
		available[pRequest]--;
		THM+=abs(dHead-workQueue[pRequest]);
		dHead = workQueue[pRequest];
		cout << dHead << "--> ";
	}
	cout <<"null"<< endl;
	cout << "Total Head movement is " << THM << " and seek time is " << THM*5 << "ms\n" << endl;

}

void CSCAN(int dHead,int* workQueue)
{
	cout << "CSCAN disk Scheduling Algorithm: " << endl;
	cout << "Request Processed Order :" << endl;
	int dir = 0; // 0 for left and 1 for right
	int st = INT_MAX;
	for(int i=0;i<sz;i++)
	{
		if(abs(dHead-workQueue[i])<st)
		{
			if(workQueue[i]<dHead)
				dir = 0;
			else
				dir = 1;
			st = abs(workQueue[i]-dHead);
		}
	}
	int* available = new int[200];
	for(int i=0;i<sz;i++)
	{
		available[workQueue[i]]++;
	}
	int tot = 0;
	int THM = 0;
	if(available[dHead]==1)
	{
		available[dHead]--;
		cout << dHead << "--> ";
		tot++;
	}
	//if left movement 
	if(dir == 0)
	{
		for(int i=dHead-1;i>=0&&tot<sz;i--)
		{
			if(available[i]==1)
			{
				tot++;
				THM+=abs(dHead-i);
				available[i]--;
				dHead = i;
				cout << dHead << "--> ";
			}
		}
		//extra left
		if(tot!=sz)
		{
			THM+=dHead;
			dHead = 0;
		}
		//as CSCAN can move from 0 to 199 in one movement but no head moveement
		dHead = 199;
		for(int i=199;i>=0&& tot<sz;i--)
		{
			if(available[i]==1)
			{
				tot++;
				THM+=abs(dHead-i);
				dHead = i;
				available[i]--;
				cout << dHead << "--> ";
			}
		}
	}
	else
	{
		for(int i=dHead+1;i<200 && tot<sz;i++)
		{
			if(available[i]==1)
			{
				tot++;
				THM+=abs(dHead-i);
				dHead = i;
				available[i]--;
				cout << dHead << "--> ";
			}
		}
		if(tot!=sz)
		{
			THM+=abs(199-dHead);
			dHead = 0;
		}
		//as CSCAN move to 0 from 199
		dHead = 0;
		for(int i=0;i<200 && tot < sz;i++)
		{
			if(available[i]==1)
			{
				tot++;
				THM+=abs(dHead-i);
				dHead = i;
				available[i]--;
				cout << dHead << "--> ";
			}
		}
	}
	cout <<"null" <<  endl;
	cout << "Total Head movement is " << THM << " and seek time is " << THM*5 << "ms\n" << endl;

}

int main()
{
  sync;
  int diskHead = 50;
  //input request
  int workQueue[] = {95, 180, 34, 119, 11, 123, 62, 64 };
  
  //Calling Various Algorithm
  FCFS(diskHead,workQueue);
  SCAN(diskHead,workQueue);
  CSCAN(diskHead,workQueue);
  SSTF(diskHead,workQueue);
}
