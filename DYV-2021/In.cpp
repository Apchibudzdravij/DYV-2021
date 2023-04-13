#include "stdafx.h"
#include <fstream>
#include <string>
#include "In.h"
#include "Error.h"
#include "FST.h"

#define MAX_STRING_LENGTH 255

namespace In
{
	int lns;
	int gnr;
	char text[10000];
	IN getin(wchar_t infile[])
	{
		setlocale(LC_ALL, ".1251");
		IN inn;
		inn.size = 0;
		inn.lines = 1;
		inn.ignor = 0;
		char text[10000] = {};
		short position = 0;		//все символы, которые попадут в промежуточный файл, используется как позиция в массиве text[]
		bool canHaveSep = false;//пробелы и табы внутри конструкции @" " должны оставаться, а вне такой конструкции - нет
		bool isComment = false;//игнорирую все сиволы после символа-начала комментария
		std::ifstream input(infile, std::ios::in);
		if (!input)//проверяю поток на существование
		{
			ERROR_THROW(4);
		}
		int positionInLine = 0;//Счётчик, увеличивает значение на 1 с каждым символом, обнуляется при переходе на новую строку
		while(!input.eof())
		{
			char ch = input.get();

			++positionInLine;
			if ((((int)ch >= 0)&&(inn.code[(int)ch] == IN::F))||(((int)ch < 0)&&(inn.code[(int)(ch) + 256] == IN::F)))//Проверяет на наличие ошибочных символов и выдаёт ошибку 111, если такие есть
				ERROR_THROW_IN(102, inn.lines, positionInLine);
			if ((int)ch >= 0)//Этот и след. ифы проверяют нужно ли игнорировать символ
			{
				if (inn.code[(int)ch] == IN::I)
				{
					++inn.ignor;
					continue;
				}
			}
			else if ((int)ch < 0)
			{
				if (inn.code[(int)ch + 256] == IN::I)
				{
					++inn.ignor;
					continue;
				}
			}

			if (!isComment)
			{
				if (ch == '#')
				{
					isComment = true;
					continue;
				}
				if (ch == '\n')//все \n замещаются спец-символом, обнуляется позиция в строке
				{
					text[position] = (char)11;
					++position;
					++inn.lines;
					positionInLine = 0;
					++inn.size;
					continue;
				}
				if ((ch == '"') && canHaveSep)//проверяет, закрывающие ли это кавычки конструкции @" "
				{
					canHaveSep = false;
					text[position] = ch;
					++position;
					++inn.size;
					continue;
				}
				if ((ch == '"') && (position > 0) && (text[position - 1] == '@'))//-//-, открывающие -//-
				{
					canHaveSep = true;
					text[position - 1] = ch;
					continue;
				}
				if (!canHaveSep)//Если не внутри @" "
				{
					if ((ch == ' ') || (ch == '	'))
					{
						continue;
					}
				}
				if ((int)ch >= 0)
				{
					if (inn.code[(int)ch] == IN::T)
					{
						++inn.size;
						if (((int)ch >= 65)&&((int)ch <= 90))
						{
							text[position] = (char)((int)ch + 32);
						}
						else
						{
							text[position] = ch;
						}
						++position;
					}
				}
				else if ((int)ch < 0)//кириллица запрещена, но возможно расширение и добавление поддержки символов кириллицы
				{
					if (inn.code[(int)ch + 256] == IN::T)
					{
						++inn.size;
						text[position] = ch;
						++position;
					}
				}
			}
			else
			{
				if (ch == '\n')
				{
					text[position] = (char)11;
					++position;
					++inn.lines;
					positionInLine = 0;
					++inn.size;
					isComment = false;
					continue;
				}
				else
					--positionInLine;
			}

		}
		inn.text = &text[0];
		std::ofstream stream;
		stream.open("TEMP.txt", std::ios_base::out);
		stream << text;
		stream.close();
		return inn;
	}
}