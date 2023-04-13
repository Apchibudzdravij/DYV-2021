#pragma once

#define LEXEMA_FIXSIZE	1			//фиксированный размер лексемы
#define LEX_MAX_DEPTH	26
#define LT_TI_NULLIDX	0xffffffff	//нет элемента таблицы идентификаторов
#define LEX_OBJECT		'o'		//объект
#define LEX_GLOBAL		'g'		//глобальный объект
#define LEX_PROPERTY	'p'		//свойство
#define LEX_LITERAL		'l'		//любой литерал
#define LEX_ADD			'n'		//особое свойство объекта - добавление свойства
#define LEX_APPEND		'f'		//особое свойство объекта - добавление активного свойства
#define LEX_PRINT		's'		//вывод данных
#define LEX_CHECK		'c'		//активное свойство - сравнение двух объектов или литералов
#define LEX_EQUALS		'e'		//значение сравнения от свойства CHECK
#define LEX_LOOP		'&'		//особое свойство работы с циклом
#define LEX_SEMICOLON	';'		//разделение выражений
#define LEX_COLON		':'		//разделение блоков кода в условном операторе ?::
#define LEX_COMMA		','		//разделение объектов или литералов, сравниваемых в CHECK
#define LEX_DOT			'.'		//выделение свойства
#define LEX_PLUS		'+'		//арифметические операции
#define LEX_MINUS		'-'
#define LEX_STAR		'*'
#define LEX_DIRSLASH	'/'
#define LEX_MODULO		'%'
#define LEX_IS			'='		//оператор присваивания
#define LEX_INTERROGATION '?'	//начало условного оператора ?::

namespace LT
{
	struct Entry			//Строка таблицы лексем
	{
		char lexema[LEXEMA_FIXSIZE];
		int sn;				//Номер строки в исх. тексте
		int pn;				//Номер позиции в строке в и.т.
		int idxProp;		//индекс в таблице свойств или  LI_TI_NULLDX
		/*//int idxObj;					
		//индекс в таблице объектов или LT_TI_NULLDX*/
		int priority;	//2 у +-, 3 у */%, -1 у остальных
		char name[240];
	};

	struct LexTable					//экземпляр таблицы лексем
	{
		int maxsize;				//ёмкость ТЛ
		int size;					//текущий размер ТЛ
		Entry* table;				//массив строк ТЛ
	};

	LexTable Create(int size);					//создать ТЛ, зная ёмкость
	void Add(LexTable& lextable, Entry entry);	//добавить строку в ТЛ

	Entry GetEntry(LexTable& lextable, int n);	//получить строку ТЛ
	void Delete(LexTable& lextable);			//удалить ТЛ

	void ShowLT(LexTable& lextable);
	void WriteLT(LexTable& lextable);
}