#include "io.h"
void pass1()
{
	staaddress="";
	blocks["default"].number=0;
	bord.pb("default");
	locctr=0;
	line l=read();
	while(l.opcode=="")
	{
		fprint(l);
		l=read();
	}
	if(l.opcode=="START")
	{
		staaddress=l.operand;
		locctr=hex_int(l.operand);
		l.loc=int_hex(locctr,5);
		fprint(l);
		l=read();
	}
	while(l.opcode!="END"&&(l.opcode!=""||l.comment!=""))
	{
		if(l.opcode!="")
		{
			l.loc=int_hex(locctr,5);
			if(l.opcode=="USE")
			{
				l.loc="";
				if(blocks.find(l.operand)!=blocks.end())
				{
					blocks[curb].length=locctr;
					locctr=blocks[l.operand].length;
					prevb=curb;
					curb=l.operand;

				}
				else if(l.operand==""&&prevb!="")
				{
					string temp=curb;
					blocks[curb].length=locctr;
					locctr=blocks[prevb].length;
					curb=prevb;
					prevb=temp;
				}
				else
				{
					if(l.operand=="")
					{
						cerr<<"ERROR ON LINE"<<linecounter<<": No name specified";
						l.operand=int_hex(rand(),7);
					}
					blocks[l.operand].number=blocks.size()-1;
					bord.pb(l.operand);
					blocks[curb].length=locctr;
					locctr=0;
					prevb=curb;
					curb=l.operand;
				}
			}
			else if(l.opcode=="EQU")
			{
				if(symtab.find(l.label)!=symtab.end()&&equs.find(l.label)!=equs.end())
				{
					cout<<"errorx\n";
				}
				else if(l.operand=="*")
				{
					equs[l.label].first=locctr;
					equs[l.label].second=1;
				}
				else if(str_int(l.operand).first)
				{
					equs[l.label]=pair<int,bool>(str_int(l.operand).second,0);
				}
				else 
				{
					equs[l.label]=expr(l.operand);
				}
			}
			else if(l.opcode=="ORG")
			{
				if(l.operand!="")
				{
					if(str_int(l.operand).first)
					{
						prevloc=locctr;;
						locctr=str_int(l.operand).second;
					}
					else if(symtab.find(l.operand)==symtab.end())
					{
						cerr<<"ERROR ON LINE "<<linecounter<<": Symbol is not defined.\n Location counter is not changed.\n";
					}
					else{
						prevloc=locctr;
						locctr=symtab[l.operand];
					}
				}
				else if(prevloc!=-1)
				{
					int temp=prevloc;
					prevloc=locctr;
					locctr=temp;
				}
				else
				{
					cerr<<"ERROR ON LINE "<<linecounter<<": Symbol isn't specified.\n";
				}

			}
			else if(l.label!="")
			{
				if(symtab.find(l.label)!=symtab.end())
				{
					cout<<"error2\n";
				}
				else
					symtab[l.label]=locctr;
			}
			if(optab.find(l.opcode)!=optab.end())
				locctr+=optab[l.opcode];
			else if(l.opcode=="WORD")
				locctr+=3;
			else if(l.opcode=="RESW")
				locctr+=3*str_int(l.operand).second;
			else if(l.opcode=="RESB")
				locctr+=str_int(l.operand).second;
			else if(l.opcode=="BYTE")
				locctr+=stringlen(l.operand);
			else if(directives.find(l.opcode)!=directives.end())
				l.loc="";
			else
			{
				cout<<l.opcode<<" error3\n";
			}
			if(l.operand.size()>0&&l.operand[0]=='='&&littab.find(l.operand)==littab.end())
			{	
				int i;
				for(i=0;i<temp_lit.size();i++)
					if(temp_lit[i]==l.operand)
						break;
				if(l.operand=="=*")
				{
					litstar++;
					temp_lit.pb(l.operand+'('+int_hex(litstar,1)+')');
					litstaraddress.pb(l.loc);
				}
				else if(i==temp_lit.size())
				temp_lit.pb(l.operand);
			}
			if(l.opcode=="LTORG")
			{
				for(int i=0;i<temp_lit.size();i++)
				{
					fprint(l);
					l.loc=int_hex(locctr,5);
					l.label="*";
					l.opcode=temp_lit[i];
					l.operand="";
					l.comment="";
					littab[temp_lit[i]]=locctr;
					if(temp_lit[i][0]=='='&&temp_lit[i][1]=='*')
					{
						locctr+=3;
						l.operand=*litstaraddress.begin();
						litstaraddress.erase(litstaraddress.begin());
					}
					else
					locctr+=stringlen(temp_lit[i].substr(1));
				}
				temp_lit.clear();
			}

		}
		fprint(l);
		l=read();
	}	
	line end=l;
	for(int i=0;i<temp_lit.size();i++)
	{
		l.loc=int_hex(locctr,5);
		l.label="*";
		l.opcode=temp_lit[i];
		l.operand="";
		l.comment="";
		littab[temp_lit[i]]=locctr;
		if(temp_lit[i][0]=='='&&temp_lit[i][1]=='*')
		{
			locctr+=3;
			l.operand=*litstaraddress.begin();
			litstaraddress.erase(litstaraddress.begin());
		}
		else
			locctr+=stringlen(temp_lit[i].substr(1));
		fprint(l);
	}
	l=end;
	temp_lit.clear();
	if(l.opcode=="END")
	{
		l.loc=int_hex(locctr,5);
		fprint(l);
	}
	programlen=locctr-hex_int(staaddress);
}

void pass2()
{
	linecounter=0;
	litstar=0;
	locctr=hex_int(staaddress);
	l=readIntermediate();
	while(l.opcode=="")
	{
		print_list(l,"");
		l=readIntermediate();
	}
	if(l.opcode=="START")
	{
		print_list(l,"");
		obj<<'H';
		obj.width(6);
		obj<<left<<l.label;
		obj<<int_hex(locctr,6)<<int_hex(programlen,6)<<endl;
		l=readIntermediate();
	}
	string objcode;
	string text="";
	string statext;
	while(l.opcode!="END"&&(l.opcode!=""||l.comment!=""))
	{
		objcode="";
		if(l.opcode!="")
		{	
			if(l.opcode[0]=='=')
			{
				objcode="";
				if(l.opcode[1]=='X')
				{
					objcode=l.opcode.substr(3);
					objcode.pop_back();
				}
				else if(l.opcode[1]=='C')
				{
					string te=l.opcode.substr(3);
					te.pop_back();
					for(int i=0;i<te.size();i++)
						objcode+=int_hex((int)te[i],2);
				}
				else if(l.opcode[1]=='*')
				{
					objcode=l.operand;
				}
			}
			else if(l.opcode=="BASE")
			{
				base=1;
				if(l.operand=="*")
					SetB=1;
				else if(symtab.find(l.operand)!=symtab.end())
					RegB=symtab[l.operand];
				else
				{
					//error
					RegB=0;
				}
			}
			else if(l.opcode=="NOBASE")
				base=0;
			else if(optab.find(l.opcode)!=optab.end())
			{
				if(optab[l.opcode]==1)
				{
					objcode=hexcode[l.opcode];
				}
				else if(optab[l.opcode]==2)
				{
					objcode=hexcode[l.opcode];
					bool single=1;
					string r1,r2;
					getRegister(l.operand,r1,r2);
					if(l.opcode=="SHIFTL"||l.opcode=="SHIFTR")
					{
						if(reg.find(r2)==reg.end())
						{
							//error
						}
						else
							r2=int_hex(reg[r2]-1,1);
						if(reg_check(r1))
							r1=int_hex(reg[r1],1);
					}
					else
					{
						if(reg_check(r1))
							r1=int_hex(reg[r1],1);
						if(reg_check(r2))
							r2=int_hex(reg[r2],1);
					}
					objcode+=r1+r2;
				}
				else if(l.opcode=="RSUB"||l.opcode=="+RSUB")
				{
					objcode=int_hex(hex_int(hexcode[l.opcode])+3,2);
					objcode+=int_hex((l.opcode[0]=='+'),1);
					objcode+=int_hex(0,l.opcode[0]=='+'?5:3);
				}
				else
				{
					int xbpe=0;
					if(optab[l.opcode]==4)
						xbpe=1;
					if(l.operand.substr(l.operand.size()-2)==",X")
						xbpe=8;
					string act="";
					int i=0;
					if(l.operand.size()!=0&&(l.operand[i]=='@'||l.operand[i]=='#'))
						i++;
					for(;i<l.operand.size()&&l.operand[i]!=',';i++)
						act+=l.operand[i];
					objcode=hexcode[l.opcode];
					if(l.operand[0]=='@')
					{
						if(xbpe/2)
						{
							//error
						}
						if(symtab.find(act)==symtab.end())
						{
							//error
						}
						else
						{
							if(xbpe%2==1)
							{
								act=int_hex(symtab[act],5);
								string mod="M"+int_hex(hex_int(l.loc)+1,6)+int_hex(5,2);
								modrec.pb(mod);
							}
							else if(-2048<=symtab[act]-hex_int(l.loc)-optab[l.opcode]&&symtab[act]-hex_int(l.loc)-optab[l.opcode]<2048)
							{
								xbpe+=2;
								act=int_hex(symtab[act]-hex_int(l.loc)-optab[l.opcode],3);
							}
							else if(base&&0<=symtab[act]-RegB&&symtab[act]-RegB<4096)
							{
								xbpe+=4;
								act=int_hex(symtab[act]-RegB,3);
							}
							else 
							{
								//error
							}
							objcode=int_hex(hex_int(objcode)+2,2);
							objcode+=int_hex(xbpe,1);
							objcode+=act;
						}

					}
					else if(l.operand[0]=='#')
					{
						if(xbpe/2)
						{
							//error
						}
						if(str_int(act).first)
						{
							if(xbpe%2)
								act=int_hex(str_int(act).second,5);
							else
								act=int_hex(str_int(act).second,3);
						}
						else if(equs.find(act)!=equs.end())
						{
							if(equs[act].second==0)
							{
								act=int_hex(equs[act].first,(xbpe%2==1?5:3));
							}
							if(xbpe%2==1)
							{
								act=int_hex(equs[act].first,5);
								string mod="M"+int_hex(hex_int(l.loc)+1,6)+int_hex(5,2);
								modrec.pb(mod);
							}
							else if(-2048<=equs[act].first-hex_int(l.loc)-optab[l.opcode]&&equs[act].first-hex_int(l.loc)-optab[l.opcode]<2048)
							{
								xbpe+=2;
								act=int_hex(equs[act].first-hex_int(l.loc)-optab[l.opcode],3);
							}
							else if(base&&0<=equs[act].first-RegB&&equs[act].first-RegB<4096)
							{
								xbpe+=4;
								act=int_hex(equs[act].first-RegB,3);
							}
							else 
							{
								//error
							}
						}
						else if(symtab.find(act)==symtab.end())
						{
							//error
						}
						else
						{
							if(xbpe%2==1)
							{
								act=int_hex(symtab[act],5);
								string mod="M"+int_hex(hex_int(l.loc)+1,6)+int_hex(5,2);
								modrec.pb(mod);
							}
							else if(-2048<=symtab[act]-hex_int(l.loc)-optab[l.opcode]&&symtab[act]-hex_int(l.loc)-optab[l.opcode]<2048)
							{
								xbpe+=2;
								act=int_hex(symtab[act]-hex_int(l.loc)-optab[l.opcode],3);
							}
							else if(base&&0<=symtab[act]-RegB&&symtab[act]-RegB<4096)
							{
								xbpe+=4;
								act=int_hex(symtab[act]-RegB,3);
							}
							else 
							{
								//error
							}
						}
						objcode=int_hex(hex_int(objcode)+1,2);
						objcode+=int_hex(xbpe,1);
						objcode+=act;
					}
					else if(l.operand[0]=='=')
					{
						if(l.operand[1]=='*'){
							litstar++;
							act+='('+int_hex(litstar,1)+')';
						}
						if(xbpe%2==1)
							{
								act=int_hex(littab[act],5);
								string mod="M"+int_hex(hex_int(l.loc)+1,6)+int_hex(5,2);
								modrec.pb(mod);
							}
							else if(-2048<=littab[act]-hex_int(l.loc)-optab[l.opcode]&&littab[act]-hex_int(l.loc)-optab[l.opcode]<2048)
							{
								xbpe+=2;
								act=int_hex(littab[act]-hex_int(l.loc)-optab[l.opcode],3);
							}
							else if(hexcode[l.opcode]!="68"&&base&&0<=littab[act]-RegB&&littab[act]-RegB<4096)
							{
								xbpe+=4;
								act=int_hex(littab[act]-RegB,3);
							}
							else 
							{
								//error
							}
							objcode=int_hex(hex_int(objcode)+3,2);
							objcode+=int_hex(xbpe,1);
							objcode+=act;
					}
					else
					{
						if(symtab.find(act)==symtab.end())
						{
							//error
							cout<<"act"<<endl;
						}
						else
						{
							if(xbpe%2==1)
							{
								act=int_hex(symtab[act],5);
								string mod="M"+int_hex(hex_int(l.loc)+1,6)+int_hex(5,2);
								modrec.pb(mod);
							}
							else if(-2048<=symtab[act]-hex_int(l.loc)-optab[l.opcode]&&symtab[act]-hex_int(l.loc)-optab[l.opcode]<2048)
							{
								xbpe+=2;
								act=int_hex(symtab[act]-hex_int(l.loc)-optab[l.opcode],3);
							}
							else if(base&&0<=symtab[act]-RegB&&symtab[act]-RegB<4096)
							{
								xbpe+=4;
								act=int_hex(symtab[act]-RegB,3);
							}
							else 
							{
								//error
								cout<<act<<"1\n";
							}
							objcode=int_hex(hex_int(objcode)+3,2);
							objcode+=int_hex(xbpe,1);
							objcode+=act;
						}	
					}
				}
			}
			else if(l.opcode=="WORD")
				objcode=int_hex(str_int(l.opcode).second,6);
			else if(l.opcode=="BYTE")
			{
				objcode="";
				if(l.operand[0]=='X')
				{
					objcode=l.operand.substr(2);
					objcode.pop_back();
				}
				else if(l.operand[0]=='C')
				{
					string te=l.operand.substr(2);
					te.pop_back();
					for(int i=0;i<te.size();i++)
						objcode+=int_hex((int)te[i],2);
				}
			}
			else if(l.opcode=="RESB"||l.opcode=="RESW"||l.opcode=="ORG"||l.opcode=="USE")
			{
				if(text.size())
					obj<<"T0"<<statext<<int_hex(text.size(),2)<<text<<endl;
				text="";
				statext=l.loc;
			}
			if(text.size()+objcode.size()>60)
			{
				obj<<"T0"<<statext<<int_hex(text.size(),2)<<text<<endl;
				text="";
			}
			if(text.size()==0)
				statext=l.loc;
			text+=objcode;
		}
		print_list(l,objcode);
		l=readIntermediate();
		if(SetB&&l.loc!="")
		{
			RegB=hex_int(l.loc);
			SetB=0;
		}
	}
	if(text.size())
	obj<<"T0"<<statext<<int_hex(text.size(),2)<<text<<endl;
	print_list(l,"");
	for(int i=0;i<modrec.size();i++)
		obj<<modrec[i]<<endl;
	if(symtab.find(l.operand)==symtab.end())
	{
		//error
	}
	obj<<"E"<<int_hex(symtab[l.operand],6);
}
