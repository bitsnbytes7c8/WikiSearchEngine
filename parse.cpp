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
#include"rapidxml/rapidxml.hpp"
#include<fstream>
#include<cctype>
#include"stemmer.h"

using namespace std;
using namespace rapidxml;

char buffer[2000000000];


struct node
{
	int docID;
	char title;
	char infobox;
	char link;
	char category;
	short int text;
};

map<string, vector<node> > inverted_index;
map<string, int> stop_words;
map<string, vector<int> >::iterator iter;

void parse(char doid[], string title, string txt, int page_count)
{
	ofstream oidtitle;
	oidtitle.open("Index/ID_Title_Index", ios::app | ios::binary);
	map<string, vector<int> > temp_map;
	node new_node;
	int k = 0, l=0;
	char word[10000];
	string wrd;
	stack<char> brack;
	int count;
	page_count = atoi(doid);
	oidtitle.write((char*)&page_count, 4);
	int title_length = title.length();
	oidtitle.write((char*)&title_length, 4);
	oidtitle.write((char*)title.c_str(), title.length());
	oidtitle.close();
	//Parsing Title
	while(title[l] != '\0')
	{
		if(isalpha(title[l]))
		{
			count = 0;
			while(isalpha(title[l]))
			{
				word[count++] = title[l++];
			}
			word[count] = '\0';
			stemfile(word);
			wrd.assign(word);
			if(stop_words.find(wrd) != stop_words.end())
			{
			}
			else if(temp_map.find(wrd) != temp_map.end())
			{
				temp_map[wrd][0] ++;
			}
			else
			{
				vector<int> f (6, 0);
				temp_map[wrd] = f;
				temp_map[wrd][0] = 1;
			}
		}
		else
		{
			l++;
		}
	}
	//Parsing Infobox
	if(txt.length() >= 9 && txt[0] == '{' && txt[1] == '{' && txt[2] == 'I' && txt[3] == 'n' && txt[4] == 'f' && txt[5] == 'o' && txt[6] == 'b' && txt[7] == 'o' && txt[8] == 'x')
	{
		brack.push(txt[0]);
		k = 1;
		while(!brack.empty())
		{
			if(isalpha(txt[k]))
			{
				count = 0;
				while(isalpha(txt[k]))
				{
					word[count++] = txt[k++];
				}
				word[count] = '\0';
				stemfile(word);
				wrd.assign(word);
				if(stop_words.find(wrd) != stop_words.end())
				{
				}
				else if(temp_map.find(wrd) != temp_map.end())
				{
					temp_map[wrd][1] ++;
				}
				else
				{
					vector<int> f (6, 0);
					temp_map[wrd] = f;
					temp_map[wrd][1] = 1;
				}
			}

			//Outlinks : URL
			else if(txt[k] == '[' && txt.length()-k>=6 && txt[k+1] == 'h' && txt[k+2] == 't' && txt[k+3] == 't' && txt[k+4] == 'p' && txt[k+5] == ':')
			{
				k++;
				count = 0;
				while(txt[k] != ']' && txt[k] != ' ')
				{
					word[count++] = txt[k++];
				}
				word[count] = '\0';
				stemfile(word);
				wrd.assign(word);
				if(stop_words.find(wrd) != stop_words.end())
				{
				}
				else if(temp_map.find(wrd) != temp_map.end())
				{
					temp_map[wrd][2] ++;
				}
				else
				{
					vector<int> f (6, 0);
					temp_map[wrd] = f;
					temp_map[wrd][2] = 1;
				}
				while(txt[k] != ']')
				{
					if(isalpha(txt[k]))
					{
						count = 0;
						while(isalpha(txt[k]))
						{
							word[count++] = txt[k++];
						}
						word[count] = '\0';
						stemfile(word);
						wrd.assign(word);
						if(stop_words.find(wrd) != stop_words.end())
						{
						}
						else if(temp_map.find(wrd) != temp_map.end())
						{
							temp_map[wrd][2] ++;
						}
						else
						{
							vector<int> f (6, 0);
							temp_map[wrd] = f;
							temp_map[wrd][2] = 1;
						}
					}
						else
							k++;
					
				}
			}

			// Outlinks : Internal
			else if(txt[k] == '[' && txt[k+1] == '[')
			{
				k+=2;
				while(!(txt[k] == ']' && txt[k+1] == ']'))
				{
					if(isalpha(txt[k]))
					{
						count = 0;
						while(isalpha(txt[k]))
						{
							word[count++]= txt[k++];
						}
						word[count] = '\0';
						stemfile(word);
						wrd.assign(word);
						if(stop_words.find(wrd) != stop_words.end())
						{
						}
						else if(temp_map.find(wrd) != temp_map.end())
						{
							temp_map[wrd][2] ++;
						}
						else
						{
							vector<int> f (6, 0);
							temp_map[wrd] = f;
							temp_map[wrd][2] = 1;
						}
					}
					else
						k++;
				}
			}

			// Pipes : Ignore Word
			else if(txt[k] == '|' && (isalpha(txt[k+1]) ))
			{
				k++;
				if(isalpha(txt[k]))
				{
					count = 0;
					while(isalpha(txt[k]))
					{
						word[count++] = txt[k++];
					}
					word[count] = '\0';
				}
				else
					k++;
			}

			else if(txt[k] == '{')
			{
				brack.push(txt[k]);
				k++;
			}
			else if(txt[k] == '}')
			{
				brack.pop();
				k++;
			}
			else
			{
				k++;
			}
		}
	}
	
	//PARSING CONTENT
	while(txt[k]!='\0')
	{
		if(isalpha(txt[k]) )
		{
			count = 0;
			while(isalpha(txt[k]))
			{
				word[count++] = txt[k++];
			}
			word[count] = '\0';
			stemfile(word);
			wrd.assign(word);
			if(stop_words.find(wrd) != stop_words.end())
			{
			}
			else if(temp_map.find(wrd) != temp_map.end())
			{
				temp_map[wrd][5] ++;
			}
			else
			{
				vector<int> f (6, 0);
				temp_map[wrd] = f;
				temp_map[wrd][5] = 1;
			}
		}

		//Outlink : URL
		else if(txt[k] == '[' && txt.length()-k>=6 && txt[k+1] == 'h' && txt[k+2] == 't' && txt[k+3] == 't' && txt[k+4] == 'p' && txt[k+5] == ':')
		{
			k++;
			count = 0;
			while(txt[k] != ']' && txt[k] != ' ')
			{
				word[count++] = txt[k++];
			}
			word[count] = '\0';
			stemfile(word);
			wrd.assign(word);
			if(stop_words.find(wrd) != stop_words.end())
			{
			}
			else if(temp_map.find(word) != temp_map.end())
			{
				temp_map[wrd][3] ++;
			}
			else
			{
				vector<int> f (6, 0);
				temp_map[wrd] = f;
				temp_map[wrd][3] = 1;
			}
			while(txt[k] != ']')
			{
				if(isalpha(txt[k]))
				{
					count = 0;
					while(isalpha(txt[k]))
					{
						word[count++] = txt[k++];
					}
					word[count] = '\0';
					stemfile(word);
					wrd.assign(word);
					if(stop_words.find(wrd) != stop_words.end())
					{
					}
					else if(temp_map.find(wrd) != temp_map.end())
					{
						temp_map[wrd][2] ++;
					}
					else
					{
						vector<int> f (6, 0);
						temp_map[wrd] = f;
						temp_map[wrd][2] = 1;
					}
				}
					else
						k++;
				
			}
		}

		//Category
		else if(txt[k] == '[' && txt[k+1] == '[' && txt.length()-k>=10 && txt[k+2] == 'C' && txt[k+3] == 'a' && txt[k+4] == 't' && txt[k+5] == 'e' && txt[k+6] == 'g' && txt[k+7] == 'o' && txt[k+8] == 'r' && txt[k+9] == 'y')
		{
			k+=10;
			while(!(txt[k] == ']' && txt[k+1] == ']'))
			{
				if(isalpha(txt[k]))
				{

					count = 0;
					while(isalpha(txt[k]))
					{
						word[count++] = txt[k++];
					}
					word[count] = '\0';
					stemfile(word);
					wrd.assign(word);
					if(stop_words.find(wrd) != stop_words.end())
					{
					}
					else if(temp_map.find(wrd) != temp_map.end())
					{
						temp_map[wrd][4] ++;
					}
					else
					{
						vector<int> f (6, 0);
						temp_map[wrd] = f;
						temp_map[wrd][4] = 1;
					}
				}
				else
					k++;
			}
		}

		//Outlink : Internal
		else if(txt[k] == '[' && txt[k+1] == '[')
		{
			k+=2;
			while(!(txt[k] == ']' && txt[k+1] == ']'))
			{
				count = 0;
				if(isalpha(txt[k]))
				{
					count = 0;
					while(isalpha(txt[k]))
					{
						word[count++]= txt[k++];
					}
					word[count] = '\0';
					stemfile(word);
					wrd.assign(word);
					if(stop_words.find(wrd) != stop_words.end())
					{
					}
					else if(temp_map.find(wrd) != temp_map.end())
					{
						temp_map[wrd][3] ++;
					}
					else
					{
						vector<int> f (6, 0);
						temp_map[wrd] = f;
						temp_map[wrd][3] = 1;
					}
				}
				else
					k++;
			}
			k+=2;
		}
		else
			k++;
	}
	for(iter = temp_map.begin(); iter!=temp_map.end(); iter++)
	{
		new_node.docID = page_count;
		new_node.title = (char) (iter->second[0]);
		new_node.infobox = (char) (iter->second[1] + iter->second[2]);
		new_node.link = (char) (iter->second[2] + iter->second[3]);
		new_node.category = (char)(iter->second[4]);
		new_node.text = (short int)(iter->second[5]);
		//new_node.freq[i] = iter->second[i];
		if(inverted_index.find(iter->first)!=inverted_index.end())
		{
			inverted_index[iter->first].push_back(new_node);
		}
		else
		{
			vector<node> temp_vec;
			temp_vec.push_back(new_node);
			inverted_index[iter->first] = temp_vec;
		}
	}

}


int main(int argc, char **argv)
{
	map<string, int>::iterator iter;
	FILE *f = fopen("stop_words.txt", "r");
	s = (char *) malloc(i_max+1);
	char q;
	int pos = 0;
	string txt, wrd;
	char word[1000];
	while(fscanf(f, "%c", &q)!=EOF)
	{
		if((q<='z' && q>='a') || (q<='9' && q>='0'))
		{
			word[pos++] = q;
		}
		else
		{
			word[pos] = '\0';
			wrd.assign(word);
			if(stop_words[wrd] == 0)
			{
				stop_words[wrd] = 1;
			}
			pos = 0;
		}
	}
	fclose(f);

	xml_document<> doc;
	ifstream in;
	in.open(argv[1], ios::binary);
	cout<<argv[1]<<endl;
	in.seekg(0, ios::end);
	int file_length = in.tellg();
	in.seekg(0, ios::beg);
	in.read(buffer, file_length);
	in.close();
	cout<<"Read to file\n";
	doc.parse<0>(&buffer[0]); 
	xml_node<> * root_node;
	root_node = doc.first_node("file");
	int page_count = 0;
	page_count= 0;
	string title, id;
	xml_node<> *y, *z, *j, *did;
	//PARSING TEXT
	for(xml_node<> * i = root_node->first_node("page"); i; i = i->next_sibling("page"))
	{
		page_count++;
		y = i->first_node("title");
		title = y->value();
		char docid[10];
		did = i->first_node("id");
		id = did->value();
		docid[id.size()] = '\0';
		memcpy(docid, id.c_str(), id.size());
		z = i->first_node("revision");
		j = z->first_node("text");
		txt = j->value();
		parse(docid, title, txt, page_count);
	}
	map<string, vector<node> >::iterator it;
	ofstream of;
	of.open(argv[2], ios::out | ios::binary);
	for(it = inverted_index.begin(); it!=inverted_index.end(); it++)
	{
		if(it->first.length() <= 1)
			continue;
		int length = it->first.size();
		of.write((char*)&length, 4);
		//cout<<length<<endl;
		of.write((char*)it->first.c_str(), length);
		//cout<<it->first.c_str()<<endl;
		length = it->second.size();
		of.write((char*)&length, 4);
		//cout<<length<<endl;
		for(int i=0; i<it->second.size(); i++)
		{
		//	cout<<it->second[i].docID<<" "<<it->second[i].text<<" "<<it->second[i].title<<" "<<it->second[i].infobox<<" "<<it->second[i].link<<" "<<it->second[i].category<<endl;
    	of.write((char*)&it->second[i].docID, 4);
			of.write((char*)&it->second[i].text, 2);
			of.write((char*)&it->second[i].title, 1);
			of.write((char*)&it->second[i].infobox, 1);
			of.write((char*)&it->second[i].link, 1);
			of.write((char*)&it->second[i].category, 1);
		}
	}
	of.close();
	return 0;
}
