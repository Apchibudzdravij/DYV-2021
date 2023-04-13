#pragma once
#include "LT.h"
#include "LitT.h"
#include "regex"

#define ID_MAXSIZE		240			//макс. кол-во символов в ID
#define PROP_T_MAXSIZE  1024		//макс. кол-во свойств у объектов и свойств
#define TI_INT_DEFAULT	0x00000000	//знач. по умолчанию integer
#define TI_STR_DEFAULT	0x00		//знач. по умолчанию string
#define TI_NULLIDX		0xffffffff	//нет элемента ТИ
#define TI_STR_MAXSIZE	255
#define START_COUNTER	6

namespace IT
{
	/*enum IDDATATYPE { INT = 1, STR = 2 };		//ТД идентификаторов
	enum IDTYPE { V = 1, F = 2, P = 3, L = 3 }; //типы идентиф.: переменная, функция, параметр, литерал

	struct Entry					//строка ТИ
	{
		int			idxfirstLE;		//индекс первой строки в ТЛ
		char		id[ID_MAXSIZE+1]; //id, усекается до ID_MAXSIZE
		IDDATATYPE	iddatatype;		//тип данных
		IDTYPE		idtype;			//тип идентификатора
		union
		{
			int vint;	//знач. integer
			struct
			{
				char len;
				char str[TI_STR_MAXSIZE - 1];
			}  vstr;	//знач. string
			LT::Entry** lexstring;//если id вычисляемый, то храню цепочку, чтобы вычисления проводились лишь когда это будет нужно
		}			value;			//значение идентификатора
		int lexInt;//длина цепочки lexstring
	};*/


	static struct GlobalObj
	{
		int integerdefault = 0;
		double doubledefault = 0.0;
		char* stringdefault = {};
		bool check = false;
	};


	struct PropTable : GlobalObj
	{
		bool appended = false;
		int integer;
		char* string;
		double doublenum;//зарезервирован на будущее
		char* hexinteger;
		char* name;//только для чтения
		int propertycounter = START_COUNTER;
		PropTable()
		{
			integer = integerdefault;//заполняю поле integer значением
			doublenum = doubledefault;//заполняю поле doublenum значением
			string = name = stringdefault;
			hexinteger = new char[4];//заполняю поле hexinteger значением
			hexinteger[0] = '0';
			hexinteger[1] = 'x';
			hexinteger[2] = '0';
			hexinteger[3] = '\0';
		};
		PropTable(char* chArr, int len)
		{
			integer = integerdefault;//заполняю поле integer значением
			doublenum = doubledefault;//заполняю поле doublenum значением
			if (std::regex_match(chArr, chArr+len, std::regex("[0-9]*")))//если целое число
			{
				int tens = 1;
				for (int i = 1; i < len; ++i)
					tens *= 10;
				for (int i = 0; i < len; ++i)//заполняю поле integer значением
				{
					integer += ((int)(chArr[i]) - (int)'0') * tens;
					tens /= 10;
				}
				doublenum = integer;// заполняю поле doublenum значением
				hexinteger = new char[len+1];
				hexinteger[0] = '0';
				hexinteger[1] = 'x';
				/*for (int i = 2; i < len + 2; ++i)
				{
					hexinteger[i] = chArr[i - 2];//заполняю поле hexinteger значением
				}*/
				int i = 2;
				int rem;
				int buffint = integer;
				while ((buffint != 0)&&(i <= len))
				{
					rem = buffint % 16;
					if (rem < 10)
						rem = rem + 48;
					else
						rem = rem + 55;
					if (len > 2)
						hexinteger[len + 1 - i] = rem;
					else
						hexinteger[len + 2 - i] = rem;
					i++;
					buffint = buffint / 16;
				}
				if (len > 2)
					hexinteger[len] = '\0';
				else
					hexinteger[len + 1] = '\0';
			}
			else if (std::regex_match(chArr, chArr + len, std::regex("0x[a-f|0-9]+")))//Если вход в 16 с/с
			{
				int i = 0;
				int tens = 1;
				hexinteger = new char[len+1];// заполняю поле hexinteger значением
				while (i < len)
				{
					hexinteger[i] = chArr[i];
					++i;
				}
				hexinteger[len] = '\0';
				/*int buffres = 0; int mem = 0;//modify if std::stoi won't work!
				for (int ind = 0; ind < len - 2; ++ind)
				{
					tens *= 10;
				}
				for (int i = 2; i < len; ++i)
					tens *= 10;
				for (int i = 2; i < len; ++i)//заполняю поле integer значением
				{
					if (((int)(chArr[i]) - (int)'0') < 10)
					{
						integer += (((int)(chArr[i]) - (int)'0') + mem) * tens;
						mem = 0;
					}
					else
					{
						integer += ((((int)(chArr[i]) - (int)'0') % 10) + mem) * tens ;
						//mem = //!!!!!!!!!!!!!!!!!!!!!!!!!!CHeck is symbol correct for 16c/c
					}
					tens /= 10;
				}*/
				integer = std::stoi(hexinteger, 0, 16);
				doublenum = integer;
			}
			else if (std::regex_match(chArr, chArr + len, std::regex("[0-9]+.[0-9]+")))//если с плавающей точкой
			{
				int i = 0;
				int tens = 1;
				
				while (chArr[i] != '.')
				{
					tens *= 10;
					++i;
				}
				hexinteger = new char[i];// заполняю поле hexinteger значением
				hexinteger[0] = '0';
				hexinteger[1] = 'x';
				for (int j = 2; j < i + 2; ++j)
				{
					hexinteger[j] = chArr[j - 2];
				}
				float subtens = 1.0;
				while (i < len)
				{
					subtens /= 10;
					++i;
				}
				while (chArr[i] != '.')
				{
					doublenum += ((int)(chArr[i]) - (int)'0') * subtens;//заполняю поле doublenum (.xxxxx) значением
					subtens *= 10;
					--i;
				}
				while (i >= 0)
				{
					doublenum += ((int)(chArr[i]) - (int)'0') * tens;//заполняю поле doublenum (xxxxx.) значением
					tens /= 10;
					--i;
				}
				integer = (int)doublenum;// заполняю поле integer значением
			}
			else//если просто строка
			{
				hexinteger = new char[4];//заполняю поле hexinteger значением
				hexinteger[0] = '0';
				hexinteger[1] = 'x';
				hexinteger[2] = '0';
				hexinteger[3] = '\0';
			}
			string = new char[len];
			if (len < 9)
				name = new char[len];
			else
				name = new char[9];
			int i = 0;
			for (; (i < len)&&(chArr[i] != '\0'); ++i)//заполняю поле string значением
			{
				string[i] = chArr[i];
				if (i < 8)
					name[i] = chArr[i];
			}
			if (i < 8)
			{
				name[i] = '\0';
				string[i] = '\0';
			}
			else
			{
				string[i] = '\0';
				name[8] = '\0';
			}
			table = new PropTable[PROP_T_MAXSIZE];
		}
		PropTable* table;
	};

	/*struct ObjTable : GlobalObj
	{
		int integer;
		char* string;
		double doublenum;
		char* hexinteger;
		char* name;
		int propertycounter = START_COUNTER;
		ObjTable()
		{
			integer = integerdefault;//заполняю поле integer значением
			doublenum = doubledefault;//заполняю поле doublenum значением
			string = name = stringdefault;
			hexinteger = new char[3];//заполняю поле hexinteger значением
			hexinteger[0] = '0';
			hexinteger[1] = 'x';
			hexinteger[2] = '0';
		};
		ObjTable(char* chArr, int len)
		{
			integer = integerdefault;//заполняю поле integer значением
			doublenum = doubledefault;//заполняю поле doublenum значением
			name = new char[len];
			if (std::regex_match(chArr, chArr+len, std::regex("[0-9]*")))//если целое число
			{
				int tens = 1;
				for (int i = 0; i < len; ++i)
					tens *= 10;
				for (int i = 0; i < len; ++i)//заполняю поле integer значением
				{
					integer += chArr[i] * tens;
					tens /= 10;
				}
				for (int i = 0; (i < len) && (i < 8); ++i)//заполняю поле string значением
				{
					string[i] = chArr[i];
				}
				doublenum = integer;// заполняю поле doublenum значением
				hexinteger = new char[len + 3];
				hexinteger[0] = '0';
				hexinteger[1] = 'x';
				for (int i = 2; i < len + 2; ++i)
				{
					hexinteger[i] = chArr[i - 2];//заполняю поле hexinteger значением
				}
				hexinteger[len+2] = '\0';
			}
			else if (std::regex_match(chArr, chArr+len, std::regex("[0-9]+.[0-9]+")))//если с плавающей точкой
			{
				int i = 0;
				int tens = 1;
				while (chArr[i] != '.')
				{
					tens *= 10;
					++i;
				}
				hexinteger = new char[i+1];// заполняю поле hexinteger значением
				hexinteger[0] = '0';
				hexinteger[1] = 'x';
				for (int j = 2; j < i + 2; ++j)
				{
					hexinteger[j] = chArr[j - 2];
				}
				hexinteger[i] = '\0';
				float subtens = 1.0;
				while (i < sizeof(subtens))
				{
					subtens /= 10;
					++i;
				}
				while (chArr[i] != '.')
				{
					doublenum += ((int)(chArr[i]) - (int)'0') * subtens;//заполняю поле doublenum (.xxxxx) значением
					subtens *= 10;
					--i;
				}
				while (i >= 0)
				{
					doublenum += ((int)(chArr[i]) - (int)'0') * tens;//заполняю поле doublenum (xxxxx.) значением
					tens /= 10;
					--i;
				}
				integer = (int)doublenum;// заполняю поле integer значением
			}
			else if (std::regex_match(chArr, chArr+len, std::regex("0x[0-9]+")))//Если вход в 16 с/с
			{
				int i = 0;
				int tens = 1;
				hexinteger = new char[len];// заполняю поле hexinteger значением
				while (i < len)
				{
					hexinteger[i] = chArr[i];
					++i;
				}
				integer = std::stoi(hexinteger, 0, 16);
				doublenum = integer;
			}
			else//если просто строка
			{
				hexinteger = new char[4];//заполняю поле hexinteger значением
				hexinteger[0] = '0';
				hexinteger[1] = 'x';
				hexinteger[2] = '0';
				hexinteger[3] = '\0';
			}
			string = new char[len];
			for (int i = 0; i < len; ++i)//заполняю поле string значением
			{
				string[i] = chArr[i];
			}
			table = new PropTable[PROP_T_MAXSIZE];
		}
		PropTable* table;
	};*/

	struct IdTable
	{
		int maxsize;
		int size;
		PropTable* table;
		GlobalObj Global;
	};

	IdTable Create(int size);						//создать ТИ
	int Add(IdTable& idtable, PropTable entry);		//добавить строку в ТИ
	//void Add(ObjTable& idtable, PropTable entry);		//добавить строку в ТО
	int Add(PropTable& idtable, PropTable& entry);		//добавить строку в ТП
	PropTable GetEntry(IdTable& idtable, int n);		//получить строку ТИ
	int IsObj(IdTable* idtable, char* id, int len);//Вернуть номер строки или TI_NULLIDX, зная экземпляр ТO и идентификатор
	//int IsObjWithProp(IdTable& idtable, char* id, int len);
	//int IsProp(ObjTable* objtable, char* id, int len);//Вернуть номер строки или TI_NULLIDX, зная экземпляр ТП и идентификатор
	int IsProp(PropTable& proptable, char* id, int len);
	void Delete(IdTable& idtable);					//Удалить ТИ
	bool WasUsed(IT::IdTable& idtable, char* str, int pos);
	IdTable Smooth(IdTable& idtable);
	void WriteIT(IdTable& idtable);
	//void CreateFromFile(LT::LexTable& lextable, IdTable& idtable, LitT::LitTable& littable, wchar_t infile[]);
	//void ShowAll(IdTable& idtable);
}