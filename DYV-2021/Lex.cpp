#include "Lex.h"
#include "Error.h"
#include "FST.h"
#include "stdafx.h"
#include <fstream>
#include <string>

namespace LEX
{
	LexList Create(int size, LT::LexTable& lt)
	{
		LexList lextable;
		lextable.ltable = lt;
		lextable.maxsize = size;
		lextable.size = 0;
		return lextable;
	}

	void Add(LexList& lextable, LexEntry entry)
	{
		lextable.table[lextable.size] = entry;
		++lextable.size;
		return;
	}

	void Delete(LexList& lextable)
	{
		delete[] lextable.table;
		return;
	}

	void CreateFromFile(LEX::LexList& lexlist, LT::LexTable& lextable, IT::IdTable& idtable, LitT::LitTable& littable)
	{
		setlocale(LC_ALL, ".1251");

		std::ifstream input;
		input.open("TEMP.txt", std::ios::in);//входной файл - результат работы in.cpp
		if (!input)
		{
			ERROR_THROW(101);
		}
		int elementLeng = 0;
		std::string element;

		int line_ok = 1;//счётчик строк для ТЛ
		std::ofstream stream;//заношу в лог таблицы лексем номер первой строки
		remove("LT.log");
		stream.open("LT.log", std::ios_base::app);
		if (!stream.is_open())
		{
			ERROR_THROW(5)
		}
		stream << "000 ";
		stream.close();

		int strNumber = 0;
		int posNumber = 0;
		int lexInLine = 0;
		char lexs;
		char lexc;//буфферы для хранения символа лексемы
		int lexpos = 0;
		int objpos = 0;
		bool islit = false;
		bool integerWas = 0;//буфферы для хранения типа последующей переменной
		bool stringWas = 0;
		bool isobj = true;
		bool loopNotFinished = false;
		//bool unСheckedRequest = true;

		char ch = input.get();
		while (!input.eof())
		{
			if (ch == '\v')
			{
				++strNumber;
				posNumber = 0;
				stream.open("LT.log", std::ios_base::app);
				stream.put('\n');
				if (line_ok < 10)
					stream << "00" << line_ok << " ";
				else if (line_ok < 100)
					stream << "0" << line_ok << " ";
				else
					stream << line_ok << " ";
				stream.close();
				++line_ok;
				lexInLine = 0;
			}
			else
			{
				if ((((ch == LEX_DOT) && (!islit)) || (ch == LEX_IS) || (ch == LEX_PLUS) || (ch == LEX_MINUS) || (ch == LEX_STAR) || (ch == LEX_DIRSLASH) || (ch == LEX_MODULO) || (ch == LEX_COMMA) || (ch == LEX_COLON) || (ch == LEX_SEMICOLON) || (ch == LEX_INTERROGATION)))
				{
					char* buffWord = new char[element.length() + 1];
					for (int i = 0; i < element.length() + 1; ++i)
						buffWord[i] = element[i];
					lexs = FST::called(buffWord);
					if (ch != LEX_COLON)
					{
						if (lexs == NULL)
						{
							if (element[0] == '"')
							{
								if (elementLeng > LIT_MAXLEN)
								{
									ERROR_THROW_IN(200, strNumber, posNumber);
								}
								if (buffWord[elementLeng - 1] != '"')
									ERROR_THROW_IN(215, strNumber, posNumber);
								LitT::Entry literalEntry;
								int i = 1;
								for (; buffWord[i] != '\"'; ++i)//не важно повторяется или нет - литерал для каждой позии новый
									literalEntry.literal[i - 1] = buffWord[i];
								literalEntry.literal[i - 1] = '\0';
								literalEntry.actualSize = elementLeng;
								literalEntry.strNumInLT = strNumber;
								literalEntry.posNumInLT = lexInLine;
								if (std::regex_match(literalEntry.literal, literalEntry.literal + elementLeng-2, std::regex("0x[a-f|0-9]+")))
									literalEntry.type = LitT::Entry::HEX;
								else if (std::regex_match(literalEntry.literal, literalEntry.literal + elementLeng-2, std::regex("[0-9]+")))
									literalEntry.type = LitT::Entry::INT;
								else if (std::regex_match(literalEntry.literal, literalEntry.literal + elementLeng-2, std::regex("[0-9]+.[0-9]+")))
									literalEntry.type = LitT::Entry::DOU;
								else
									literalEntry.type = LitT::Entry::STR;
								LitT::Add(littable, literalEntry);

								LT::Entry lexEntry;
								lexEntry.lexema[0] = LEX_LITERAL;
								lexEntry.sn = strNumber;
								lexEntry.pn = lexInLine;
								lexEntry.idxProp = TI_NULLIDX;
								LT::Add(lextable, lexEntry);
								IT::PropTable* newprop = new IT::PropTable(literalEntry.literal, elementLeng-2);
								IT::Add(idtable, *newprop);
								stream.open("LT.log", std::ios_base::app);
								stream.put(LEX_LITERAL);
								stream.close();
								LEX::LexEntry lexx;
								lexx.entry = &lextable.table[lexpos];
								lexx.propt = NULL;
								LEX::Add(lexlist, lexx);
								++lexpos;
								++lexInLine;
								element.clear();
								elementLeng = 0;
							}
							else if (isobj)
							{
								if (element[0] == '\0')
								{
									LT::Entry lexEntry;
									lexEntry.lexema[0] = LEX_GLOBAL;
									lexEntry.sn = strNumber;
									lexEntry.pn = posNumber;
									lexEntry.idxProp = TI_NULLIDX;
									lexEntry.priority = -1;
									LT::Add(lextable, lexEntry);
									char bchar[2] = "_";
									IT::PropTable* newprop = new IT::PropTable(bchar, 2);
									IT::Add(idtable, *newprop);
									stream.open("LT.log", std::ios_base::app);
									stream.put(LEX_GLOBAL);
									stream.close();
									LEX::LexEntry lexx;
									lexx.entry = &lextable.table[lexpos];
									lexx.propt = NULL;
									LEX::Add(lexlist, lexx);
									++lexpos;
									++lexInLine;
									element.clear();
								}
								else
								{
									if (elementLeng > LIT_MAXLEN)
									{
										ERROR_THROW_IN(201, strNumber, posNumber);
									}

									//if (IT::IsObj(&idtable, buffWord, elementLeng) == TI_NULLIDX)
									//{
									IT::PropTable* newprop = new IT::PropTable(buffWord, elementLeng);
									IT::Add(idtable, *newprop);
									//}
									LT::Entry lexEntry;
									lexEntry.lexema[0] = LEX_OBJECT;
									lexEntry.sn = strNumber;
									lexEntry.pn = posNumber;
									lexEntry.idxProp = IT::IsObj(&idtable, buffWord, elementLeng);
									lexEntry.priority = -1;
									if (elementLeng > 8)
									{
										for (int i = 0; i < 8; ++i)
											lexEntry.name[i] = buffWord[i];
									}
									else
									{
										int i = 0;
										for (; i < elementLeng; ++i)
											lexEntry.name[i] = buffWord[i];
										if (i < 8)
											lexEntry.name[i] = '\0';
									}
									++objpos;
									LT::Add(lextable, lexEntry);
									stream.open("LT.log", std::ios_base::app);
									stream.put(LEX_OBJECT);
									stream.close();
									LEX::LexEntry lexx;
									lexx.entry = &lextable.table[lexpos];
									lexx.propt = &idtable.table[IT::IsObj(&idtable, buffWord, elementLeng)];
									LEX::Add(lexlist, lexx);
									++lexpos;
									++lexInLine;
									element.clear();
									elementLeng = 0;
								}
							}
							else
							{
								if (elementLeng > LIT_MAXLEN)
								{
									ERROR_THROW_IN(200, strNumber, posNumber);
								}
								/*if ((lextable.table[lexpos - 1].lexema[0] != LEX_GLOBAL) && (IT::IsProp(idtable.table[lexpos - 1], buffWord, elementLeng) == TI_NULLIDX))
								{
									IT::PropTable* newprop = new IT::PropTable(buffWord, elementLeng);
									IT::Add(idtable.table[lexpos-1], *newprop);
								}*/
								IT::PropTable* newprop = new IT::PropTable(buffWord, elementLeng);
								IT::Add(idtable, *newprop);
								LT::Entry lexEntry;
								lexEntry.lexema[0] = LEX_PROPERTY;
								lexEntry.sn = strNumber;
								lexEntry.pn = posNumber;
								lexEntry.idxProp = NULL;
								lexEntry.priority = -1;
								if (elementLeng >= 8)
								{
									for (int i = 0; i < 8; ++i)
										lexEntry.name[i] = buffWord[i];
								}
								else
								{
									int i = 0;
									for (; i < elementLeng; ++i)
										lexEntry.name[i] = buffWord[i];
									if (i < 8)
										lexEntry.name[i] = '\0';
								}
								LT::Add(lextable, lexEntry);
								stream.open("LT.log", std::ios_base::app);
								stream.put(LEX_PROPERTY);
								stream.close();
								LEX::LexEntry lexx;
								lexx.entry = &lextable.table[lexpos - 1];
								lexx.propt = NULL;
								LEX::Add(lexlist, lexx);
								++lexpos;
								++lexInLine;
								element.clear();
								elementLeng = 0;
							}
						}
						else
						{
							LT::Entry lexEntry;
							lexEntry.lexema[0] = lexs;
							lexEntry.sn = strNumber;
							lexEntry.pn = posNumber;
							lexEntry.idxProp = TI_NULLIDX;
							lexEntry.priority = -1;
							LT::Add(lextable, lexEntry);
							IT::PropTable* newprop = new IT::PropTable(buffWord, elementLeng);
							IT::Add(idtable, *newprop);
							LEX::LexEntry lexx;
							lexx.entry = &lextable.table[lexpos];
							lexx.propt = NULL;
							LEX::Add(lexlist, lexx);
							++lexpos;
							++lexInLine;
							element.clear();
							elementLeng = 0;
						}
					}
					std::string symbol;
					symbol += ch;
					char* buffChar = new char[symbol.length() + 1];
					for (int i = 0; i < symbol.length() + 1; ++i)
						buffChar[i] = symbol[i];
					lexc = FST::called(buffChar);
					if (lexc != '\0')
					{
						LT::Entry lexEntry;
						lexEntry.lexema[0] = lexc;
						lexEntry.sn = strNumber;
						lexEntry.pn = posNumber;
						lexEntry.idxProp = TI_NULLIDX;
						lexEntry.name[0] = lexc;
						lexEntry.name[1] = '\0';
						if ((lexc == LEX_PLUS) || (lexc == LEX_MINUS))
							lexEntry.priority = 2;
						else if ((lexc == LEX_STAR) || (lexc == LEX_DIRSLASH) || (lexc == LEX_MODULO))
							lexEntry.priority = 3;
						else
							lexEntry.priority = -1;
						LT::Add(lextable, lexEntry);
						char bchar[2] = "_";
						IT::PropTable* newprop = new IT::PropTable(bchar, 2);
						IT::Add(idtable, *newprop);
						LEX::LexEntry lexx;
						lexx.entry = &lextable.table[lexpos];
						lexx.propt = NULL;
						LEX::Add(lexlist, lexx);
						++lexpos;
						++lexInLine;
						++posNumber;
						element.clear();
						elementLeng = 0;
					}
					if ((ch == LEX_DOT))
						isobj = false;
					else
						isobj = true;
				}
				else
				{
					element += ch;
					++elementLeng;
					++posNumber;
					if (ch == '"')
						islit = !islit;
				}
			}
			ch = input.get();

		}
		input.close();

		return;
	}

	void SemanticCheck(LEX::LexList& lexlist, LT::LexTable& lextable, IT::IdTable& idtable, LitT::LitTable& littable)
	{
		int position = 0;
		int depth = 0;
		int line = 0;
		bool canIUseOperators = false;
		while (position < lextable.size)
		{
			if (lextable.table[position].sn != line)
			{
				line = lextable.table[position].sn;
			}
			if (lextable.table[position].lexema[0] == LEX_PROPERTY)
			{
				++depth;
				if (depth > LEX_MAX_DEPTH)
				{
					ERROR_THROW_IN(202, lextable.table[position].sn, lextable.table[position].pn);
				}
			}
			else
			{
				depth = 0;
			}
			if ((lextable.table[position].lexema[0] == LEX_INTERROGATION) && (position > 0) && (lextable.table[position - 1].lexema[0] != LEX_EQUALS))
			{
				ERROR_THROW_IN(203, lextable.table[position].sn, lextable.table[position].pn);
			}
			else if ((lextable.table[position].lexema[0] == LEX_EQUALS) && (!idtable.Global.check))
			{
				ERROR_THROW_IN(204, lextable.table[position].sn, lextable.table[position].pn);
			}
			else if ((lextable.table[position].lexema[0] == LEX_EQUALS) && (lextable.table[position - 1].lexema[0] != LEX_GLOBAL))
			{
				ERROR_THROW_IN(207, lextable.table[position].sn, lextable.table[position].pn);
			}
			else if ((lextable.table[position].lexema[0] == LEX_LOOP) && (lextable.table[position - 1].lexema[0] != LEX_GLOBAL))
			{
				ERROR_THROW_IN(208, lextable.table[position].sn, lextable.table[position].pn);
			}
			else if ((lextable.table[position].lexema[0] == LEX_CHECK) && (lextable.table[position - 1].lexema[0] != LEX_GLOBAL))
			{
				ERROR_THROW_IN(211, lextable.table[position].sn, lextable.table[position].pn);
			}
			else if ((lextable.table[position].lexema[0] == LEX_PRINT) && (lextable.table[position - 1].lexema[0] == LEX_GLOBAL))
			{
				ERROR_THROW_IN(205, lextable.table[position].sn, lextable.table[position].pn);
			}
			else if ((lextable.table[position].lexema[0] == LEX_PRINT) && ((lextable.table[position - 1].lexema[0] != LEX_PROPERTY) && (lextable.table[position - 1].lexema[0] != LEX_OBJECT)))
			{
				ERROR_THROW_IN(206, lextable.table[position].sn, lextable.table[position].pn);
			}
			else if (lextable.table[position].lexema[0] == LEX_CHECK)
			{
				idtable.Global.check = true;
			}
			if (lextable.table[position].lexema[0] == LEX_IS)
			{
				canIUseOperators = true;
			}
			else if (lextable.table[position].lexema[0] == LEX_SEMICOLON)
			{
				canIUseOperators = false;
			}
			if (((lextable.table[position].lexema[0] == LEX_PLUS) || (lextable.table[position].lexema[0] == LEX_MINUS) || (lextable.table[position].lexema[0] == LEX_STAR) || (lextable.table[position].lexema[0] == LEX_DIRSLASH) || (lextable.table[position].lexema[0] == LEX_MODULO)) && (!canIUseOperators))
			{
				ERROR_THROW_IN(209, lextable.table[position].sn, lextable.table[position].pn);
			}
			/*if ((lexlist.table[position].propt->integer == 0) && (lextable.table[position].lexema[0] == LEX_DIRSLASH))
			{
				ERROR_THROW_IN(210, lextable.table[position].sn, lextable.table[position].pn);
			}*/
			else if (((lextable.table[position].lexema[0] == LEX_ADD) || (lextable.table[position].lexema[0] == LEX_APPEND)) && (lextable.table[position - 1].lexema[0] == LEX_GLOBAL))
			{
				ERROR_THROW_IN(212, lextable.table[position].sn, lextable.table[position].pn);
			}
			else if (lextable.table[position].lexema[0] == LEX_ADD)
			{
				if (lextable.table[position + 2].lexema[0] == LEX_OBJECT)
				{
					IT::PropTable newprop = IT::PropTable(lextable.table[position + 2].name, sizeof(lextable.table[position + 2].name));
					int iserror = IT::Add(*idtable.table[IT::IsObj(&idtable, idtable.table[position - 1].name, 8) - 1].table, newprop);
					if (iserror == -1)
					{
						ERROR_THROW_IN(212, lextable.table[position].sn, lextable.table[position].pn);
					}
				}
				else if (lextable.table[position + 2].lexema[0] == LEX_LITERAL)
				{
					LitT::Entry le = LitT::GetEntry(littable, lextable.table[position + 2].sn, lextable.table[position + 2].pn);
					IT::PropTable newprop = IT::PropTable(le.literal, sizeof(le.literal));
					int iserror = IT::Add(idtable.table[IT::IsObj(&idtable, idtable.table[position - 1].name, 8) - 1], newprop);
					if (iserror == -1)
					{
						ERROR_THROW_IN(212, lextable.table[position].sn, lextable.table[position].pn);
					}
				}
			}
			else if (lextable.table[position].lexema[0] == LEX_APPEND)
			{
				if (lextable.table[position + 2].lexema[0] == LEX_LITERAL)
				{
					LitT::Entry le = LitT::GetEntry(littable, lextable.table[position + 2].sn, lextable.table[position + 2].pn);
					IT::PropTable newprop = IT::PropTable(le.literal, sizeof(le.literal));
					char ch1[10] = "parameter";
					ch1[9] = '\0';
					IT::PropTable newparameter = IT::PropTable(ch1, 10);
					char ch2[8] = "essence";
					ch2[7] = '\0';
					IT::PropTable newessence = IT::PropTable(ch2, 10);
					IT::Add(newprop, newessence);
					IT::Add(newprop, newparameter);
					newprop.appended = true;
					int iserror = IT::Add(idtable.table[IT::IsObj(&idtable, idtable.table[position - 1].name, 8) - 1], newprop);
					if (iserror == -1)
					{
						ERROR_THROW_IN(212, lextable.table[position].sn, lextable.table[position].pn);
					}
				}
				else
				{
					ERROR_THROW_IN(216, lextable.table[position].sn, lextable.table[position].pn);
				}
			}
			if (lextable.table[position].lexema[0] == LEX_PROPERTY)
			{
				std::string buff = "";
				for (int i = 0; lextable.table[position].name[i] != '\0'; ++i)
					buff += lextable.table[position].name[i];
				if (!((buff == "integer") || (buff == "string") || (buff == "doublenum") || (buff == "hexinteger") || (buff == "name") || (buff == "propertycounter") || (buff == "integerdefault") || (buff == "stringdefault") || (buff == "doubledefault") || (buff == "start") || (buff == "return") || (buff == "check") || (buff == "equals") || (buff == "print") || (buff == "_")))
				{
					/*if (IT::IsProp(idtable.table[IT::IsObj(&idtable, idtable.table[position - 1].name, 8) - 1], lextable.table[position].name, 8) == TI_NULLIDX)
					{
						ERROR_THROW_IN(214, lextable.table[position].sn, lextable.table[position].pn);
					}*/
				}
			}
			++position;
		}
	}

	std::string AddToName(LT::LexTable& lextable, int i)
	{
		std::string str = "";
		if (lextable.table[i].lexema[0] != LEX_OBJECT)
			str += AddToName(lextable, i - 1);
		for (int j = 0; (lextable.table[i].name[j] != '\0') && (j < 236); ++j)
			if (lextable.table[i].name[j] == (char)(-52))
			{
				lextable.table[i].name[j] = '\0';
				--j;
			}
		str += std::string(lextable.table[i].name) + "_";
		//str += std::string(lextable.table[i].name) + "_";
		return str;
	}
	LT::LexTable LexSmooth(LT::LexTable& lextable, IT::IdTable& idtable)
	{
		LT::LexTable resultTable = LT::Create(32768);
		std::string st = "_";
		for (int i = 0; i < lextable.size; ++i)
		{
			if ((lextable.table[i].lexema[0] == LEX_IS) || (lextable.table[i].lexema[0] == LEX_COMMA) || (lextable.table[i].lexema[0] == LEX_MINUS) || (lextable.table[i].lexema[0] == LEX_PLUS) || (lextable.table[i].lexema[0] == LEX_STAR) || (lextable.table[i].lexema[0] == LEX_DIRSLASH) || (lextable.table[i].lexema[0] == LEX_MODULO) || (lextable.table[i].lexema[0] == LEX_SEMICOLON) || (lextable.table[i].lexema[0] == LEX_PRINT) || (lextable.table[i].lexema[0] == LEX_ADD) || (lextable.table[i].lexema[0] == LEX_APPEND))
			{
				if ((lextable.table[i - 1].lexema[0] == LEX_PROPERTY) && ((lextable.table[i - 2].lexema[0] != LEX_GLOBAL) &&
					((std::string(idtable.table[i - 1].name) != std::string("name")) && (std::string(idtable.table[i - 1].name) != std::string("propertycounter")) &&
						(std::string(idtable.table[i - 1].name) != std::string("integerdefault")) && (std::string(idtable.table[i - 1].name) != std::string("stringdefault")) && (std::string(idtable.table[i - 1].name) != std::string("doubledefault")) &&
						(std::string(idtable.table[i - 1].name) != std::string("table")) && (std::string(idtable.table[i - 1].name) != std::string("start")) && (std::string(idtable.table[i - 1].name) != std::string("return")) &&
						(std::string(idtable.table[i - 1].name) != std::string("check")) && (std::string(idtable.table[i - 1].name) != std::string("_")))))
				{
					st += AddToName(lextable, i - 1);
					LT::Entry newentry;
					newentry.idxProp = lextable.table[i - 1].idxProp;
					newentry.lexema[0] = lextable.table[i - 1].lexema[0];
					newentry.pn = lextable.table[i - 1].pn;
					newentry.sn = lextable.table[i - 1].sn;
					newentry.priority = lextable.table[i - 1].priority;
					int l = 0;
					while ((st[l] != '\0') && (l < 236))
					{
						newentry.name[l] = st[l];
						++l;
					}
					newentry.name[l] = '\0';
					LT::Add(resultTable, newentry);
					st = "_";
				}
				if ((lextable.table[i - 1].lexema[0] == LEX_OBJECT))
				{
					st += AddToName(lextable, i - 1);
					LT::Entry newentry;
					newentry.idxProp = lextable.table[i - 1].idxProp;
					newentry.lexema[0] = lextable.table[i - 1].lexema[0];
					newentry.pn = lextable.table[i - 1].pn;
					newentry.sn = lextable.table[i - 1].sn;
					newentry.priority = lextable.table[i - 1].priority;
					int l = 0;
					while ((st[l] != '\0') && (l < 236))
					{
						newentry.name[l] = st[l];
						++l;
					}
					newentry.name[l] = '\0';
					LT::Add(resultTable, newentry);
					st = "_";
				}
			}
		}
		return resultTable;
	}
	int FindInSmoothed(LT::LexTable& lextable, char* name)
	{
		for (int i = 0; i < lextable.size; ++i)
		{
			bool equal = true;
			for (int j = 1; (j < 8)&&(lextable.table[i].name[j] != '_'); ++j)
				if (lextable.table[i].name[j] != name[j-1])
				{
					equal = false;
					break;
				}
			if (equal) return i;
		}
		return -1;
	}
	int FindInNormal(LT::LexTable& lextable, char* name)
	{
		for (int i = 0; i < lextable.size; ++i)
		{
			bool equal = true;
			for (int j = 1; (j < 8) && (lextable.table[i].name[j] != -52); ++j)
				if (lextable.table[i].name[j-1] != name[j - 1])
				{
					equal = false;
					break;
				}
			if (equal) return i;
		}
		return -1;
	}
}