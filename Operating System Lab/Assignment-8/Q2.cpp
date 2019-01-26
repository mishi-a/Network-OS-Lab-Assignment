#include<bits/stdc++.h>

using namespace std;

bool checkAvailable(int i,vector<int >& v)
{
	for(int j=0;j<v.size();++j)
	{
		if(v[j] == i)
			return true;
	}
	return false;
}

void FIFO(int frameSize,vector<int>&sequences)
{
		int ans[frameSize][sequences.size()];
		memset(ans,0,sizeof(ans));
		int oldest = 0;
		int pageFault = 0;
				
		vector<int > pattern;
		cout << "FIFO :" << endl;

		//iterating over sequences
		for(int start=0;start<sequences.size();start++)
		{
			bool found = false;
			for(int j=0;j<frameSize;j++)
			{
				if(start==0)
					break;
				ans[j][start] = ans[j][start-1];
			}
			// checking if available in previously
			for(int j=0;j<frameSize;j++)
			{
				if(ans[j][start] == sequences[start])
				{
					found = true;
					break;
				}
			}

			//if not present replace the oldest one
			if(found == false)
			{
				ans[oldest][start] = sequences[start];
				oldest = (oldest+1);
				if(oldest+1>frameSize)
					oldest = 0;
				pageFault++;
				pattern.push_back(start);
			}
		}		

		//Printing the output.
		for(int k=0;k<frameSize;++k)
		{
			for(int i = 0;i<sequences.size();++i )
			{
				if(checkAvailable(i,pattern))
				{
					cout<<ans[k][i]<<" ";
				}
				else cout<<"  ";
			}
			cout<<endl;
		}
		cout<<"\nUsing "<< frameSize << "frames ,the reference string yielding :"<<endl;
		cout<<"Scheme        #faults "<<	endl;
		cout<<"FIFO          "<<	pageFault<<	endl;
}

int main()
{
	FILE *fp = fopen("page.txt","r");

	int frameSize;
	
	//reading input
	while(fscanf(fp,"%d",&frameSize)!=EOF)
	{
		vector<int> sequences;
		int x;
		while(fscanf(fp,"%d",&x)!=EOF)
		{
			if(x == -1)
			break;
			sequences.push_back(x);
		}
		//calling FIFO scheme
		FIFO(frameSize,sequences);
		
	}
}
