#include "GRB.h"

#include <string>

#define GRB_ERROR_SERIES 200
#define NS(n) GRB::Rule::Chain::N(n)
#define TS(n) GRB::Rule::Chain::T(n)
#define ISNS(n)	GRB::Rule::Chain::isN(n)
namespace GRB {
	Greibach greibach(
		NS('S'),
		TS('$'),
		8,
		Rule(NS('S'), GRB_ERROR_SERIES + 0, //неверная структура программы (функции и их содежимое)/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			7,

			//специально для void
			Rule::Chain(12, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), TS(';'), TS('}'), NS('S')),

			//специально для void
			Rule::Chain(11, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), TS(';'), TS('}')),

			Rule::Chain(13, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), NS('S')),

			Rule::Chain(12, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), TS('r'), NS('E'), TS(';'), TS('}'), NS('S')),

			Rule::Chain(11, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), TS('r'), NS('E'), TS(';'), TS('}')),
			
			Rule::Chain(12, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}')),

			Rule::Chain(4, TS('m'), TS('{'), NS('N'), TS('}'))
		),
		Rule(NS('N'), GRB_ERROR_SERIES + 1, //Операторы программы/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			14,

			Rule::Chain(5, TS('v'), TS('t'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(4, TS('v'), TS('t'), TS('i'), TS(';')),
			
			//цикл
			Rule::Chain(9, TS('w'), TS('('), TS('i'), TS(')'), TS('{'), NS('A'), TS('}'), TS(';'), NS('N')),
			Rule::Chain(8, TS('w'), TS('('), TS('i'), TS(')'), TS('{'), NS('A'), TS('}'), TS(';')),

			Rule::Chain(9, TS('w'), TS('('), TS('l'), TS(')'), TS('{'), NS('A'), TS('}'), TS(';'), NS('N')),
			Rule::Chain(8, TS('w'), TS('('), TS('l'), TS(')'), TS('{'), NS('A'), TS('}'), TS(';')),


			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
			

			//мб еще вызывать просто функции
			Rule::Chain(6, TS('i'), TS('('), NS('W'), TS(')'), TS(';'), NS('N')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), TS(';')),

			//static function
			Rule::Chain(4, TS('p'), NS('P'), TS(';'), NS('N')),
			Rule::Chain(3, TS('p'), NS('P'), TS(';')),

			Rule::Chain(3, TS('b'), TS(';'), NS('N')),
			Rule::Chain(2, TS('b'), TS(';'))
		),
		Rule(NS('E'), GRB_ERROR_SERIES + 2, //выражение/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			10,
			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(2, TS('l'), NS('M')),

			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),

			//Траблы со сложными выражениями, где много скобок

			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M')),

			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),
			
			Rule::Chain(3, TS('c'), NS('P'), NS('M')), //uint
			Rule::Chain(2, TS('c'), NS('P'))
		),
		Rule(NS('M'), GRB_ERROR_SERIES + 3,//выражение с операторами/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			10,
			Rule::Chain(2, TS('+'), NS('E')), //string, uint, 
			Rule::Chain(2, TS('-'), NS('E')), //uint
			Rule::Chain(2, TS('*'), NS('E')), //uint
			Rule::Chain(2, TS('['), NS('E')), //uint, char
			Rule::Chain(2, TS(']'), NS('E')), //uint, char

			Rule::Chain(3, TS('+'), NS('E'), NS('M')),
			Rule::Chain(3, TS('-'), NS('E'), NS('M')),
			Rule::Chain(3, TS('*'), NS('E'), NS('M')),
			Rule::Chain(3, TS('['), NS('E'), NS('M')),
			Rule::Chain(3, TS(']'), NS('E'), NS('M'))
		),
		Rule(NS('F'), GRB_ERROR_SERIES + 4, //параметры функции/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			2,
			Rule::Chain(2, TS('t'), TS('i')),
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F'))
		),
		Rule(NS('W'), GRB_ERROR_SERIES + 5, //подвыражение/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			4,
			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(3, TS('l'), TS(','), NS('W')),
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l'))
		),

		Rule(NS('A'), GRB_ERROR_SERIES + 6, /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			8,
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('A')),
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),

			//мб еще вызывать просто функции(Это только VOID)
			Rule::Chain(6, TS('i'), TS('('), NS('W'), TS(')'), TS(';'), NS('A')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), TS(';')),

			//static function
			Rule::Chain(4, TS('p'), NS('P'), TS(';'), NS('A')),
			Rule::Chain(3, TS('p'), NS('P'), TS(';')),

			Rule::Chain(3, TS('b'), TS(';'), NS('A')),
			Rule::Chain(2, TS('b'), TS(';'))
		),

		//В print и casual могут быть только i/l
		Rule(NS('P'), GRB_ERROR_SERIES + 7,
				2,
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l'))
				)
	);
	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)	//
	{
		nt = new GRBALPHABET[size = psize];
		int* p = (int*)&s;
		for (short i = 0; i < psize; ++i)	// 
			nt[i] = (GRBALPHABET)p[i];
	};

	Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...)	//
	{
		nn = pnn;
		iderror = piderror;
		chains = new Chain[size = psize];
		Chain* p = &c;
		for (int i = 0; i < size; i++)
			chains[i] = p[i];
	};

	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottom, short psize, Rule r, ...)
	{
		startN = pstartN;
		stbottomT = pstbottom;
		rules = new Rule[size = psize];
		Rule* p = &r;
		for (int i = 0; i < size; ++i)
			rules[i] = p[i];
	}

	Greibach getGreibach()
	{
		return greibach;
	}

	short Greibach::getRule(GRBALPHABET pnn, Rule& prule)
	{
		short rc = -1;
		short k = 0;
		while (k < size && rules[k].nn != pnn)
			k++;
		if (k < size)
			prule = rules[rc = k];
		return rc;
	}

	Rule Greibach::getRule(short n)
	{
		Rule rc;
		if (n < size)
			rc = rules[n];
		return rc;
	};

	char* Rule::getCRule(char* b, short nchain)//получить правило в виде N-> цепочка
	{
		char bchain[200];
		b[0] = Chain::alphabet_to_char(nn);
		b[1] = '-';
		b[2] = '>';
		b[3] = 0x00;
		chains[nchain].getCChain(bchain);
		strcat_s(b, sizeof(bchain) + 5, bchain);
		return b;
	};

	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j)
	{
		short rc = -1;
		while (j < size && chains[j].nt[0] != t)
			++j;
		rc = (j < size ? j : -1);
		if (rc >= 0)
			pchain = chains[rc];
		return rc;
	}

	char* Rule::Chain::getCChain(char* b)//получить правую сторону правила
	{
		for (int i = 0; i < size; i++)
			b[i] = alphabet_to_char(nt[i]);
		b[size] = 0x00;
		return b;
	}
}