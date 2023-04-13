#pragma once
#include "stdafx.h"
#include <fstream>

#define FST_APPEND 7,\
		NODE(1, RELATION('a', 1)),\
		NODE(1, RELATION('p', 2)),\
		NODE(1, RELATION('p', 3)),\
		NODE(1, RELATION('e', 4)),\
		NODE(1, RELATION('n', 5)),\
		NODE(1, RELATION('d', 6)),\
		NODE()

#define FST_EQUALS 7,\
		NODE(1, RELATION('e', 1)),\
		NODE(1, RELATION('q', 2)),\
		NODE(1, RELATION('u', 3)),\
		NODE(1, RELATION('a', 4)),\
		NODE(1, RELATION('l', 5)),\
		NODE(1, RELATION('s', 6)),\
		NODE()

#define FST_CHECK 6,\
		NODE(1, RELATION('c', 1)),\
		NODE(1, RELATION('h', 2)),\
		NODE(1, RELATION('e', 3)),\
		NODE(1, RELATION('c', 4)),\
		NODE(1, RELATION('k', 5)),\
		NODE()

#define FST_ADD 4,\
		NODE(1, RELATION('a', 1)),\
		NODE(1, RELATION('d', 2)),\
		NODE(1, RELATION('d', 3)),\
		NODE()

#define FST_PRINT 6,\
		NODE(1, RELATION('p', 1)),\
		NODE(1, RELATION('r', 2)),\
		NODE(1, RELATION('i', 3)),\
		NODE(1, RELATION('n', 4)),\
		NODE(1, RELATION('t', 5)),\
		NODE()

#define FST_LOOP 5,\
		NODE(1, RELATION('l', 1)),\
		NODE(1, RELATION('o', 2)),\
		NODE(1, RELATION('o', 3)),\
		NODE(1, RELATION('p', 4)),\
		NODE()

#define FST_SEMICOLON 2,\
		NODE(1, RELATION(';', 1)),\
		NODE()

#define FST_COLON 2,\
		NODE(1, RELATION(':', 1)),\
		NODE()

#define FST_COMMA 2,\
		NODE(1, RELATION(',', 1)),\
		NODE()

#define FST_DOT 2,\
		NODE(1, RELATION('.', 1)),\
		NODE()

#define FST_PLUS 2,\
		NODE(1, RELATION('+', 1)),\
		NODE()

#define FST_MINUS 2,\
		NODE(1, RELATION('-', 1)),\
		NODE()

#define FST_STAR 2,\
		NODE(1, RELATION('*', 1)),\
		NODE()

#define FST_DIRSLASH 2,\
		NODE(1, RELATION('/', 1)),\
		NODE()

#define FST_MODULO 2,\
		NODE(1, RELATION('%', 1)),\
		NODE()

#define FST_IS 2,\
		NODE(1, RELATION('=', 1)),\
		NODE()

#define FST_INTERROGATION 2,\
		NODE(1, RELATION('?', 1)),\
		NODE()


namespace FST
{
	struct RELATION
	{
		char symbol;
		short nnode;
		RELATION(char c = 0x00, short ns = NULL);
	};

	struct NODE
	{
		short n_relation;
		RELATION* relations;
		NODE();
		NODE(short n, RELATION rel, ...);
	};

	struct FST
	{
		char* string;
		short position;
		short nstates;
		NODE* nodes;
		short* rstates;
		FST(char* s, short ns, NODE n, ...);
	};

	bool step(FST& fst, short*& rstates);
	bool execute(FST& fst);
	char called(char mas[]);
}