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
	in.open("Index/ID_Title_Index", ios::in | ios::binary);
	out.open("Index/Dense_Index_ID_Title", ios::out | ios::binary);
	int id, length;
	long long int offset = 0;
	char title[1000];
	int count = 0;
	while(1)
	{
		if(in.read((char*)&id, 4) == 0)
			break;
		count++;
		in.read((char*)&length, 4);
		in.read(title, length);
		out.write((char*)&id, 4);
		out.write((char*)&offset, 8);
		offset = offset + 4 + 4 + length;
	}
	cout<<count<<endl;
	in.close();
	out.close();
	return 0;
}
