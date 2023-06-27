#include "LT.h"
#include "Error.h"
#include <fstream>

using namespace debugFilePrint;

namespace LT
{
	

	LexTable Create(int size)
	{
		if (size > LT_MAXSIZE) throw ERROR_THROW(100);
		LexTable tableLex;
		tableLex.table = new Entry[size];
		tableLex.maxsize = size;
		tableLex.size = NULL;
		return tableLex;
	}

	void Add(LexTable& lextable, Entry entry)
	{
		if (lextable.size >= lextable.maxsize) throw ERROR_THROW(101);
		lextable.table[lextable.size++] = entry;
	}

	Entry GetEntry(LexTable& lextable, int n)
	{
		if (n > lextable.maxsize)
		{
			throw ERROR_THROW(106)
		}
		return lextable.table[n];
	}

	void PrintLexTable(const char* in, LexTable& lextable)
	{
		File _(in);

		 _._("--------- Таблица лексем ---------");

		 size_t str_numb = 1;
		 for (size_t i = 0; i < lextable.size; i++)
		 {
			 _._('\n'); _._(lextable.table[i].sn); _._(".\t");

			 if (str_numb != lextable.table[i + 1].sn)
			 {

				 //++str_numb;

				 str_numb = lextable.table[i+1].sn;

				 if (lextable.table[i].idxTI == LT_TI_NULLIDX)
					 _._(lextable.table[i].lexema);
				 else
				 {
					 _._(" "); _._(lextable.table[i].lexema); _._("_");  _._(lextable.table[i].idxTI); _._(" ");
				 }
					 

				 continue;
			 }

			 for (; str_numb == lextable.table[i].sn; i++)
			 {
				 if (lextable.table[i].idxTI == LT_TI_NULLIDX)
					 _._(lextable.table[i].lexema);
				 else
				 {
					 _._(" "); _._(lextable.table[i].lexema); _._("_"); _._(lextable.table[i].idxTI); _._(" ");
				 }
					
			 }

			 //++str_numb;

			 str_numb = lextable.table[i].sn;

			 --i;
		 }

		 /*_._("\n\n--------- Таблица лексем(полная) ---------\n");

		 for (size_t i = 0; i < lextable.size; i++)
		 {
			 _._(i); _._(".\t");
			 _._("сокращенная лексема"); _._("\t"); _._("номер строки"); _._("\t"); _._("индекс в ТИ"); _._("\t"); _._("лексема"); _._("\n");
			 _._("\t"); _._(lextable.table[i].lexema); _._("\t\t\t");
			 _._(lextable.table[i].sn); _._("\t\t");
			 _._(lextable.table[i].idxTI); _._("\t\t");
			 _._(lextable.table[i].tekst);;

			 _._("\n"); _._("\n");
		 }*/
	}

	void Delete(LexTable& lextable)
	{
		delete[] lextable.table;
	}
}