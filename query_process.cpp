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
#include"stemmer.h"
#include<sys/time.h>

using namespace std;

float no_of_documents = 12961996.0;

class node
{
	public:
		int value;
		node *children[26];
};

struct doc
{
	int docID;
	char title;
	char infobox;
	char link;
	char category;
	short int text;
};

map<int, float> tf_idf;

struct pair_str_less
{
	bool operator()( const pair< 
			string, long long int>& lhs, 
			const string& rhs ) const 
	{ return lhs.first < rhs; }
	bool operator()( const string& lhs, 
			const pair< 
			string, long long int >& rhs ) const 
	{ return lhs < rhs.first;}
};


struct pair_int_less
{
	bool operator()( const pair< 
			int, long long int>& lhs, 
			const int& rhs ) const 
	{ return lhs.first < rhs; }
	bool operator()( const int& lhs, 
			const pair< 
			int, long long int >& rhs ) const 
	{ return lhs < rhs.first;}
};


struct CapacityGreater : public binary_function<vector<int>, vector<int>,bool>
{
	bool operator()(const vector<int> &a, const vector<int> &b) const
	{ return a.size() < b.size(); }
};

struct Ranking : public binary_function<int, int,bool>
{
	bool operator()(const pair <int, float> &a, const pair<int, float> &b) const

	{ return a.second > b.second; }
};

class Trie
{
	public:
		//node *root;
		vector<pair<string, long long int> > secondary;
		vector<pair<int, long long int> > secondary_ID;
		int count;

		Trie()
		{
			ifstream in;
			int length;
			long long int offset;
			char word[1000];
			string wrd;
			in.open("Index/Secondary_Index", ios::in | ios::binary);
			while(1)
			{
				if(in.read((char*)&length, 4) == 0)
					break;
				in.read(word, length);
				word[length] = '\0';
				in.read((char*)&offset, 8);
				wrd.assign(word);
				secondary.push_back(make_pair(wrd, offset));
				count++;
			}
			in.close();
			int id;
			in.open("Index/Secondary_Index_ID_Title", ios::in | ios::binary);
			while(1)
			{
				if(in.read((char*)&id, 4) == 0)
					break;
				in.read((char*)&offset, 8);
				secondary_ID.push_back(make_pair(id, offset));
			}
			in.close();
		}

		long long int search_secondary(int id)
		{
			int first = 0, last = secondary_ID.size()-1, mid = (first+last)/2;
			/*vector<pair<int, long long int> >::iterator low ;
			low = lower_bound(secondary_ID.begin(), secondary_ID.end(), id, pair_int_less());
			return secondary_ID[low-secondary_ID.begin()].second;*/
			while(first<=last)
			{
				if(secondary_ID[mid].first < id)
				{
					if(mid == secondary_ID.size() -1)
						return secondary_ID[mid].second;
					if(secondary_ID[mid+1].first > id)
						return secondary_ID[mid].second;
					else
						first = mid+1;
				}
				else if(secondary_ID[mid].first == id)
					return secondary_ID[mid].second;
				else
					last = mid-1;
				mid = (first+last)/2;
			}
		}


		long long int search_secondary(string word)
		{
			int first = 0, last = count-1, mid = (first+last)/2;
			vector<pair<string, long long int> >::iterator low ;
			/*low = lower_bound(secondary.begin(), secondary.end(), word, pair_str_less());
			cout<<(*low).second;
			cout<<secondary[low-secondary.begin()].second;
			return secondary[low-secondary.begin()].second;*/
			while(first<=last)
			{
				if(secondary[mid].first<word)
				{
					if(mid == secondary.size()-1)
					{
						return secondary[mid].second;
					}
					if(secondary[mid+1].first>word)
					{
						return secondary[mid].second;
					}
					else
						first = mid+1;
				}
				else if(secondary[mid].first == word)
				{
					return secondary[mid].second;
				}
				else
					last = mid-1;
				mid = (first+last)/2;
			}
		}

		long long int search_id(int id)
		{
			int index;
			ifstream in;
			in.open("Index/Dense_Index_ID_Title", ios::binary | ios::in);
			long long int offset = search_secondary(id);
			in.seekg(offset, ios::beg);
			while(1)
			{
				if(in.read((char*)&index, 4) == 0)
					return -1;
				in.read((char*)&offset, 8);
				if(index>id)
					return -1;
				else if(index == id)
					return offset;
			}
			in.close();
			return -1;
		}

		long long int search_word(char word[])
		{
			int length = strlen(word);
			int index;
			ifstream in;
			in.open("Index/Dense_Index", ios::in | ios::binary);
			string wrd;
			wrd.assign(word);
			char w[1000];
			long long int offset2 = search_secondary(wrd);
			in.seekg(offset2, ios::beg);
			long long int offset;
			while(1)
			{
				if(in.read((char*)&length, 4).eof())
				{
					return -1;
				}
				in.read(w, length);
				w[length]= '\0';
				in.read((char*)&offset, 8);
				if(strcmp(w, word)>0)
				{
					return -1;
				}
				else if(strcmp(w, word) == 0)
				{
					return offset;
				}
			}
			return -1;
		}

		void intersect(vector<pair<int, float> >& answer, vector<pair<int, float> >& a)
		{
			vector<pair<int, float> > final;
			int i, j;
			for(i=0, j=0; i<answer.size() && j<a.size(); )
			{
				if(answer[i].first == a[j].first)
				{
					final.push_back(make_pair(answer[i].first, answer[i].second + a[j].second));
					i++;
					j++;
				}
				else if(answer[i].first<a[j].first)
					i++;
				else
					j++;
			}
			answer = final;
		}


		void search_title(int id, char title[])
		{
			long long int offset;
			ifstream in;
			in.open("Index/ID_Title_Index", ios::in | ios::binary);
			offset = search_id(id);
			if(offset == -1)
				return;
			in.seekg(offset, ios::beg);
			int length;
			in.read((char*)&id, 4);
			in.read((char*)&length, 4);
			in.read(title, length);
			title[length] = '\0';
			in.close();
			return;
		}


		void search_query(vector<pair<string, int> > words, vector<pair<int, float> >& answer)
		{
			vector<pair<int, float> > docids;
			ifstream in;
			string word;
			int idf, tf;
			int flag, string_length, size;
			long long int offset;
			char w[1000];
			doc posting;
			in.open("Index/Final_Index", ios::in | ios::binary);
			for(int i=0; i<words.size(); i++)
			{
				string word = words[i].first;
				w[word.size()] = '\0';
				memcpy(w, word.c_str(), word.size());
				flag = words[i].second;
				offset = search_word(w);
				if(offset == -1)
					continue;
				in.seekg(offset, ios::beg);
				in.read((char*)&string_length, 4);
				in.read(w, string_length);
				//cout<<"Found word "<<w<<endl;
				in.read((char*)&size, 4);
				//cout<<size<<endl;
				docids.clear();
				for(int j=0; j<size; j++)
				{
					in.read((char*)&posting.docID, 4);
					in.read((char*)&posting.text, 2);
					in.read((char*)&posting.category, 1);
					in.read((char*)&posting.title, 1);
					in.read((char*)&posting.link, 1);
					in.read((char*)&posting.infobox, 1);
					//cout<<flag<<" "<<posting.title<<endl;
					if((flag == 1 && posting.text>0) || (flag == 2 && posting.title>0) || (flag == 3 && posting.infobox>0) || (flag == 4 && posting.link>0) || (flag == 5 && posting.category>0))
					{
						switch(flag)
						{
							case 1: tf = posting.text;
											break;
							case 2: tf = posting.title;
											break;
							case 3: tf = posting.infobox;
											break;
							case 4: tf = posting.link;
											break;
							case 5: tf = posting.category;
											break;
						}
						docids.push_back(make_pair(posting.docID, tf));
					}
				}
				int idf = docids.size();
				for(int k=0; k<idf; k++)
				{
					docids[k].second *= log(no_of_documents/idf);
				}
				sort(docids.begin(), docids.end());
				if(!answer.empty())
				{
					intersect(answer, docids);
				}
				else
				{
					answer = docids;
				}
			}
			sort(answer.begin(), answer.end(), Ranking());
		}
};

void parse_query(char query[], vector<pair<string, int> >&query_vector)
{
	char word[100][1000], word2[2];
	for(int i=0; query[i]!='\0' && query[i+1] != '\0' && query[i+2] != '\0'; i++)
	{
		int word_count = 0;
		while(1)
		{
			if(isalpha(query[i]))
			{
				int count = 0;
				while(isalpha(query[i]))
				{
					word[word_count][count++] = query[i++];
				}
				word[word_count++][count] = '\0';
			}
			else
			{
				i++;
			}
			if(query[i] == '\0') {
				break;
			}
			else if(word_count>=2 && query[i] == ' ' && query[i+2] == ' ') {
				break;
			}
		}
		int flag;
		if(strcmp(word[0], "T") == 0)
			flag = 2;
		else if(strcmp(word[0], "B") == 0)
			flag = 1;
		else if(strcmp(word[0], "I") == 0)
			flag = 3;
		else if(strcmp(word[0], "C") == 0)
			flag = 5;
		else if(strcmp(word[0], "O") == 0)
			flag = 4;
		string wrd;
		for(int j=1; j<word_count; j++)
		{
			stemfile(word[j]);
			wrd.assign(word[j]);
			//cout<<wrd<<" "<<flag<<endl;
			query_vector.push_back(make_pair(wrd, flag));
		}
		if(query[i] == '\0')
			break;
	}
}

int main()
{
	s = (char *) malloc(i_max+1);
	Trie t;
	char query[10000];
	struct timeval tv, tv2;
	vector<pair<string, int> > query_vector;
	vector<pair<int, float> > answer;
	char title[1000];
	while(1)
	{
		query_vector.clear();
		answer.clear();
		//cout<<"$Search_Wikipedia$ ";
		scanf("%[^\n]", query);
		gettimeofday(&tv, NULL);
		getchar();
		if(strcmp(query, "quit") == 0)
			break;
		cout<<endl;
		parse_query(query, query_vector);
		t.search_query(query_vector, answer);
		for(int i=0; i<answer.size()&&i<15; i++)
		{
			//cout<<answer[i].first<<endl;
			t.search_title(answer[i].first, title);
			cout<<title<<endl;
		}
		cout<<endl;
		gettimeofday(&tv2, NULL);
		cout<<(((float)tv2.tv_usec-(float)tv.tv_usec)/1000.0) + tv2.tv_sec*1000 - tv.tv_sec*1000<<endl;
	}
	return 0;
}
