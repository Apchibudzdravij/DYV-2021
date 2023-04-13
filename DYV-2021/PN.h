#pragma once
#include "stdafx.h"
#include "LT.h"
#include "IT.h"

namespace PN
{
	bool PolishNotation(int i, LT::LexTable & lex, IT::IdTable idtable);
	void PolishStart(LT::LexTable& lex, IT::IdTable& idtable);
}