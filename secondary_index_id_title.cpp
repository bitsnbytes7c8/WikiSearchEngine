#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cmath>
#include<cstring>
#include<string>
#include<vector>
#include<map>
#include<stack>
#include<queue>
#include<algorithm>
#include<fstream>

using namespace std;

int main()
{
	ifstream in;
	ofstream out;
	in.open("Index/Dense_Index_ID_Title", ios::in|ios::binary);
	out.open("Index/Secondary_Index_ID_Title", ios::out|ios::binary);
	int id;
	long long int offset = 0, pos = 0;
	int count= 0;
	while(1)
	{
		if(in.read((char*)&id, 4) == 0)
			break;
		in.read((char*)&pos, 8);
		if(count%100 == 0)
		{
			out.write((char*)&id, 4);
			out.write((char*)&offset, 8);
		}
		count++;
		offset = offset + 12;
	}
	cout<<count<<endl;
	in.close();
	out.close();
	return 0;
}
