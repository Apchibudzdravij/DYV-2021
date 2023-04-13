#include "MFST.h"
#include "GRB.h"
#include "stdafx.h"
#include "LT.h"
#include <fstream>

int FST_TRACE_n = -1;
char rbuf[205], sbuf[205], lbuf[1024];



namespace MFST 
{
	MfstState::MfstState()
	{
		lenta_position = 0;
		nrule = -1;
		nrulechain = -1;
	};
	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrulechain)
	{
		lenta_position = pposition;
		st = pst;
		nrulechain = pnrulechain;
	};
	MfstState::MfstState(short pposition, MFSTSTSTACK pst, short pnrule, short pnrulechain) 
	{
		lenta_position = pposition;
		st = pst;
		nrule = pnrule;
		nrulechain = pnrulechain;
	}
	
	Mfst::MfstDiagnosis::MfstDiagnosis()
	{
		lenta_position = -1;
		rc_step = SURPRISE;
		nrule = -1;
		nrule_chain = -1;
	};

	Mfst::MfstDiagnosis::MfstDiagnosis(short plenta_position, RC_STEP prc_step, short pnrule, short pnrule_chain) 
	{
		lenta_position = plenta_position;
		rc_step = prc_step;
		nrule = pnrule;
		nrule_chain = pnrule_chain;
	}

	Mfst::Mfst() { lenta = 0; lenta_size = lenta_position = 0; };
	Mfst::Mfst(LT::LexTable plex, GRB::Greibach pgrebach)
	{
		grebach = pgrebach;
		lex = plex;
		lenta = new short[lenta_size = lex.size];
		for (int k = 0; k < lenta_size; k++) lenta[k] = TS(lex.table[k].lexema[0]);
		lenta_position = 0;
		st.push(grebach.stbottomT);
		st.push(grebach.startN);
		nrulechain = -1;
	};

	Mfst::RC_STEP Mfst::step()
	{
		std::ofstream stream;//
		stream.open("MFST.log", std::ios_base::app);
		if (!stream.is_open())
		{
			ERROR_THROW(5)
		}
		RC_STEP rc = SURPRISE;
		if (lenta_position < lenta_size)
		{
			if (ISNS(st.top()))
			{
				GRB::Rule rule;
				if ((nrule = grebach.getRule(st.top(), rule)) >= 0)
				{
					GRB::Rule::Chain chain;
					if ((nrulechain = rule.getNextChain(lenta[lenta_position], chain, nrulechain + 1)) >= 0)
					{
						MFST_TRACE1
							savestate(); st.pop(); push_chain(chain); rc = NS_OK;
						MFST_TRACE2
					}
					else
					{
						MFST_TRACE4("TNS_NORULECHAIN/NS_NORULE")
							savediagnosis(NS_NORULECHAIN); rc = reststate() ? NS_NORULECHAIN : NS_NORULE;
					};
				}
				else rc = NS_ERROR;
			}
			else if ((st.top() == lenta[lenta_position]))
			{
				lenta_position++; st.pop(); nrulechain = -1; rc = TS_OK;
				MFST_TRACE3
			}
			else
			{
				MFST_TRACE4("TS_NOK/NS_NORULECHAIN") rc = reststate() ? TS_NOK : NS_NORULECHAIN;
			};
		}
		else
		{
			rc = LENTA_END; MFST_TRACE4("LENTA_END");
		}
		stream.close();
		return rc;
	};

	bool Mfst::push_chain(GRB::Rule::Chain chain)
	{
		for (int k = chain.size - 1; k >= 0; k--) st.push(chain.nt[k]);
		return true;
	};

	bool Mfst::savestate()
	{
		std::ofstream stream;//
		stream.open("MFST.log", std::ios_base::app);
		if (!stream.is_open())
		{
			ERROR_THROW(5)
		}
		storestate.push(MfstState(lenta_position, st, nrule, nrulechain));
		MFST_TRACE6("SAVESTATE:", storestate.size());
		stream.close();
		return true;
	};

	bool Mfst::reststate()
	{
		std::ofstream stream;//
		stream.open("MFST.log", std::ios_base::app);
		if (!stream.is_open())
		{
			ERROR_THROW(5)
		}
		bool rc = false;
		MfstState state;
		if (rc = (storestate.size() > 0))
		{
			state = storestate.top();
			lenta_position = state.lenta_position;
			st = state.st;
			nrule = state.nrule;
			nrulechain = state.nrulechain;
			storestate.pop();
			MFST_TRACE5("RESSTATE")
			MFST_TRACE2
		};
		stream.close();
		return rc;
	};

	bool Mfst::savediagnosis(RC_STEP prc_step)
	{
		bool rc = false;
		short k = 0;
		while (k < MFST_DIAGN_NUMBER && lenta_position <= diagnosis[k].lenta_position) k++;
		if (rc = (k < MFST_DIAGN_NUMBER))
		{
			diagnosis[k] = MfstDiagnosis(lenta_position, prc_step, nrule, nrulechain);
			for (short j = k + 1; j < MFST_DIAGN_NUMBER; j++) diagnosis[j].lenta_position = -1;
		};
		return rc;
	};

	bool Mfst::start() 
	{
		std::ofstream stream;//
		remove("MFST.log");
		stream.open("MFST.log", std::ios_base::app);
		if (!stream.is_open())
		{
			ERROR_THROW(5)
		}
		MFST_TRACE_START
		bool rc = false;
		RC_STEP rc_step = SURPRISE;
		char buf[MFST_DIAGN_MAXSIZE];
		rc_step = step();
		while (rc_step == NS_OK || rc_step == NS_NORULECHAIN || rc_step == TS_OK || rc_step == TS_NOK) 
		{ rc_step = step(); }
		
		switch (rc_step)
		{
		case LENTA_END: MFST_TRACE4("------>LENTA_END")
			stream << "------------------------------------------------------------------" << std::endl;
			stream << 0 << " всего строк " << lenta_size << ", синтаксический анализ выполнен без ошибок";
			stream << std::setw(4) << std::left << 0 << ": всего лексем " << lenta_size << ", синтаксический анализ выполнен без ошибок" << std::endl;
			rc = true;
			break;
		case NS_NORULE: MFST_TRACE4("------>NS_NORULE")
			stream << "------------------------------------------------------------------" << std::endl;
			stream << getDiagnosis(0, buf) << std::endl;
			stream << getDiagnosis(1, buf) << std::endl;
			stream << getDiagnosis(2, buf) << std::endl;
			break;
		case NS_NORULECHAIN: MFST_TRACE4("------>NS_NORULENORULECHAIN") break;
		case NS_ERROR: MFST_TRACE4("------>NS_ERROR") break;
		case SURPRISE: MFST_TRACE4("------>SURPRISE")break;
		};
		stream.close();
		return rc;
	};
	char* Mfst::getCSt(char* buf) 
	{
		for (int k = (signed)st.size() - 1; k >= 0; --k) 
		{
			short p = st.c[k];
			buf[st.size() - 1 - k] = GRB::Rule::Chain::alphabet_to_char(p);
		}
		buf[st.size()] = 0x00;
		return buf;
	}
	char* Mfst::getCLenta(char* buf, short pos, short n)
	{
		short i, k = (pos + n < lenta_size) ? pos + n : lenta_size;
		for (i = pos; i < k; i++) buf[i - pos] = GRB::Rule::Chain::alphabet_to_char(lenta[i]);
		buf[i - pos] = 0x00;
		return buf;
	};
	char* Mfst::getDiagnosis(short n, char* buf)
	{
		//возможно нужен будет фикс rc из-за выделения памяти ) 
		char* rc = new char[MFST_DIAGN_MAXSIZE];
		int errid = 0;
		int lpos = -1;
		if (n < MFST_DIAGN_NUMBER && (lpos = diagnosis[n].lenta_position) >= 0)
		{
			errid = grebach.getRule(diagnosis[n].nrule).iderror;
			Error::ERROR err = Error::geterror(errid);
			err.id = errid;
			sprintf_s(buf, MFST_DIAGN_MAXSIZE, "%d: line %d, %s", err.id, lex.table[lpos].sn, err.message);
			rc = buf;
		};
		return rc;
	};

	void Mfst::printrules() 
	{
		MfstState state;
		GRB::Rule rule;
		std::ofstream stream;//
		stream.open("MFST.log", std::ios_base::app);
		if (!stream.is_open())
		{
			ERROR_THROW(5)
		}
		for (unsigned short k = 0; k < storestate.size(); k++) 
		{
			state = storestate.c[k];
			rule = grebach.getRule(state.nrule);
			MFST_TRACE7
		}
		stream.close();
	}

	bool Mfst::savededucation() 
	{
		MfstState state;
		GRB::Rule rule;
		deducation.nrules = new short[deducation.size = storestate.size()];
		deducation.nrulechains = new short[deducation.size];
		for (unsigned short k = 0; k < storestate.size(); k++) 
		{
			state = storestate.c[k];
			deducation.nrules[k] = state.nrule;
			deducation.nrulechains[k] = state.nrulechain;
		}
		return true;
	}
};
