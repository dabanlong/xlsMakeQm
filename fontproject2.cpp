#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;


int main(int argc, char* argv[])
{

for(int i=0;i<argc;i++)
{
	cout<<string(argv[i])<<" ";
	//printf("%s ",argv[i]);
}
cout<<endl;

char codestring[80];
int unicodenum;

string cmd;

ifstream unicode_add("unicode_add.txt");
ofstream unicode_del("unicode_del_range.txt");
int code_idx = 0;//0 to 65535
int unicodenum_prev = -1;

unicode_del<<"0"<<endl;

while(unicode_add.good())
{
	unicode_add.getline(codestring,80);
	if(strlen(codestring)==0)break;
	unicodenum = atoi(codestring);
	if(unicodenum==0)continue;
	
	printf("unicode_num?%d\n",unicodenum);

	if(unicodenum_prev==-1)
	{
		unicodenum_prev = unicodenum;
		unicode_del<<(unicodenum-1)<<endl;
		continue;
	}

	if(unicodenum==unicodenum_prev+1)
	{
		unicodenum_prev = unicodenum;
		continue;
	}
	else
	{
		printf("unicode_num??%d\n",unicodenum);
		int start_idx = unicodenum_prev+1;
		int end_idx = unicodenum-1;
		unicode_del<<start_idx<<endl;
		unicode_del<<end_idx<<endl;
		unicodenum_prev = unicodenum;
	}

}


if(unicodenum_prev<65535)
{
	unicode_del<<unicodenum_prev+1<<endl;
	unicode_del<<"65535"<<endl;
}


unicode_add.close();
unicode_del.close();

ifstream unicode_del2("unicode_del_range.txt");
char startstring[80];
char endstring[80];
while(unicode_del2.good())
{
	//assume pairwise input in unicode_del_range.txt
	unicode_del2.getline(startstring,80);
	if(!unicode_del2.good())break;
	unicode_del2.getline(endstring,80);

	cmd = "fontforge -script script_range1020.pe ";
	cmd += startstring;
	cmd += " ";
	cmd += endstring;

	if(strlen(startstring)==0||strlen(endstring)==0)continue;

	printf("cmd: %s\n",cmd.c_str());
	int exit = system(cmd.c_str());

	printf("exit state: %d, start: %s, end: %s\n\n",exit,startstring,endstring);

}

return 0;

}
