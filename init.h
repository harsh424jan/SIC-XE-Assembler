#include<bits/stdc++.h>
#define pb push_back
using namespace std;

map<string,int> optab;
map<string,string> hexcode;
ofstream inter;
map<string,int> reg;
ofstream listing;
ofstream obj;
ifstream in;
ifstream raw;
vector<string> modrec;
set<string> directives;
map<string,int> littab;
vector<string> temp_lit;
vector<string> litstaraddress;
map<string,int> symtab;
map<string,pair<int,bool> > equs;
string staaddress;
int litstar=0;
int RegB;
bool base;
bool SetB;
int locctr;
int programlen;
int linecounter;
int prevloc=-1;
set<string> NoOperands;
struct line{
	string comment,label,loc,opcode;
	string operand;
	string blck;
};

line l;
string curb="default";
string prevb;
vector<string> bord;
struct blk{
	int length,sta,number;
};
map<string,blk> blocks;