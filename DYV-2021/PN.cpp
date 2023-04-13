#include "stdafx.h"
#include "LT.h"
#include "PN.h"
#include <stack>
#include <queue>

using namespace std;
namespace PN
{
	bool PolishNotation(int i, LT::LexTable& lex, IT::IdTable idtable)
	{
		stack<LT::Entry> stack;
		queue<LT::Entry> queue;
		LT::Entry temp;
		temp.idxProp = -1;
		temp.lexema[0] = ' ';
		temp.sn = -1;
		LT::Entry func;
		func.lexema[0] = '^';
		LT::Entry num;
		num.lexema[0] = ' ';
		int countLex = 0;
		int posLex = i;
		bool findFunc = false;
		int buf = 0;

		for (i; lex.table[i].lexema[0] != LEX_SEMICOLON; i++, countLex++)
		{
			switch (lex.table[i].lexema[0])
			{
			case LEX_LITERAL:
			{
				queue.push(lex.table[i]);
				continue;
			}
			case LEX_COMMA: case LEX_OBJECT: case LEX_PROPERTY: case LEX_ADD:
			case LEX_APPEND: case LEX_CHECK: case LEX_GLOBAL: case LEX_LOOP: case LEX_PRINT:
			{
				queue.push(lex.table[i]);
				continue;
			}
			case LEX_PLUS: case LEX_MINUS: case LEX_STAR: case LEX_DIRSLASH: case LEX_MODULO:
			{
				while (!stack.empty() && lex.table[i].priority <= stack.top().priority) 
				{
					queue.push(stack.top());
					stack.pop();
				}
				stack.push(lex.table[i]);
				continue;
			}
			}
		}
		while (!stack.empty())
		{
			queue.push(stack.top());
			stack.pop();
		}
		while (countLex != 0)
		{
			if (!queue.empty()) {
				lex.table[posLex++] = queue.front();
				queue.pop();
			}
			else
			{
				lex.table[posLex++] = temp;
			}
			countLex--;
		}
		return true;
	}

	void PolishStart(LT::LexTable& lex, IT::IdTable& idtable)
	{
		for (int i = 0; i < lex.size; i++)
		{
			if (lex.table[i].lexema[0] == '=')
			{
				PolishNotation(i + 1, lex, idtable);
			}
		}
	}
}