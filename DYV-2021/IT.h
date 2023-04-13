#pragma once
#include "LT.h"
#include "LitT.h"
#include "regex"

#define ID_MAXSIZE		240			//����. ���-�� �������� � ID
#define PROP_T_MAXSIZE  1024		//����. ���-�� ������� � �������� � �������
#define TI_INT_DEFAULT	0x00000000	//����. �� ��������� integer
#define TI_STR_DEFAULT	0x00		//����. �� ��������� string
#define TI_NULLIDX		0xffffffff	//��� �������� ��
#define TI_STR_MAXSIZE	255
#define START_COUNTER	6

namespace IT
{
	/*enum IDDATATYPE { INT = 1, STR = 2 };		//�� ���������������
	enum IDTYPE { V = 1, F = 2, P = 3, L = 3 }; //���� �������.: ����������, �������, ��������, �������

	struct Entry					//������ ��
	{
		int			idxfirstLE;		//������ ������ ������ � ��
		char		id[ID_MAXSIZE+1]; //id, ��������� �� ID_MAXSIZE
		IDDATATYPE	iddatatype;		//��� ������
		IDTYPE		idtype;			//��� ��������������
		union
		{
			int vint;	//����. integer
			struct
			{
				char len;
				char str[TI_STR_MAXSIZE - 1];
			}  vstr;	//����. string
			LT::Entry** lexstring;//���� id �����������, �� ����� �������, ����� ���������� ����������� ���� ����� ��� ����� �����
		}			value;			//�������� ��������������
		int lexInt;//����� ������� lexstring
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
		double doublenum;//�������������� �� �������
		char* hexinteger;
		char* name;//������ ��� ������
		int propertycounter = START_COUNTER;
		PropTable()
		{
			integer = integerdefault;//�������� ���� integer ���������
			doublenum = doubledefault;//�������� ���� doublenum ���������
			string = name = stringdefault;
			hexinteger = new char[4];//�������� ���� hexinteger ���������
			hexinteger[0] = '0';
			hexinteger[1] = 'x';
			hexinteger[2] = '0';
			hexinteger[3] = '\0';
		};
		PropTable(char* chArr, int len)
		{
			integer = integerdefault;//�������� ���� integer ���������
			doublenum = doubledefault;//�������� ���� doublenum ���������
			if (std::regex_match(chArr, chArr+len, std::regex("[0-9]*")))//���� ����� �����
			{
				int tens = 1;
				for (int i = 1; i < len; ++i)
					tens *= 10;
				for (int i = 0; i < len; ++i)//�������� ���� integer ���������
				{
					integer += ((int)(chArr[i]) - (int)'0') * tens;
					tens /= 10;
				}
				doublenum = integer;// �������� ���� doublenum ���������
				hexinteger = new char[len+1];
				hexinteger[0] = '0';
				hexinteger[1] = 'x';
				/*for (int i = 2; i < len + 2; ++i)
				{
					hexinteger[i] = chArr[i - 2];//�������� ���� hexinteger ���������
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
			else if (std::regex_match(chArr, chArr + len, std::regex("0x[a-f|0-9]+")))//���� ���� � 16 �/�
			{
				int i = 0;
				int tens = 1;
				hexinteger = new char[len+1];// �������� ���� hexinteger ���������
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
				for (int i = 2; i < len; ++i)//�������� ���� integer ���������
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
			else if (std::regex_match(chArr, chArr + len, std::regex("[0-9]+.[0-9]+")))//���� � ��������� ������
			{
				int i = 0;
				int tens = 1;
				
				while (chArr[i] != '.')
				{
					tens *= 10;
					++i;
				}
				hexinteger = new char[i];// �������� ���� hexinteger ���������
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
					doublenum += ((int)(chArr[i]) - (int)'0') * subtens;//�������� ���� doublenum (.xxxxx) ���������
					subtens *= 10;
					--i;
				}
				while (i >= 0)
				{
					doublenum += ((int)(chArr[i]) - (int)'0') * tens;//�������� ���� doublenum (xxxxx.) ���������
					tens /= 10;
					--i;
				}
				integer = (int)doublenum;// �������� ���� integer ���������
			}
			else//���� ������ ������
			{
				hexinteger = new char[4];//�������� ���� hexinteger ���������
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
			for (; (i < len)&&(chArr[i] != '\0'); ++i)//�������� ���� string ���������
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
			integer = integerdefault;//�������� ���� integer ���������
			doublenum = doubledefault;//�������� ���� doublenum ���������
			string = name = stringdefault;
			hexinteger = new char[3];//�������� ���� hexinteger ���������
			hexinteger[0] = '0';
			hexinteger[1] = 'x';
			hexinteger[2] = '0';
		};
		ObjTable(char* chArr, int len)
		{
			integer = integerdefault;//�������� ���� integer ���������
			doublenum = doubledefault;//�������� ���� doublenum ���������
			name = new char[len];
			if (std::regex_match(chArr, chArr+len, std::regex("[0-9]*")))//���� ����� �����
			{
				int tens = 1;
				for (int i = 0; i < len; ++i)
					tens *= 10;
				for (int i = 0; i < len; ++i)//�������� ���� integer ���������
				{
					integer += chArr[i] * tens;
					tens /= 10;
				}
				for (int i = 0; (i < len) && (i < 8); ++i)//�������� ���� string ���������
				{
					string[i] = chArr[i];
				}
				doublenum = integer;// �������� ���� doublenum ���������
				hexinteger = new char[len + 3];
				hexinteger[0] = '0';
				hexinteger[1] = 'x';
				for (int i = 2; i < len + 2; ++i)
				{
					hexinteger[i] = chArr[i - 2];//�������� ���� hexinteger ���������
				}
				hexinteger[len+2] = '\0';
			}
			else if (std::regex_match(chArr, chArr+len, std::regex("[0-9]+.[0-9]+")))//���� � ��������� ������
			{
				int i = 0;
				int tens = 1;
				while (chArr[i] != '.')
				{
					tens *= 10;
					++i;
				}
				hexinteger = new char[i+1];// �������� ���� hexinteger ���������
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
					doublenum += ((int)(chArr[i]) - (int)'0') * subtens;//�������� ���� doublenum (.xxxxx) ���������
					subtens *= 10;
					--i;
				}
				while (i >= 0)
				{
					doublenum += ((int)(chArr[i]) - (int)'0') * tens;//�������� ���� doublenum (xxxxx.) ���������
					tens /= 10;
					--i;
				}
				integer = (int)doublenum;// �������� ���� integer ���������
			}
			else if (std::regex_match(chArr, chArr+len, std::regex("0x[0-9]+")))//���� ���� � 16 �/�
			{
				int i = 0;
				int tens = 1;
				hexinteger = new char[len];// �������� ���� hexinteger ���������
				while (i < len)
				{
					hexinteger[i] = chArr[i];
					++i;
				}
				integer = std::stoi(hexinteger, 0, 16);
				doublenum = integer;
			}
			else//���� ������ ������
			{
				hexinteger = new char[4];//�������� ���� hexinteger ���������
				hexinteger[0] = '0';
				hexinteger[1] = 'x';
				hexinteger[2] = '0';
				hexinteger[3] = '\0';
			}
			string = new char[len];
			for (int i = 0; i < len; ++i)//�������� ���� string ���������
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

	IdTable Create(int size);						//������� ��
	int Add(IdTable& idtable, PropTable entry);		//�������� ������ � ��
	//void Add(ObjTable& idtable, PropTable entry);		//�������� ������ � ��
	int Add(PropTable& idtable, PropTable& entry);		//�������� ������ � ��
	PropTable GetEntry(IdTable& idtable, int n);		//�������� ������ ��
	int IsObj(IdTable* idtable, char* id, int len);//������� ����� ������ ��� TI_NULLIDX, ���� ��������� �O � �������������
	//int IsObjWithProp(IdTable& idtable, char* id, int len);
	//int IsProp(ObjTable* objtable, char* id, int len);//������� ����� ������ ��� TI_NULLIDX, ���� ��������� �� � �������������
	int IsProp(PropTable& proptable, char* id, int len);
	void Delete(IdTable& idtable);					//������� ��
	bool WasUsed(IT::IdTable& idtable, char* str, int pos);
	IdTable Smooth(IdTable& idtable);
	void WriteIT(IdTable& idtable);
	//void CreateFromFile(LT::LexTable& lextable, IdTable& idtable, LitT::LitTable& littable, wchar_t infile[]);
	//void ShowAll(IdTable& idtable);
}