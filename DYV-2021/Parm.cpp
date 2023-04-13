#include "stdafx.h"
#include <wchar.h>
#include "Parm.h"
#include "Error.h"
using namespace std;
namespace Parm
{
	PARM getparm(int argc, _TCHAR* argv[])
	{
		setlocale(LC_ALL, "RUS");
		PARM param = {};
		wchar_t str[900];
		wchar_t fullstr[900] = L"";
		wchar_t founded[900] = L"";
		wchar_t findi[] = L"-in:";
		wchar_t findo[] = L"-out:";
		wchar_t findl[] = L"-log:";
		wchar_t space[] = L" ";
		for (int i = 1; i < argc; i++)
		{
			wcscpy_s(str, argv[i]);
			wcsncat_s(fullstr, str, PARM_MAX_SIZE);
			wcsncat_s(fullstr, L" ", PARM_MAX_SIZE);
		}
		if (wcsstr(fullstr, findi) == NULL)
		{
			throw ERROR_THROW(2);
		}
		else
		{
			int counter = 0;
			wcscpy_s(founded, wcsstr(fullstr, findi));
			for (int i = 4, j = 0; i < std::wcslen(founded); i++, j++)
			{
				counter++;
				if (founded[i + 1] == space[0]) break;
			}
			if ((counter + 4) >= PARM_MAX_SIZE)
			{
				throw ERROR_THROW(3);
			}
			wcscpy_s(founded, wcsstr(fullstr, findi));
			for (int i = 4, j = 0; i < std::wcslen(founded); i++, j++)
			{
				param.in[j] = founded[i];
				if (founded[i + 1] == space[0]) break;
			}
		}
		if (wcsstr(fullstr, findo) == NULL)	//out
		{
			wcsncat_s(param.out, param.in, PARM_MAX_SIZE);
			wcsncat_s(param.out, L".out.txt", PARM_MAX_SIZE);
		}
		else
		{
			int counter = 0;
			wcscpy_s(founded, wcsstr(fullstr, findo));
			for (int i = 5, j = 0; i < std::wcslen(founded); i++, j++)
			{
				counter++;
				if (founded[i + 1] == space[0]) break;
			}
			if ((counter + 5) >= PARM_MAX_SIZE)
			{
				throw ERROR_THROW(3);
			}
			wcscpy_s(founded, wcsstr(fullstr, findo));
			for (int i = 5, j = 0; i < std::wcslen(founded); i++, j++)
			{
				param.out[j] = founded[i];
				if (founded[i + 1] == space[0]) break;
			}
		}
		if (wcsstr(fullstr, findl) == NULL)	//log
		{
			wcsncat_s(param.log, param.in, PARM_MAX_SIZE);
			wcsncat_s(param.log, L".log", PARM_MAX_SIZE);
		}
		else
		{
			int counter = 0;
			wcscpy_s(founded, wcsstr(fullstr, findl));
			for (int i = 5, j = 0; i < std::wcslen(founded); i++, j++)
			{
				counter++;
				if (founded[i + 1] == space[0]) break;
			}
			if ((counter + 5) >= PARM_MAX_SIZE)
			{
				throw ERROR_THROW(3);
			}
			wcscpy_s(founded, wcsstr(fullstr, findl));
			for (int i = 5, j = 0; i < std::wcslen(founded); i++, j++)
			{
				param.log[j] = founded[i];
				if (founded[i + 1] == space[0]) break;
			}
		}
		return param;
	}
}