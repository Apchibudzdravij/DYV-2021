#include "stdafx.h"
#include "LT.h"
#include "FST.h"

namespace FST
{
	bool step(FST& fst, short*& rstates) {
		bool rc = false;
		std::swap(rstates, fst.rstates);
		for (short i = 0; i < fst.nstates; i++) {
			if (rstates[i] == fst.position)
				for (short j = 0; j < fst.nodes[i].n_relation; j++) {
					if (fst.nodes[i].relations[j].symbol == fst.string[fst.position]) {
						fst.rstates[fst.nodes[i].relations[j].nnode] = fst.position + 1;
						rc = true;
					};
				};
		};
		return rc;
	};

	bool execute(FST& fst) {
		short* rstates = new short[fst.nstates]; memset(rstates, 0xff, sizeof(short) * fst.nstates);
		short lstring = strlen(fst.string);
		bool rc = true;
		for (short i = 0; i < lstring && rc; i++)
		{
			fst.position++;
			rc = step(fst, rstates);
		};
		delete[] rstates;
		return (rc ? (fst.rstates[fst.nstates - 1] == lstring) : rc);
	}
	RELATION::RELATION(char c, short nn) {
		symbol = c;
		nnode = nn;
	}

	NODE::NODE() {
		n_relation = 0;
		RELATION* relations = NULL;
	};

	NODE::NODE(short n, RELATION rel, ...) {
		n_relation = n;
		RELATION* p = &rel;
		relations = new RELATION[n];
		for (short i = 0; i < n; i++) {
			relations[i] = p[i];
		}
	}
	FST::FST(char* s, short ns, NODE n, ...) {
		string = s;
		nstates = ns;
		nodes = new NODE[ns];
		NODE* p = &n;
		for (int k = 0; k < ns; k++) nodes[k] = p[k];
		rstates = new short[nstates];
		memset(rstates, 0xff, sizeof(short) * nstates);
		rstates[0] = 0;
		position = -1;
	}
	
	char called(char mas[])
	{
		std::ofstream stream;
		stream.open("LT.log", std::ios_base::app);

		FST ADD(mas, FST_ADD);
		FST APP(mas, FST_APPEND);
		FST PRI(mas, FST_PRINT);
		FST CHE(mas, FST_CHECK);
		FST LOO(mas, FST_LOOP);
		FST SEM(mas, FST_SEMICOLON);
		FST COL(mas, FST_COLON);
		FST COM(mas, FST_COMMA);
		FST DOT(mas, FST_DOT);
		FST PLU(mas, FST_PLUS);
		FST MIN(mas, FST_MINUS);
		FST STA(mas, FST_STAR);
		FST DIR(mas, FST_DIRSLASH);
		FST MOD(mas, FST_MODULO);
		FST  IS(mas, FST_IS);
		FST IRR(mas, FST_INTERROGATION);
		FST EQU(mas, FST_EQUALS);

		if (execute(ADD))
		{
			stream.put(LEX_ADD);
			stream.close();
			return LEX_ADD;
		}
		else if (execute(APP))
		{
			stream.put(LEX_APPEND);
			stream.close();
			return LEX_APPEND;
		}
		else if (execute(PRI))
		{
			stream.put(LEX_PRINT);
			stream.close();
			return LEX_PRINT;
		}
		else if (execute(CHE))
		{
			stream.put(LEX_CHECK);
			stream.close();
			return LEX_CHECK;
		}
		else if (execute(LOO))
		{
			stream.put(LEX_LOOP);
			stream.close();
			return LEX_LOOP;
		}
		else if (execute(SEM))
		{
			stream.put(LEX_SEMICOLON);
			stream.close();
			return LEX_SEMICOLON;
		}
		else if (execute(COL))
		{
			stream.put(LEX_COLON);
			stream.close();
			return LEX_COLON;
		}
		else if (execute(COM))
		{
			stream.put(LEX_COMMA);
			stream.close();
			return LEX_COMMA;
		}
		else if (execute(PLU))
		{
			stream.put(LEX_PLUS);
			stream.close();
			return LEX_PLUS;
		}
		else if (execute(MIN))
		{
			stream.put(LEX_MINUS);
			stream.close();
			return LEX_MINUS;
		}
		else if (execute(STA))
		{
			stream.put(LEX_STAR);
			stream.close();
			return LEX_STAR;
		}
		else if (execute(DIR))
		{
			stream.put(LEX_DIRSLASH);
			stream.close();
			return LEX_DIRSLASH;
		}
		else if (execute(MOD))
		{
			stream.put(LEX_MODULO);
			stream.close();
			return LEX_MODULO;
		}
		else if (execute(IS))
		{
			stream.put(LEX_IS);
			stream.close();
			return LEX_IS;
		}
		else if (execute(IRR))
		{
			stream.put(LEX_INTERROGATION);
			stream.close();
			return LEX_INTERROGATION;
		}
		else if (execute(EQU))
		{
			stream.put(LEX_EQUALS);
			stream.close();
			return LEX_EQUALS;
		}
		else
		{
			return NULL;
		}
	}
}