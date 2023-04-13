#pragma once

#include"LT.h"
#include "IT.h"
#include "LitT.h"
#include "Lex.h"
#include <vector>


#define SEPSTREMP  "\n;------------------------------\n"
#define SEPSTR(x)  "\n;----------- " + std::string(x) + " ------------\n"


#define BEGIN ".586\n"\
".model flat, stdcall\n"\
"includelib libucrt.lib\n"\
"includelib kernel32.lib\n"\
"includelib \"..\Debug\StaticLib2.lib\"\n"\
//"includelib \"\""\

#define EXTERN  ".stack 65536\n"\
"ExitProcess PROTO:DWORD \n"\
"\n outnumline PROTO : DWORD\n"\
" outstrline PROTO : DWORD\n"\
" power PROTO : DWORD\n"\
" strcomp PROTO : DWORD\n"\
" pausese PROTO :\n"\
" input PROTO : DWORD\n"\
" _ConvertToChar PROTO : DWORD\n"\

#define END "push 0\ncall pausese\npush 0\ncall ExitProcess\nmain ENDP\nend main"

#define ITENTRY(x)  idtable.table[lextable.table[x].idxProp]
#define LEXEMA(x)   lextable.table[x].lexema[0]


#define CONST "\n.const\n\tprintline byte 13, 10, 0"
#define DATA "\n.data\n\ttemp sdword ?\n\tbuffer byte 256 dup(0)\n"
#define CODE "\n.code\nmain PROC\n"

namespace Gen
{
	void CodeGeneration(LT::LexTable& smoothtable, LT::LexTable& lextable, IT::IdTable& idtable, LitT::LitTable& littable, LEX::LexList& lexlist);
};