#include <bits/stdc++.h>

using namespace std;

int arrivalTime[5],burstTime[5],QueueNumber[5];
vector<pair<int,int> > info;
int cBurstTime[5];
priority_queue<pair<int,int>,vector< pair<int,int> >, greater< pair<int,int> > >Q1;
vector<int> Q2;
int turnAroundTime[5];
int taken[5];

int add_process(int cTime,int fTime)
{
	int flag = 1;
    for(int i=0;i<5;i++)
    {
    	if(taken[info[i].second]==0 && arrivalTime[info[i].second]>=cTime && arrivalTime[info[i].second]<fTime)
    	{
    		if(QueueNumber[info[i].second]==1)
    			taken[info[i].second]=1,Q1.push({cBurstTime[info[i].second],info[i].second}),flag=0;
    		else
    			taken[info[i].second]=1,Q2.push_back(info[i].second);
    	}
    }
    return flag;
}

int main()
{
	ifstream infile("scheduler.txt.txt");
    string a;
	infile >> a >> a >> a >> a;
	int idx=1;
	while(infile>> a >> arrivalTime[idx] >> burstTime[idx] >> QueueNumber[idx])
	{
      cout << "Reading Row " << idx << ".... \n";
      cBurstTime[idx]=burstTime[idx];
      idx++;
	}
    for(int i=1;i<5;i++)
    	info.push_back({arrivalTime[i],i});
    sort(info.begin(),info.end());
    int curTime = info[0].first;
    int finalTime = info[4].first;
    add_process(curTime,curTime+1);
    cout << curTime ;
    while(curTime<=finalTime)
    {
      while(!Q1.empty())
      {
      	 pair <int,int > val = Q1.top();
         int processId = val.second;
         Q1.pop();
         add_process(curTime,curTime+burstTime[processId]);
         cout << "-->P" << processId <<"--->" << curTime+burstTime[processId];
         curTime+=burstTime[processId];
         turnAroundTime[processId] = curTime-arrivalTime[processId];
      }
      while(1)
      {
      	 int k=0;
      	 int pre=0;
         for(int i=0;i<Q2.size();i++)
         {
         	if(burstTime[Q2[i]]>0)
         	{
         		pre=1;
         		burstTime[Q2[i]]--;
         		curTime++;
         		cout << "---P" << Q2[i] << "--->" << curTime ;
         		if(burstTime[Q2[i]]==0)
         		{
         			turnAroundTime[Q2[i]] = curTime-arrivalTime[Q2[i]];
         		}
         		if(!add_process(curTime,curTime+1))
         		{
         			k = 1;
         			break;
         		}
         	}
         }
         if(k==1||pre==0)
         	break;
         if(pre==0 && Q1.empty())
         {
         	if(curTime<finalTime)
         	{
         		add_process(curTime,curTime+1);
         		curTime++;
         	}
         }
      }
    }
    while(!Q1.empty())
    {
	    pair <int,int > val = Q1.top();
        int processId = val.second;
        Q1.pop();
        add_process(curTime,curTime+burstTime[processId]);
        cout << "-->P" << processId <<"--->" << curTime+burstTime[processId]; 
        curTime+=burstTime[processId];
        turnAroundTime[processId] = curTime-arrivalTime[processId];
    }
    while(1)
    {
    	int pre = 0;
    	for(int i=0;i<Q2.size();i++)
        {
         	if(burstTime[Q2[i]]>0)
         	{
         		pre=1;
         		burstTime[Q2[i]]--;
         		cout << "---P" << Q2[i] << "--->" << curTime+1 ;
         		curTime++;
         		if(burstTime[Q2[i]]==0)
         		{
         			turnAroundTime[Q2[i]] = curTime-arrivalTime[Q2[i]];
         		}
         	}
        }
        if(pre == 0)
        	break;
    }
    int totTurnAroundTime=0;
    int totWaitingTime =0;
    for(int i=1;i<5;i++)
    {
    	totTurnAroundTime+=turnAroundTime[i];
    	totWaitingTime+=(turnAroundTime[i]-cBurstTime[i]);
    	//cout << turnAroundTime[i]  << " " << burstTime[i] <<  endl;
    }
    cout << endl;
    cout << "Avg turnAroundTime " << totTurnAroundTime/4.0 << endl;
    cout << "Avg waitingTime " << totWaitingTime/4.0 << endl;

}