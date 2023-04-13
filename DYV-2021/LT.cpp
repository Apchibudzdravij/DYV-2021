#include "Error.h"
#include <iostream>
#include <fstream>
#include <string>
#include "LT.h"

namespace LT 
{
	LexTable Create(int size)
	{
		LexTable lextable;
		lextable.maxsize = size;
		lextable.size = 0;
		lextable.table = new Entry[size];
		return lextable;
	}

	void Add(LexTable& lextable, Entry entry)
	{
		++lextable.size;
		if (lextable.size > lextable.maxsize)
			ERROR_THROW(103);
		lextable.table[lextable.size - 1] = entry;
		return;
	}

	Entry GetEntry(LexTable& lextable, int n)
	{
		if ((n > lextable.size) || (n < 0))
			ERROR_THROW(104);
		return lextable.table[n - 1];
	}

	void Delete(LexTable& lextable)
	{
		delete[] lextable.table;
		return;
	}

	void ShowLT(LexTable& lextable)
	{
		int line = 0;
		for (int i = 0; i < lextable.size; ++i)
		{
			if (lextable.table[i].sn > line)
			{
				++line;
				std::cout << "\n" << lextable.table[i].lexema[0];
			}
			else
				std::cout << lextable.table[i].lexema[0];
		}
		std::cout << std::endl;
	}

	void WriteLT(LexTable& lextable)
	{
		std::ofstream stream;//заношу в лог таблицы лексем номер первой строки
		remove("LexT.log");
		stream.open("LexT.log", std::ios_base::app);
		if (!stream.is_open())
		{
			ERROR_THROW(5)
		}
		for (int i = 0; i < lextable.size; ++i)
			stream << "	" << lextable.table[i].lexema[0] << " | " << lextable.table[i].sn << " | " << lextable.table[i].pn << "\n";
		stream.close();
	}
}