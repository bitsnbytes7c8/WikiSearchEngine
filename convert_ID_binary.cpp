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
#include<ctype.h>

using namespace std;

int main()
{
	int id;
	char title[1000], ID[100];
	ifstream in;
	ofstream out;
	in.open("docId.txt", ios::in);
	out.open("Index/ID_Title_Index", ios::out | ios::binary);
	string line;
	while(1)
	{
		getline(in, line);
	//	cout<<line<<endl;
		if(line.length() == 0)
			break;
		for(int k=0; line[k]!='\0'; k++)
		{
			if(isdigit(line[k]))
			{
				int count = 0;
				while(isdigit(line[k]))
				{
					ID[count++] = line[k++];
				}
				ID[count] = '\0';
				id = atoi(ID);
			}
			else if(line[k] == '`')
			{
				k++;
				int count = 0;
				while(line[k]!='`')
				{
					title[count++] = line[k++];
				}
				title[count] = '\0';
				break;
			}
			else
				k++;
		}
	//	cout<<id<<" "<<title<<endl;
		out.write((char*)&id, 4);
		int length = strlen(title);
		out.write((char*)&length, 4);
		out.write(title, strlen(title));
	}
	in.close();
	in.open("docId2.txt", ios::in);
	while(1)
	{
		getline(in, line);
	//	cout<<line<<endl;
		if(line.length() == 0)
			break;
		for(int k=0; line[k]!='\0'; k++)
		{
			if(isdigit(line[k]))
			{
				int count = 0;
				while(isdigit(line[k]))
				{
					ID[count++] = line[k++];
				}
				ID[count] = '\0';
				id = atoi(ID);
			}
			else if(line[k] == '`')
			{
				k++;
				int count = 0;
				while(line[k]!='`')
				{
					title[count++] = line[k++];
				}
				title[count] = '\0';
				break;
			}
			else
				k++;
		}
	//	cout<<id<<" "<<title<<endl;
		out.write((char*)&id, 4);
		int length = strlen(title);
		out.write((char*)&length, 4);
		out.write(title, strlen(title));
	}
	in.close();
	out.close();
	return 0;
}
