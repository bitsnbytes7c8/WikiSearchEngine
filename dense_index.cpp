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

struct node 
{
	int docID;
	char title;
	char infobox;
	char link;
	char category;
	short int text;
};

int main()
{
	char word[100000];
	ifstream in;
	int record_size, string_length;
	long long int offset = 0;
	ofstream out;
	in.open("Index/Final_Index", ios::in | ios::binary);
	out.open("Index/Dense_Index", ios::out | ios::binary);
	int count = 0;
	while(1)
	{
		if(in.read((char*)&string_length, 4) == 0) 
		{
			break;
		}
		count++;
		in.read(word, string_length);
		word[string_length] = '\0';
		out.write((char*)&string_length, 4);
		out.write(word, string_length);
		if(strcmp(word, "preiti") == 0)
			cout<<word<<endl;
		out.write((char*)&offset, 8);
		in.read((char*)&record_size, 4);
		offset = offset + 4 + string_length + 4 + record_size*10;
		in.seekg(record_size*10, ios::cur);
	}
	cout<<count<<endl;
	out.close();
	in.close();
	return 0;
}
