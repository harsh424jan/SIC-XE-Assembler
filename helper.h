#ifndef helper_H
#define helper_H
#include "init.h"
void trim(string &s);
int hex_int(string s)
{
	int temp=0;
	for(unsigned int i=0;i<s.size();i++)
	{
		temp*=16;
		if(s[i]-'0'>=0&&s[i]-'0'<10)
temp+=(s[i]-'0');
		else if(s[i]-'A'>=0&&s[i]-'A'<6)
temp+=(s[i]-'A'+10);
		else
		{
cout<<"error1\n";
		}
	}
	return temp;
}
string int_hex(int x,int len)
{
	if(x<0)
	{
		string s="";
		for(int i=0;i<len;i++)
s+='F';
		return int_hex(hex_int(s)+x+1,len);
	}
	string temp="";
	int r;
	while(x)
	{
		r=x%16;
		x/=16;
		if(r<10)
temp=char('0'+r)+temp;
		else
temp=char('A'+r-10)+temp;
	}
	if(temp.size()>len)
	{
		/*error*/
	}
	for(int i=temp.size();i<len;i++)
	temp="0"+temp;
	return temp;
}
pair<bool,int> str_int(string s)
{
	int a=0;
	for(int i=0;i<s.size();i++)
	{
		a*=10;
		if(-1<s[i]-'0'&&s[i]-'0'<10)
a+=(s[i]-'0');
		else
return pair<bool,int>(0,0);
	}
	return pair<bool,int>(1,a);
}
bool reg_check(string r){
	if(reg.find(r)==reg.end()||reg[r]==7||reg[r]>9)
	{
		return 0;
	}
	return 1;
}
void getRegister(string s,string &r1,string &r2)
{
	r1="";
	r2="";
	int i;
	for(i=0;i<s.size();i++)
	{
		if(s[i]!=',')
r1+=s[i];
		else
break;
	}
	if(i+1==s.size()){
		/*error*/
	}
	for(i++;i<s.size();i++)
		r2+=s[i];
	if(r2=="")
		r2="0";
}
string scn(string s,int i,bool b)
{
	int d;
	if(b==0)
		d=-1;
	else
		d=1;
	string temp="";
	for(i+=d;i<s.size()&&i>-1;i+=d)
	{
		if(s[i]=='/'||s[i]=='*'||s[i]=='-'||s[i]=='+')
			return temp;
		temp+=s[i];
	}
	return temp;
}
pair<int,bool> eval(string s)
{
	bool r;
	for(int i=0;i<s.size();i++)
		if(s[i]=='+'||s[i]=='-')
		{
			string a=s.substr(0,i);
			string b=scn(s,i,1);
		return pair<int,bool>(equs[a].first-symtab[b],0);
		}
	return pair<int,bool>(4096,0);
}
pair<int,bool> expr(string s)
{
	for(int i=0;i<s.size();i++)
		if(s[i]=='-'||s[i]=='+'||s[i]=='/'||s[i]=='*')
		return eval(s);
	if(symtab.find(s)!=symtab.end())
		return pair<int,bool> (symtab[s],1);
	else if(equs.find(s)!=equs.end())
		return equs[s];
	else
	{
		cerr<<"FATAL ERROR ON LINE "<<linecounter<<": Following operand is not defined "<<s<<endl;
		exit(1);
	}
}
string getword(string s,int &i)
{
	string temp="";
	bool in=0;
	while(i<s.size()&&s[i]!=' ')
		temp+=s[i++];	
	for(int j=0;j<temp.size();j++)
	if(temp[j]=='\'')
	{
		if(in)
return temp;
		else
in=1;
	}
	if(in==0)
		return temp;
	while(i<s.size()&&s[i]!='\'')
		temp+=s[i++];
	if(i<s.size())
		temp+=s[i++];
	return temp;
}
void trim (string &s){
	bool space=1;
	string temp="";
	bool inside=0;
	for(unsigned int i=0;i<s.size();i++)
	{
		if(s[i]=='\'')
inside=!inside;
		if(inside)
temp+=s[i];
		else if(s[i]!=' '&&s[i]!='\t')
		{
space=0;
temp+=s[i];
		}
		else if(space==0)
		{
space=1;
temp+=" ";
		}
	}
	if(temp.size()>0&&temp[temp.size()-1]==' ')
		temp.pop_back();
	s=temp;
}
int stringlen(string s)
{
	int a=0;
	if(s[0]=='X')
	{
		a=s.size()-3;
		if(a%2)
		{
/*error*/
		}
		else
return a/2;
	}
	else if(s[0]=='C')
		return s.size()-3;
	else
	{
		/*error*/
	}
}
void initialiseOpTable()
{
	ifstream file;
	file.open("OPTAB.txt");
	if(!file)
	{
		cerr<<"File unable to open";
		exit(1);
	}
	string a,s;
	int v;
	while(file>>s)
	{
		file>>v;
		file>>a;
		if(v==3)
		{
optab["+"+s]=4;
hexcode["+"+s]=a;
		}
		if(v==1)
NoOperands.insert(s);
		optab[s]=v;
		hexcode[s]=a;
	}
	directives.insert("BASE");
	directives.insert("NOBASE");
	directives.insert("RESW");
	directives.insert("RESB");
	directives.insert("WORD");
	directives.insert("BYTE");
	directives.insert("END");
	directives.insert("START");
	directives.insert("LTORG");
	directives.insert("EQU");
	directives.insert("ORG");
	directives.insert("USE");
	NoOperands.insert("LTORG");
	NoOperands.insert("RSUB");
	NoOperands.insert("+RSUB");
	file.close();
	reg["A"]=reg["0"]=0;
	reg["X"]=reg["1"]=1;
	reg["L"]=reg["2"]=2;
	reg["B"]=reg["3"]=3;
	reg["S"]=reg["4"]=4;
	reg["T"]=reg["5"]=5;
	reg["F"]=reg["6"]=6;
	reg["7"]=7;
	reg["PC"]=reg["8"]=8;
	reg["9"]=9;
	reg["10"]=10;
	reg["11"]=11;
	reg["12"]=12;
	reg["13"]=13;
	reg["14"]=14;
	reg["15"]=15;
	reg["16"]=16;
}
#endif