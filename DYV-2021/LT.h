#pragma once

#define LEXEMA_FIXSIZE	1			//������������� ������ �������
#define LEX_MAX_DEPTH	26
#define LT_TI_NULLIDX	0xffffffff	//��� �������� ������� ���������������
#define LEX_OBJECT		'o'		//������
#define LEX_GLOBAL		'g'		//���������� ������
#define LEX_PROPERTY	'p'		//��������
#define LEX_LITERAL		'l'		//����� �������
#define LEX_ADD			'n'		//������ �������� ������� - ���������� ��������
#define LEX_APPEND		'f'		//������ �������� ������� - ���������� ��������� ��������
#define LEX_PRINT		's'		//����� ������
#define LEX_CHECK		'c'		//�������� �������� - ��������� ���� �������� ��� ���������
#define LEX_EQUALS		'e'		//�������� ��������� �� �������� CHECK
#define LEX_LOOP		'&'		//������ �������� ������ � ������
#define LEX_SEMICOLON	';'		//���������� ���������
#define LEX_COLON		':'		//���������� ������ ���� � �������� ��������� ?::
#define LEX_COMMA		','		//���������� �������� ��� ���������, ������������ � CHECK
#define LEX_DOT			'.'		//��������� ��������
#define LEX_PLUS		'+'		//�������������� ��������
#define LEX_MINUS		'-'
#define LEX_STAR		'*'
#define LEX_DIRSLASH	'/'
#define LEX_MODULO		'%'
#define LEX_IS			'='		//�������� ������������
#define LEX_INTERROGATION '?'	//������ ��������� ��������� ?::

namespace LT
{
	struct Entry			//������ ������� ������
	{
		char lexema[LEXEMA_FIXSIZE];
		int sn;				//����� ������ � ���. ������
		int pn;				//����� ������� � ������ � �.�.
		int idxProp;		//������ � ������� ������� ���  LI_TI_NULLDX
		/*//int idxObj;					
		//������ � ������� �������� ��� LT_TI_NULLDX*/
		int priority;	//2 � +-, 3 � */%, -1 � ���������
		char name[240];
	};

	struct LexTable					//��������� ������� ������
	{
		int maxsize;				//������� ��
		int size;					//������� ������ ��
		Entry* table;				//������ ����� ��
	};

	LexTable Create(int size);					//������� ��, ���� �������
	void Add(LexTable& lextable, Entry entry);	//�������� ������ � ��

	Entry GetEntry(LexTable& lextable, int n);	//�������� ������ ��
	void Delete(LexTable& lextable);			//������� ��

	void ShowLT(LexTable& lextable);
	void WriteLT(LexTable& lextable);
}