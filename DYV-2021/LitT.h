#pragma once

#define LIT_MAXLEN 128
#define LIT_ERRORLIT ENTRY errlitt = {errlit.literal = "Err_NoPos"; errlit.strNumInLT = -1; errlit.posNumInLT = -1;};

namespace LitT
{
	struct Entry
	{
		char literal[LIT_MAXLEN];	//запоминаю содержимое литерала
		enum
		{
			INT = 1,
			STR = 2,
			DOU = 3,
			HEX = 4,
		}type;
		int actualSize;
		int strNumInLT;				//запоминаю строку в ТЛ
		int posNumInLT;				//запоминаю позицию в строке в ТЛ
	};

	struct LitTable
	{
		int maxsize;				//ёмкость таблицы литералов
		int size;					//текущий размер таблицы литералов
		Entry* table;
	};

	LitTable Create(int size);					//создать таблицу, зная ёмкость
	void Add(LitTable& lextable, Entry entry);	//добавить строку в таблицу
	Entry GetEntry(LitTable& littable, int str, int pos);	//получить строку таблицы по номеру строки ТЛ и позиции в ней
	void Delete(LitTable& littable);			//удалить таблицу
	void WriteLT(LitTable& littable);
}