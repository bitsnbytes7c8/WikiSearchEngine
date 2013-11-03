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

int main(int argc, char **argv)
{
	ifstream f[4000];
	ofstream out;
	string s;
	priority_queue<pair<string, int>, vector<pair<string, int> >, greater<pair<string, int> > > words;
	out.open("Index/Final_Index", ios::out | ios::binary);
	char word[1000];
	char filename[1000] = "Index/";
	int no_of_files = atoi(argv[1]), i;
	for(int i=0; i<no_of_files; i++)
	{
		char fname[6];
		strcpy(filename, "Index/");
		sprintf(fname, "%d", i);
		strcat(filename, fname);
		f[i].open(filename, ios::in | ios::binary);
		int t;
		f[i].read((char*)&t, 4);
		f[i].read(word, t);
		word[t]= '\0';
		s.assign(word);
		words.push(make_pair(s, i));
	}
	int flag[4000] = {0}, n = no_of_files, min_pos;
	string min;
	int word_count=0;
	vector<node> posting_list;
	while(n)
	{
		min = words.top().first;
		min_pos = words.top().second;
		words.pop();
		posting_list.clear();
		int no_of_records, size;
		f[min_pos].read((char*)&no_of_records, 4);
		for(int j=0; j<no_of_records; j++)
		{
			node nd;
			f[min_pos].read((char*)&nd.docID, 4);
			f[min_pos].read((char*)&nd.text, 2);
			f[min_pos].read((char*)&nd.title, 1);
			f[min_pos].read((char*)&nd.infobox, 1);
			f[min_pos].read((char*)&nd.link, 1);
			f[min_pos].read((char*)&nd.category, 1);
			posting_list.push_back(nd);
		}
		string new_word = min;
		while(1)
		{
			if(words.empty())
				break;
			new_word = words.top().first;
			int j = words.top().second;
			if(new_word!=min)
				break;
			else
			{
				words.pop();
				int nor, sz;
				f[j].read((char*)&nor, 4);
				no_of_records += nor;
				for(int k=0; k<nor; k++)
				{
					node nd;
					f[j].read((char*)&nd.docID, 4);
					f[j].read((char*)&nd.text, 2);
					f[j].read((char*)&nd.title, 1);
					f[j].read((char*)&nd.infobox, 1);
					f[j].read((char*)&nd.link, 1);
					f[j].read((char*)&nd.category, 1);
					posting_list.push_back(nd);
				}
				if(f[j].read((char*)&sz, 4) == 0)
				{
					flag[j] = 1;
					n--;
				}
				else
				{
					f[j].read(word, sz);
					word[sz] = '\0';
					s.assign(word);
					words.push(make_pair(s,j));
				}
			}
		}
		int ln = min.length();
		out.write((char*)&ln, 4);
		//cout<<ln<<endl;
		out.write((char*)min.c_str(), ln);
		//cout<<min.c_str()<<endl;
		//word_count++;
		ln = posting_list.size();
		//cout<<ln<<endl;
		out.write((char*)&ln, 4);
		for(int j=0; j<ln; j++)
		{
		//	cout<<posting_list[j].docID<<" "<<posting_list[j].text<<" "<<posting_list[j].title<<" "<<posting_list[j].infobox<<" "<<posting_list[j].link<<" "<<posting_list[j].category<<endl;
			out.write((char*)&posting_list[j].docID, 4);
			out.write((char*)&posting_list[j].text, 2);
			out.write((char*)&posting_list[j].title, 1);
			out.write((char*)&posting_list[j].infobox, 1);
			out.write((char*)&posting_list[j].link, 1);
			out.write((char*)&posting_list[j].category, 1);
		}
		if(f[min_pos].read((char*)&size, 4) == 0)
		{
			flag[min_pos] = 1;
			n--;
		}
		else
		{
			f[min_pos].read(word, size);
			word[size] = '\0';
			s.assign(word);
			words.push(make_pair(s, min_pos));
		}
	}
	//cout<<word_count<<endl;
	out.close();
	return 0;
}
