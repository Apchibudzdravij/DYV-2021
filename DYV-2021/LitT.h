#pragma once

#define LIT_MAXLEN 128
#define LIT_ERRORLIT ENTRY errlitt = {errlit.literal = "Err_NoPos"; errlit.strNumInLT = -1; errlit.posNumInLT = -1;};

namespace LitT
{
	struct Entry
	{
		char literal[LIT_MAXLEN];	//��������� ���������� ��������
		enum
		{
			INT = 1,
			STR = 2,
			DOU = 3,
			HEX = 4,
		}type;
		int actualSize;
		int strNumInLT;				//��������� ������ � ��
		int posNumInLT;				//��������� ������� � ������ � ��
	};

	struct LitTable
	{
		int maxsize;				//������� ������� ���������
		int size;					//������� ������ ������� ���������
		Entry* table;
	};

	LitTable Create(int size);					//������� �������, ���� �������
	void Add(LitTable& lextable, Entry entry);	//�������� ������ � �������
	Entry GetEntry(LitTable& littable, int str, int pos);	//�������� ������ ������� �� ������ ������ �� � ������� � ���
	void Delete(LitTable& littable);			//������� �������
	void WriteLT(LitTable& littable);
}