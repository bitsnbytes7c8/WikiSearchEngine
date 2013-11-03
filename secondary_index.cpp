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
	char word[1000];
	ifstream in;
	ofstream out;
	int string_length;
	long long int offset= 0, pos = 0;
	in.open("Index/Dense_Index", ios::in | ios::binary);
	out.open("Index/Secondary_Index", ios::out | ios::binary);
	int count = 0, secondary_count = 0;
	while(1)
	{
		if(in.read((char*)&string_length, 4) == 0)
			break;
		if(count%1000 == 0)
		{
			secondary_count++;
			in.read(word, string_length);
			word[string_length] = '\0';
			in.read((char*)&pos, 8);
			out.write((char*)&string_length, 4);
			out.write(word, string_length);
			out.write((char*)&offset, 8);
		}
		else
		{
			in.seekg(string_length+8, ios::cur);
		}
		offset = offset + 4 + string_length + 8;
		count++;
	}
	cout<<secondary_count<<" "<<count<<endl;
	in.close();
	out.close();
	return 0;
}
