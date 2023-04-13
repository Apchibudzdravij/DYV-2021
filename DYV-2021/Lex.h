#pragma once
#include "LT.h"
#include "IT.h"

namespace LEX
{
	struct LexEntry
	{
		LT::Entry* entry;
		//IT::ObjTable* objt;
		IT::PropTable* propt;
	};

	struct LexList
	{
		LT::LexTable ltable;
		int size;
		int maxsize;
		LexEntry* table = new LexEntry[65536];
		//LexList() {};
	};

	LexList Create(int size, LT::LexTable& lt);					//создать ТЛ, зная ёмкость
	void Add(LexList& lextable, LexEntry entry);	//добавить строку в ТЛ
	void Delete(LexList& lextable);
	void CreateFromFile(LEX::LexList& lexlist, LT::LexTable& lextable, IT::IdTable& idtable, LitT::LitTable& littable);
	void SemanticCheck(LEX::LexList& lexlist, LT::LexTable& lextable, IT::IdTable& idtable, LitT::LitTable& littable);
	std::string AddToName(LT::LexTable& lextable, int i);
	LT::LexTable LexSmooth(LT::LexTable& lextable, IT::IdTable& idtable);
	int FindInSmoothed(LT::LexTable& lextable, char* name);
	int FindInNormal(LT::LexTable& lextable, char* name);
}