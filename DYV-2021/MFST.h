#pragma once
#include "Error.h"
#include "stdafx.h"
#include "GRB.h"
#include "LT.h"
#include <stack>


#define MFST_DIAGN_MAXSIZE 2*ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3

#define NS(n) GRB::Rule::Chain::N(n)
#define TS(n) GRB::Rule::Chain::T(n)
#define ISNS(n) GRB::Rule::Chain::isN(n)
#define MFST_TRACE1 stream << std::setw(4) << std::left << ++FST_TRACE_n << ": "\
									<<std::setw(20)<<std::left<<rule.getCRule(rbuf,nrulechain)\
									<<std::setw(30)<<std::left<<getCLenta(lbuf,lenta_position)\
									<<std::setw(20)<<std::left<<getCSt(sbuf)\
									<<std::endl;
#define MFST_TRACE2 stream << std::setw(4) << std::left << FST_TRACE_n << ": "\
									<<std::setw(20)<<std::left<<" "\
									<<std::setw(30)<<std::left<<getCLenta(lbuf,lenta_position)\
									<<std::setw(20)<<std::left<<getCSt(sbuf)\
									<<std::endl;
#define MFST_TRACE3 stream << std::setw(4) << std::left << ++FST_TRACE_n << ": "\
									<<std::setw(20)<<std::left<<" "\
									<<std::setw(30)<<std::left<<getCLenta(lbuf,lenta_position)\
									<<std::setw(20)<<std::left<<getCSt(sbuf)\
									<<std::endl;

#define MFST_TRACE4(c)   stream <<std::setw(4)<<std::left << ++FST_TRACE_n << ": " << std::setw(20)<<std::left<<c<<std::endl;
#define MFST_TRACE5(c)   stream <<std::setw(4)<<std::left << FST_TRACE_n << ": " << std::setw(20)<<std::left<<c<<std::endl;
#define MFST_TRACE6(c,k) stream <<std::setw(4)<<std::left << FST_TRACE_n << ": " << std::setw(20)<<std::left<<c<<k<<std::endl;
#define MFST_TRACE7		 stream <<std::setw(4)<<std::left<<state.lenta_position<<": "\
												<<std::setw(20)<<std::left<<rule.getCRule(rbuf,state.nrulechain)\
												<<std::endl;


#define MFST_TRACE_START stream << std::setw( 4)<<std::left<<"Step"<< ":"\
								   << std::setw(20)<<std::left<<" Rule" \
								   << std::setw(20)<<std::left<<" Entrance tape" \
								   << std::setw(20)<<std::left<<" Stack" \
								   << std::endl;



class MFSTSTSTACK :public std::stack<short>
{
public:
	using std::stack<short>::c;
};


namespace MFST
{
	struct MfstState
	{
		short lenta_position;
		short nrule;
		short nrulechain;
		MFSTSTSTACK st;
		MfstState();
		MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain);
		MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain);
	};

	class MFSTSTATE : public std::stack<MfstState>
	{
	public:
		using std::stack<MfstState>::c;
	};

	struct Mfst
	{
		enum RC_STEP
		{
			NS_OK,
			NS_NORULE,
			NS_NORULECHAIN,
			NS_ERROR,
			TS_OK,
			TS_NOK,
			LENTA_END,
			SURPRISE
		};

		struct MfstDiagnosis
		{
			short lenta_position;
			RC_STEP rc_step;
			short nrule;
			short nrule_chain;
			MfstDiagnosis();
			MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain);
		}diagnosis[MFST_DIAGN_NUMBER];

		GRBALPHABET* lenta;
		short lenta_position;
		short nrule;
		short nrulechain;
		short lenta_size;
		GRB::Greibach grebach;
		LT::LexTable lex;
		MFSTSTSTACK st;
		MFSTSTATE storestate;
		Mfst();
		Mfst(LT::LexTable plex, GRB::Greibach pgrebach);

		char* getCSt(char* buf);
		char* getCLenta(char* buf, short pos, short n = 25);
		char* getDiagnosis(short n, char* buf);
		bool savestate();
		bool reststate();
		bool push_chain(GRB::Rule::Chain chain);
		RC_STEP step();
		bool start();
		bool savediagnosis(RC_STEP pprc_step);
		void printrules();

		struct Deducation
		{
			short size;
			short* nrules;
			short* nrulechains;
			Deducation() { size = 0; nrules = 0; nrulechains = 0; };
		}deducation;
		bool savededucation();
	};
}