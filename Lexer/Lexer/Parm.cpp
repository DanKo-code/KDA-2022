#include "Error.h"
#include "Parm.h"
#include <iostream>


namespace Parm
{
	PARM getparm(int argc, _TCHAR* argv[])
	{
		setlocale(LC_CTYPE, "Russian");

		Parm::PARM temp{ false, false, false };

		for (int i = 0; i < argc; i++)
		{
			if (wcslen(argv[i]) > PARM_MAX_SIZE) throw ERROR_THROW(2);

			if (wcsstr(argv[i], PARM_IN))
			{
				wcscpy_s(temp.in, argv[i] + wcslen(PARM_IN));
				continue;
			}

			if (wcsstr(argv[i], PARM_OUT))
			{
				wcscpy_s(temp.out, argv[i] + wcslen(PARM_OUT));
				continue;
			}

			if (wcsstr(argv[i], PARM_LOG))
			{
				wcscpy_s(temp.log, argv[i] + wcslen(PARM_LOG));
				continue;
			}
		}

		if (!temp.in[0]) throw ERROR_THROW(3);

		if (!temp.out[0])
		{
			wcscpy_s(temp.out, temp.in);

			wcscat_s(temp.out, PARM_OUT_DEFAULT_EXT);
		}

		if (!temp.log[0])
		{
			wcscpy_s(temp.log, temp.in);
			wcscat_s(temp.log, PARM_LOG_DEFAULT_EXT);
		}


		return temp;
	}
}