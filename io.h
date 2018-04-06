#ifndef io_h 
#define io_h
#include "helper.h"
line read()
{
	linecounter++;
	line x;
	string s;
	getline(raw,s);
	trim(s);
	if(s[0]=='.')
	{
		x.comment = s;
		return x;
	}
	int i=0;
	string temp=getword(s,i);
	if(optab.find(temp)==optab.end()&&directives.find(temp)==directives.end())
	{	
		x.label=temp;
		i++;
		temp=getword(s,i);
	}
	x.opcode=temp;
	if(optab.find(temp)==optab.end()&&directives.find(temp)==directives.end())
	{
		cerr<<"ERROR ON LINE"<<linecounter<<" No such operation exist for SIC/XE."<<endl;
		return x;
	}
	i++;
	if(NoOperands.find(temp)==NoOperands.end()){
		temp=getword(s,i);
		x.operand=temp;
		i++;
	}
	while(i<s.size())
		x.comment+=s[i++];
	return x;
}
void fprint(line l)
{
	if(l.opcode=="")
	{
		inter<<"        "<<l.comment<<endl;
		return;
	}
	inter.width(6);
	inter<<left<<l.loc;
	if(l.loc=="")
		inter<<"  ";
	else
		inter<<blocks[curb].number<<' ';
	inter.width(8);
	inter<<left<<l.label;
	inter.width(6);
	inter<<left<<l.opcode;
	inter.width(20);
	inter<<left<<l.operand;
	inter<<left<<l.comment<<endl;
}
line readIntermediate()
{
	linecounter++;
	line l;
	string s;
	getline(in,s);
	l.loc=s.substr(0,5);
	if(l.loc!="")
		l.blck=s[6];
	if(s[8]=='.')
	{
		l.comment=s.substr(8);
		return l;
	}
	l.label=s.substr(8,8);
	trim(l.label);
	l.opcode=s.substr(16,6);
	trim(l.opcode);
	bool in=0;
	for(int i=0;i<l.opcode.size();i++)
	if(l.opcode[i]=='\'')
	{
		if(in)
		{
			in=0;
			break;
		}
		else
			in=1;
	}
	int j=21;
	if(in)
		while(j+1<s.size()&&s[j]!='\'')
			l.opcode+=s[++j];
	j++;
	l.operand=s.substr(j,42-j);
	trim(l.operand);
	l.comment=s.substr(42);
	return l;
}
void print_list(line l,string s)
{
	if(l.opcode=="")
	{
		listing<<"      "<<l.comment<<endl;
		return;
	}
	listing.width(6);
	listing<<left<<l.loc;
	listing.width(2);
	listing<<left<<l.blck;
	listing.width(8);
	listing<<left<<l.label;
	listing.width(6);
	listing<<left<<l.opcode;
	if(l.opcode.size()>6){
		listing<<' ';
		listing.width(25-l.opcode.size());
	}
	else
	listing.width(20);
	listing<<left<<l.operand;
	listing.width(9);
	listing<<left<<s;
	listing<<l.comment<<endl;
}
#endif