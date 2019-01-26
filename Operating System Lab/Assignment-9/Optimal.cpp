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

using namespace std;

//find next used term
int check(vector<int>&sequences,int val,int idx)
{
  for(int i=idx;i<sequences.size();i++)
  {
  	if(sequences[i]==val)
  		return i;
  }
  return sequences.size()+1;
}

//checking if it is present in pattern
bool checkAvailable(int i,vector<int >& v)
{
	for(int j=0;j<v.size();++j)
	{
		if(v[j] == i)
			return true;
	}
	return false;
}

void Optimal(int frameSize,vector<int>&sequences)
{
	int ans[frameSize][sequences.size()];
	memset(ans,0,sizeof(ans));
	int pageFault = 0;
				
	vector<int > pattern;
	cout << "Optimal :" << endl;

	for(int i=0;i<sequences.size();i++)
	{
		//initializing with previous frame content
		for(int j=0;j<frameSize;j++)
		{
			if(i==0)
				break;
			ans[j][i] = ans[j][i-1];
		}
		bool found = false;
		//Checking if current page present
		for(int j=0;j<frameSize;j++)
		{
			if(ans[j][i]==sequences[i])
			{
				found = true;
				break;
			}
		}
		//if not present
		if(!found)
		{
			int val = -1;
			int ridx;
			bool flag1 = false;
			for(int j=0;j<frameSize;j++)
			{
				//if frame available
				if(ans[j][i]==0)
				{
					ans[j][i] = sequences[i];
					pageFault++;
					flag1 = true;
					pattern.pb(i);
					break;
				}
				else
				{
					//check how long this page is not used
					int val1 = check(sequences,ans[j][i],i+1);
					
					if(val1>=val)
					{
						bool use = true;
						//in case of clash replace oldest one
						if(val1==val)
						{
							int x = i-1;
							for(int k=i-1;k>=0;k--)
							{
								bool found1 = false;
								for(int l=0;l<frameSize;l++)
								{
									if(ans[l][k]==ans[j][i])
									{
										found1=true;
										x=k;
									}
								}
								if(found1==false)
									break;
							}
							int y=i-1;
							for(int k=i-1;k>=0;k--)
							{
								bool found1 = false;
								for(int l=0;l<frameSize;l++)
								{
									if(ans[l][k]==ans[ridx][i])
									{
										found1=true;
										y=k;
									}
								}
								if(found1==false)
									break;
							}
							if(x<y)
								ridx = j;
							use = false;
						}
						if(use)
						{
							val = val1;
							ridx = j;
						}
						
					}
				}
			}
			//replace page
			if(!flag1)
			{
				ans[ridx][i] = sequences[i];
				pattern.pb(i);
				pageFault++;
			}
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
	cout<<"Optimal          "<<	pageFault<<	endl;
}

int main()
{
  sync;
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
		Optimal(frameSize,sequences);
   }
}
