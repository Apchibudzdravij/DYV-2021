#include "gen.h"
#include "Parm.h"
#include "LitT.h"
#include "IT.h"
#include "LT.h"
#include "Error.h"
#include "regex"
#include <vector>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <stack>
#include <cctype>

#include <algorithm>

using namespace std;

/*std::string printEverything(std::string st, IT::PropTable& idtable, int ind)
{
	std::string str = "";
		for (int j = 0; j < idtable.propertycounter - START_COUNTER-1; ++j)
		{
			std::string uvw1 = std::string(idtable.name);
			for (int i = 0; i < uvw1.length(); ++i)
				if (uvw1[i] == ' ')
					uvw1[i] = '_';
			std::string uvw2 = std::string(idtable.table[j].name);
			for (int i = 0; i < uvw2.length(); ++i)
				if (uvw2[i] == ' ')
					uvw2[i] = '_';
			str += st + uvw1 + "_" + uvw2 + " dword ?\n";
			str += st + uvw1 + "_" + uvw2 + "_integer dword ?\n";
			str += st + uvw1 + "_" + uvw2 + "_string dword ?\n";
			str += st + uvw1 + "_" + uvw2 + "_doublenum dword ?\n";
			str += st + uvw1 + "_" + uvw2 + "_hexinteger dword ?\n";
			st += "_" + uvw1;
			if (idtable.table[j].propertycounter > START_COUNTER)
				str += st + printEverything(st, idtable.table[j], j);
		}
	return str;
}*/
/*static int statenum = 0;
std::string itoS(int x) { stringstream r;  r << x;  return r.str(); }
std::string genStateCode(LT::LexTable& lextable, IT::IdTable& idtable, LitT::LitTable littable, LEX::LexList lexlist, int i, std::string& cyclecode)
{
	std::string str;
	statenum++;
	cyclecode.clear();
	IT::PropTable lft = ITENTRY(i + 1); //
	IT::PropTable rgt = ITENTRY(i + 3); //
	bool w = false, r = false, c = false;
	string wstr, rstr;

	for (int j = i + 5; LEXEMA(j) != LEX_LOGSEP; j++) //
	{
		if (LEXEMA(j) == LEX_TSTATE) r = true;
		if (LEXEMA(j) == LEX_FSTATE) w = true;
		if (LEXEMA(j) == LEX_REPEAT) c = true;
	}
	str = str + "mov edx, " + lft.name + "\ncmp edx, " + rgt.name + "\n";

	if (!c && r) str = str + "\n" + rstr + " right" + itoS(statenum);
	if (!c && w) str = str + "\n" + wstr + " wrong" + itoS(statenum);
	if (c)
	{
		str = str + "\n" + rstr + " repeat" + itoS(statenum);
		cyclecode = str;
		str = str + "\njmp repeatnext" + itoS(statenum);
	}
	else if (!r || !w)  str = str + "\njmp next" + itoS(statenum);
	return str;
}*/
std::string startFillVector(LT::LexTable& smoothtable, LT::LexTable& lextable, IT::IdTable& idtable, LitT::LitTable littable, LEX::LexList lexlist)
{
	std::string v = "";
	v += BEGIN;				//
	v += "\n\n";
	v += EXTERN;

	std::string vlt = ""; vlt += CONST;
	std::string vid = ""; vid += DATA;

	for (int i = 0; i < littable.size; ++i)				// 
	{
		LitT::Entry e = littable.table[i];
		std::string buff = ""; buff += e.literal;
		buff.erase(remove(buff.begin(), buff.end(), ' '), buff.end());
		buff.erase(remove(buff.begin(), buff.end(), '!'), buff.end());
		buff.erase(remove(buff.begin(), buff.end(), ','), buff.end());
		for (int i = 0; i < buff.length(); ++i)
			if (buff[i] == '.')
				buff[i] = '_';
		std::string str = "\n\t_" + buff;
		if (std::regex_match(e.literal, e.literal+buff.length(), std::regex("[0-9]*")))
			str = "\t" + str + " dword " + std::string(e.literal);
		else
			str = "\t" + str + " byte '" + std::string(e.literal) + "', 0";
		vlt += str;
	}
	for (int i = 0; i < smoothtable.size; ++i)
	{
		std::string no_ = "";
		no_ += std::string(smoothtable.table[i].name);
		for (int j = 0; j < no_.length(); ++j)
			if (no_[j] == '_')
				no_[j] = ' ';
		std::string str = "\n\t" + std::string(smoothtable.table[i].name);
		str += "name_ byte '" + no_ + "', 0";
		size_t pos = vlt.find(str);
		if (pos == std::string::npos)
			vlt += str;
	}
	for (int i = 0; i < smoothtable.size; ++i)
	{
		/*bool oh = false;
		std::string str = "";
		std::string s = std::string(idtable.table[i].name);
		if (!std::regex_match(idtable.table[i].name, idtable.table[i].name + s.length(), std::regex("[0-9]+.[0-9]+")))
		{
			std::string uvw = std::string(idtable.table[i].name);
			for (int i = 0; i < uvw.length(); ++i)
				if (uvw[i] == ' ')
					uvw[i] = '_';
			str += "\t_" + uvw + " dword ?\n";
			str += "\t_" + uvw + "_integer dword ?\n";
			str += "\t_" + uvw + "_string dword ?\n";
			str += "\t_" + uvw + "_doublenum real8 ?\n";
			str += "\t_" + uvw + "_hexinteger dword ?\n";
			if (idtable.table[i].propertycounter > START_COUNTER)
				str += printEverything("\t", idtable.table[i], i);
			vid += str;
		}*/
		std::string str = "";
		std::string no_ = "";
		no_ += std::string(smoothtable.table[i].name);
		for (int i = 0; i < no_.length(); ++i)
			if (no_[i] == '_')
				no_[i] = ' ';
		no_.erase(remove(no_.begin(), no_.end(), ' '), no_.end());
		char* buffchar = new char[no_.length()];
		for (int i = 0; i < no_.length(); ++i)
		{
			buffchar[i] = no_[i];
		}
		if (std::regex_match(buffchar, buffchar + no_.length(), std::regex("0x[0-9]+")))
		{
			no_.erase(remove(no_.begin(), no_.end(), 'x'), no_.end());
			no_ += "h";
		}
		str += "\t" + std::string(smoothtable.table[i].name) + "integer_ dword " + no_ + "\n";
		str += "\t" + std::string(smoothtable.table[i].name) + "string_ byte '" + no_ + "', 0\n";
		str += "\t" + std::string(smoothtable.table[i].name) + "hexinteger_ byte '" + no_ + "', 0\n";
		size_t pos = vid.find(str);
		if (pos == std::string::npos)
			vid += str;
	}
	v += vlt;
	v += '\n';
	v += vid;
	v += '\n';
	v += CODE;
	return v;
}

std::string genEqualCode(LT::LexTable& smothtable, LT::LexTable& lextable, IT::IdTable& idtable, LitT::LitTable littable, LEX::LexList lexlist)
{
	string str = "";
	//IT::PropTable e1 = ITENTRY(i - 1); //
	int smoothcounter = 0;
	char loopc = 'a';
	int lupi = 1;
	bool colon1 = true;
	char ifc = 'a';
	int ifi = 1;
	//stack<string> stk;
	int i = 0;
	int k = i;
	for (; i < lextable.size; ++i)
	{
		switch (lextable.table[i].lexema[0])
		{
		/*case LEX_OBJECT:
		{
			str += "\tpush " + std::string(smothtable.table[smoothcounter].name) + "\n";
			++smoothcounter;
			//stk.push(idtable.table[smothtable.table[smoothcounter - 1].idxProp-2].name);
			break;
		}*/
		case LEX_IS:
		{
			std::string integer = "integer";
			int result_position = i - 1;
			++smoothcounter;
			int litcounter = 0; //каждый литерал в тексте - специфичный
			
			while ((lextable.table[i].lexema[0] != LEX_SEMICOLON)&&(i < lextable.size))
			{

				switch (LEXEMA(i))
				{
					//case LEX_STDFUNC:
				case LEX_LITERAL:
				{
					if (littable.table[litcounter++].type == LitT::Entry::INT)
					{
						str += "\tpush _" + std::string(/*smothtable.table[smoothcounter].name)*/LitT::GetEntry(littable, lextable.table[i].sn, lextable.table[i].pn).literal) + "\n";
						//++smoothcounter;
						//stk.push(idtable.table[smothtable.table[smoothcounter-1].idxProp].name);
						break;
					}
					else
					{
						str += "\tpush offset _" + std::string(LitT::GetEntry(littable, lextable.table[i].sn, lextable.table[i].pn).literal) + "\n";
						//++smoothcounter;
						//stk.push("offset" + (string)idtable.table[smothtable.table[smoothcounter - 1].idxProp].name);
						break;
					}
				}
				case LEX_OBJECT:
				{

					str += "\tpush " + std::string(smothtable.table[LEX::FindInSmoothed(smothtable, lextable.table[i].name)].name) + "integer_\n";
					//stk.push(idtable.table[smothtable.table[smoothcounter - 1].idxProp-2].name);
					++smoothcounter;
					break;
				}
				/*case LEX_NOT_DOUBLE_QUOTE:
				{
					ofile << "\tmov eax, 0\n\tpop ebx\n";
					ofile << "\tsub eax, ebx\n\tpush eax\n";
					break;
				}*/
				case LEX_STAR:
				{
					str += "\tpop eax\n\tpop ebx\n";
					str += "\timul ebx\n\tpush eax\n";
					break;
				}
				case LEX_PLUS:
				{
					str += "\tpop eax\n\tpop ebx\n";
					str += "\tadd eax, ebx\n\tpush eax\n";
					break;
				}
				case LEX_MINUS:
				{
					if (LEXEMA(i - 2) == LEX_IS)
					{
						str += "\tmov eax, 0\n\tpop ebx\n";
						str += "\tsub eax, ebx\n\tpush eax\n";
						break;
					}
					str += "\tpop ebx\n\tpop eax\n";
					str += "\tsub eax, ebx\n\tpush eax\n";
					break;
				}
				case LEX_DIRSLASH:
				{
					str += "\tpop ebx\n\tpop eax\n";
					str += "\tcmp ebx,0\n";
					str += "\tcdq\n";
					str += "\tidiv ebx\n\tpush eax\n";
					break;
				}
				case LEX_MODULO:
				{

					str += "\tpop ebx\n\tpop eax\n";
					str += "\tcmp ebx,0\n";
					str += "\tcdq\n";
					str += "\tidiv ebx\n\tpush edx\n";
					break;
				}
				}
				++i;
				if (lextable.table[i].lexema[0] == LEX_SEMICOLON)
				{
					if (lextable.table[i-1].lexema[0] != LEX_LITERAL)
					str += "\tpop " + std::string(smothtable.table[LEX::FindInSmoothed(smothtable, lextable.table[k].name)].name) + "integer_\n";
					k = i + 1;
					str += "\tpush " + std::string(smothtable.table[LEX::FindInSmoothed(smothtable, lextable.table[k].name)].name) + "integer_\n";
					str += "\tcall _ConvertToChar\n";
					str += "\tmov offset " + std::string(smothtable.table[LEX::FindInSmoothed(smothtable, lextable.table[k].name)].name) + "string_, eax\n";
					str += "\tmov offset " + std::string(smothtable.table[LEX::FindInSmoothed(smothtable, lextable.table[k].name)].name) + "hexinteger_, eax\n";
					break;
				}
			}
			//++smoothcounter;
			str += "\n";
			break;
		}
		case LEX_PRINT: // перевод строки 
		{
			size_t pos = std::string(smothtable.table[smoothcounter].name).find("integer");
			if (pos != std::string::npos)
			{
				str += "\npush " + std::string(smothtable.table[smoothcounter].name) + "\ncall outnumline\n";
			}
			else
			{
					str += "\npush offset " + std::string(smothtable.table[smoothcounter].name) + "string_\ncall outstrline\n";
			}
			break;
		}
		case LEX_LOOP:
		{
			if (std::string(lextable.table[i+1].name) == "start")
			{
				str += "\nlup"; str += loopc; str += ":\n";
				loopc++;
			}
			else if (std::string(lextable.table[i+1].name) == "return")
			{
				str += "\njmp lup"; str+= loopc-lupi; str += "\n";
			}
			break;
		}
		case LEX_CHECK:
		{
			str += "\tpop eax\n\tpop ebx\n";
			str += "\ttest eax, ebx\n";
			break;
		}
		case LEX_EQUALS:
		{
			str += "\tjne "; str += ifc; str += "if\n";
			break;
		}
		case LEX_COLON:
		{
			if (colon1)
			{
				str += "\tjmp "; str += (ifc + ifi);str += "if\n";
				str += "\n"; str+= ifc; str += "if:\n";
				colon1 = false;
			}
			else if (!colon1)
			{
				str += (ifc + ifi); str += "if:\n";
				colon1 = true;
			}
			break;
		}
		default:
			break;
		}
	}
	/*switch (e1.iddatatype)
	{
	case IT::IDDATATYPE::INT:
	{
		bool first = true;
		for (int j = i + 1; LEXEMA(j) != LEX_SEMICOLON; j++)
		{
			switch (LEXEMA(j))
			{
			case LEX_LITERAL:
			case LEX_OBJECT: case LEX_PROPERTY:
			{
				str = str + "push " + ITENTRY(j).name + "\n";
				break;
			}
			case LEX_PLUS:
				str = str + "pop ebx\npop eax\nadd eax, ebx\npush eax\n"; break;
			case LEX_MINUS:
				str = str + "pop ebx\npop eax\nsub eax, ebx\npush eax\n"; break;
			case LEX_STAR:
				str = str + "pop ebx\npop eax\nimul eax, ebx\npush eax\n"; break;
			case LEX_DIRSLASH:
				str = str + "pop ebx\npop eax\ncdq\nidiv ebx\npush eax\n"; break;
			case LEX_MODULO:
				str = str + "pop ebx \npop eax\ncdq\nidiv ebx\npush edx\n"; break;
			}
		} //

		str = str + "\npop ebx\nmov " + e1.name + ", ebx\n";			// 
		break;
	}
	
	}*/

	return str;
}
namespace Gen
{
	void CodeGeneration(LT::LexTable& smoothtable, LT::LexTable& lextable, IT::IdTable& idtable, LitT::LitTable& littable, LEX::LexList& lexlist)
	{
		std::ofstream stream;//заношу в лог таблицы лексем номер первой строки
		remove("project.asm");
		stream.open("project.asm", std::ios_base::out);
		if (!stream.is_open())
		{
			ERROR_THROW(6)
		}
		stream << startFillVector(smoothtable, lextable, idtable, littable, lexlist);
		stream << genEqualCode(smoothtable, lextable, idtable, littable, lexlist);
		/*string funcname;	//
		string cyclecode;	//
		int pcount;			//
		int pos, parcol;
		string str;
		for (int i = 0; i < lextable.size; i++)
		{
			switch (LEXEMA(i))
			{
			case LEX_INTERROGATION: //
			{
				str = genStateCode(lextable, idtable, littable, lexlist, i, cyclecode);
				break;
			}
			case LEX_PRINT: //
			{
				IT::PropTable e = ITENTRY(i + 1);
				{
					str = str + "\npush offset " + e.name + "\ncall outstr\n";
				}
				str = str + "\n";
				break;
			}
			case LEX_LOOP: //
			{
				str = str + "repeat" + itoS(statenum) + ":";
				break;
			}
			}
			if (!str.empty())
				v.push_back(str);
			str.clear();
		}
		v.push_back(END);
		for (auto x : v)
			ofile << x << endl;*/
		stream << END;
		stream.close();
	}
}