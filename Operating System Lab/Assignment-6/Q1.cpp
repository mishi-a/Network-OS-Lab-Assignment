//Scheduler File must be in order

#include <bits/stdc++.h>

#define pb push_back

using namespace std;

vector<int> arrivalTime,burstTime,QueueNumber,copyBurstTime;
vector<pair<int,int> > info;
vector<string> processNo;
priority_queue<pair<int,int>,vector< pair<int,int> >, greater< pair<int,int> > >Q1;
deque<int> Q2;
int lastAddedTime;
int* turnAroundTime;

//method to add process to respective queue
int add_process(int cTime,int fTime)
{
	int flag = 1;
    for(int i=0;i<info.size();i++)
    {
    	if(arrivalTime[info[i].second]>=cTime && arrivalTime[info[i].second]<fTime)
    	{
    		lastAddedTime = arrivalTime[info[i].second]; 
    		if(QueueNumber[info[i].second]==1)
    			Q1.push({copyBurstTime[info[i].second],info[i].second}),flag=0;
    		else
    			Q2.push_back(info[i].second);
    	}
    }
    return flag;
}

//method to find next arrival time
int findNextArrival(int t)
{
	for(int i=0;i<info.size();i++)
	{
		if(info[i].first>t)
			return info[i].first;
	}
	return 0;
}
int main()
{
	ifstream infile("scheduler.txt.txt");
    string a;
    infile >> a >> a >> a >> a;
    arrivalTime.pb(-1);
    burstTime.pb(-1);
    QueueNumber.pb(-1);
    copyBurstTime.pb(-1);
    processNo.pb(a);
    int idx=1,d,b,c; 
	//Reading from file
    while(infile>> a >> b >> c >> d)
	  {
      cout << "Reading Row " << idx << ".... \n";
      arrivalTime.pb(b);
      burstTime.pb(c);
      processNo.pb(a);
      QueueNumber.pb(d);
      copyBurstTime.pb(c);      
      idx++;
    }
    
    turnAroundTime = new int[arrivalTime.size()];
    
    //sorting given process as arrival time
    for(int i=1;i<arrivalTime.size();i++)
    	info.push_back({arrivalTime[i],i});
    sort(info.begin(),info.end());

    cout << "\n\n****************************\n*\tGAANT CHART\t   *\n****************************\n";
    
    lastAddedTime = info[0].first;
    int curTime = lastAddedTime;
    int finalTime = info[info.size()-1].first;
    add_process(lastAddedTime,lastAddedTime+1);
    idx = 0;
    int cnt =0 ;
    cout << curTime;
    //loop till all process has been processed
    while(lastAddedTime<=finalTime)
    {
       cnt++;
       int flag = 0;

       //For SJF
       while(!Q1.empty())
       {
       	 flag = 1;
      	 pair <int,int > val = Q1.top();
         int processId = val.second;
         Q1.pop();
         add_process(curTime+1,curTime+burstTime[processId]+1);
         curTime+=burstTime[processId];
         cout << "-->"<< processNo[processId] <<"--->" << curTime;
         turnAroundTime[processId] = curTime-arrivalTime[processId];
       }

       //For Round Robin
       while(1)
       {
       	 //indication for Q1 process
       	 int k = 0,p=0;
         while(Q2.size()>0)
         {
         	flag = 1;
            burstTime[Q2[idx]]--;
            curTime++;
            cout << "---" << processNo[Q2[idx]] << "--->" << curTime ;
            if(burstTime[Q2[idx]] == 0)
            {
            	turnAroundTime[Q2[idx]] = curTime-arrivalTime[Q2[idx]];
            }
            int val = Q2[idx];
            Q2.pop_front();

            if(burstTime[val]>0)
            	Q2.push_back(val);

            if(curTime>lastAddedTime)
            {
               if(!add_process(curTime,curTime+1))
         	   {
         			k = 1;
         			break;
         	   }
            }
         }

         //Q1 process is added
         if(k==1||p==0)
         	break;
       }
       if(flag == 0 && lastAddedTime == finalTime)
       	 break;
       else if(flag == 0 && lastAddedTime<finalTime)
       {
       	  lastAddedTime = findNextArrival(lastAddedTime);
       	  cout << "---CPU IDLE ---->" << lastAddedTime;
       	  curTime = lastAddedTime;	 
       	  add_process(lastAddedTime,lastAddedTime+1);
       }
    }
    int totTurnAroundTime=0;
    int totWaitingTime =0;
    
    //calculating turnaround and waiting time
    for(int i=1;i<arrivalTime.size();i++)
    {
    	totTurnAroundTime+=turnAroundTime[i];
    	totWaitingTime+=(turnAroundTime[i]-copyBurstTime[i]);
    	//cout << turnAroundTime[i]  << " " << burstTime[i] <<  endl;
    }
    cout << endl;

    cout << "\n\nRequired Measures   :" << endl;
    cout << "Avg turnAroundTime " << totTurnAroundTime*1.0/(arrivalTime.size()-1) << endl;
    cout << "Avg waitingTime " << totWaitingTime*1.0/(arrivalTime.size()-1) << endl;
}