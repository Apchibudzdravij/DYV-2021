#include "stdafx.h"
#include <locale>
#include <cwchar>

#include "Error.h"
#include "Parm.h"
#include "Log.h"
#include "In.h"
#include "FST.h"
#include "IT.h"
#include "LT.h"
#include "Lex.h"
#include "LitT.h"
#include "MFST.h"
#include "GRB.h"
#include "PN.h"
#include "gen.h"

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, ".1251");
	Log::LOG log = Log::INITLOG;
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		In::IN in = In::getin(parm.in);
		Log::WriteIn(log, in);
		LT::LexTable myOwnLexTable = LT::Create(65536);
		LEX::LexList myOwnLexList = LEX::Create(65536, myOwnLexTable);
		IT::IdTable myOwnIdTable = IT::Create(65536);
		LitT::LitTable myLiterals = LitT::Create(65536);

		LEX::CreateFromFile(myOwnLexList, myOwnLexTable, myOwnIdTable, myLiterals);

		LEX::SemanticCheck(myOwnLexList, myOwnLexTable, myOwnIdTable, myLiterals);

		MFST::Mfst mfst(myOwnLexTable, GRB::getGreibach());
		mfst.start();
		mfst.printrules();

		IT::IdTable mySmoothedIdTable = IT::Smooth(myOwnIdTable);
		LT::LexTable mySmoothedLexTable = LEX::LexSmooth(myOwnLexTable, myOwnIdTable);
		PN::PolishStart(myOwnLexTable, myOwnIdTable);
		LT::ShowLT(myOwnLexTable);

		LT::WriteLT(myOwnLexTable);
		IT::WriteIT(mySmoothedIdTable);
		LitT::WriteLT(myLiterals);

		Gen::CodeGeneration(mySmoothedLexTable, myOwnLexTable, mySmoothedIdTable, myLiterals, myOwnLexList);

		Log::Close(log);
		LT::Delete(myOwnLexTable);
		LEX::Delete(myOwnLexList);
		IT::Delete(myOwnIdTable);
		LitT::Delete(myLiterals);

	}
	catch (Error::ERROR e)
	{
		Log::WriteError(log, e);
	};

	system("pause");
	return 0;
}