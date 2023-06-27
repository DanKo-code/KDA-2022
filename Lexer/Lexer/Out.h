#pragma once
#include "Parm.h"
#include "In.h"
#include <fstream>

using namespace std;

namespace Out
{
	struct OUT
	{
		wchar_t outfile[PARM_MAX_SIZE];
		ofstream* stream;
	};

	static const OUT INITOUT{ L"", NULL };

	OUT getout(wchar_t logfile[]);
	void WriteOut(OUT out, In::IN in);

	/*void WriteError(OUT out, Error::ERROR error);
	void Close(OUT out);*/
}