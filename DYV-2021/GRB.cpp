#include "GRB.h"
#include "stdafx.h"
#define GRB_ERRORS_SERIES 600
namespace GRB
{
#define NS(n) Rule::Chain::N(n)
#define TS(n) Rule::Chain::T(n)
	Greibach greibach(NS('S'), TS('$'),
		6,
		Rule(NS('S'), GRB_ERRORS_SERIES + 0,
			3,
			Rule::Chain(3, TS('o'), NS('P'), NS('N')),
			Rule::Chain(3, TS('g'), NS('P'), NS('N')),
			Rule::Chain(3, TS('o'), NS('C'), NS('N'))
		),
		Rule(NS('N'), GRB_ERRORS_SERIES + 1,
			7,
			Rule::Chain(1, TS(';')),
			Rule::Chain(4, TS(';'), TS('o'), NS('P'), NS('N')),
			Rule::Chain(4, TS(';'), TS('o'), NS('C'), NS('N')),
			Rule::Chain(4, TS(';'), TS('g'), NS('P'), NS('N')),
			Rule::Chain(3, TS('o'), NS('C'), NS('N')),
			Rule::Chain(3, TS('o'), NS('P'), NS('N')),
			Rule::Chain(3, TS('g'), NS('P'), NS('N'))
		),
		Rule(NS('P'), GRB_ERRORS_SERIES + 2,
			10,
			Rule::Chain(1, TS('p')),
			Rule::Chain(1, TS('s')),
			Rule::Chain(2, TS('&'), TS('p')),
			Rule::Chain(2, TS('p'), NS('P')),
			Rule::Chain(2, TS('p'), NS('C')),
			Rule::Chain(3, TS('p'), TS(','), NS('O')),
			Rule::Chain(3, TS('n'), TS('='), NS('O')),
			Rule::Chain(3, TS('f'), TS('='), NS('L')),
			Rule::Chain(5, TS('c'), TS('='), NS('O'), TS(','), NS('O')),
			Rule::Chain(6, TS('e'), TS('?'), NS('S'), TS(':'), NS('S'), TS(':'))
		),
		Rule(NS('O'), GRB_ERRORS_SERIES + 3,
			10,
			Rule::Chain(1, TS('o')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(1, TS('g')),
			Rule::Chain(2, TS('o'), NS('P')),
			Rule::Chain(2, TS('o'), NS('C')),
			Rule::Chain(2, TS('g'), NS('P')),
			Rule::Chain(3, TS('o'), TS(','), NS('L')),
			Rule::Chain(3, TS('l'), TS(','), NS('L')),
			Rule::Chain(3, TS('o'), NS('P'), NS('N')),
			Rule::Chain(4, TS('o'), NS('P'), TS(','), NS('L'))
		),
		Rule(NS('L'), GRB_ERRORS_SERIES + 4,
			3,
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('l'), TS(','), NS('L')),
			Rule::Chain(3, TS('l'), TS(','), NS('O'))
		),
		Rule(NS('C'), GRB_ERRORS_SERIES + 5,
			6,
			Rule::Chain(2, TS('='), NS('O')),
			Rule::Chain(2, TS('+'), NS('O')),
			Rule::Chain(2, TS('-'), NS('O')),
			Rule::Chain(2, TS('*'), NS('O')),
			Rule::Chain(2, TS('/'), NS('O')),
			Rule::Chain(2, TS('%'), NS('O'))
		)
	);


	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)
	{
		nt = new GRBALPHABET[size = psize];
		int* p = (int*)&s;
		for (short i = 0; i < psize; ++i) nt[i] = (GRBALPHABET)p[i];
	};
	Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...)
	{
		nn = pnn;
		iderror = piderror;
		chains = new Chain[size = psize];
		Chain* p = &c;
		for (int i = 0; i < size; i++) chains[i] = p[i];
	};
	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottom, short psize, Rule r, ...)
	{
		startN = pstartN;
		stbottomT = pstbottom;
		rules = new Rule[size = psize];
		Rule* p = &r;
		for (int i = 0; i < size; i++) rules[i] = p[i];
	};

	Greibach getGreibach() { return greibach; };

	short Greibach::getRule(GRBALPHABET pnn, Rule& prule)
	{
		short rc = -1;
		short k = 0;
		while (k < size && rules[k].nn != pnn) k++;
		if (k < size) prule = rules[rc = k];
		return rc;
	};

	Rule Greibach::getRule(short n)
	{
		Rule rc;
		if (n < size) rc = rules[n];
		return rc;
	};

	char* Rule::getCRule(char* b, short nchain)
	{
		char bchain[200];
		b[0] = Chain::alphabet_to_char(nn); b[1] = '-'; b[2] = '>'; b[3] = 0x00;
		chains[nchain].getCChain(bchain);
		strcat_s(b, sizeof(bchain) + 5, bchain);
		return b;
	};
	
	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j)
	{
		short rc = -1;
		while (j < size && chains[j].nt[0] != t) ++j;
		rc = (j < size ? j: -1);
		if (rc >= 0) pchain = chains[rc];
		return rc;
	};

	char* Rule::Chain::getCChain(char* b)
	{
		for (int i = 0; i < size; i++) b[i] = Chain::alphabet_to_char(nt[i]);
		b[size] = 0x00;
		return b;
	};

};