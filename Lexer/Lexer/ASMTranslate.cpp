#include "ASMTranslate.h"

#define CODE_MAIN_FUNCTION_START _._("\n.code\n");\
								 _._("main PROC\n\n");

#define MAIN_FUNCTION_END \
						  _._("\n\nINVOKE ExitProcess, 0\n");\
															 \
						  _._("error_les_zero:\n");\
						  _._("CALL print_error_les_zero\n");\
						  _._("INVOKE ExitProcess, 1\n");\
														      \
						  _._("error_str_literal_length_exceeded:\n");\
					      _._("CALL print_error_str_literal_length_exceeded\n"); \
						  _._("INVOKE ExitProcess, 2\n"); \
															  \
						  _._("main ENDP\n\n\n");

#define MAIN_PROCESS_END _._("END MAIN\n");

#define UINT_PLUS _._("POP"); _._(" "); _._("EDX\n");\
				  _._("POP"); _._(" "); _._("EBX\n");\
				  _._("ADD"); _._(" "); _._("EBX"); _._(","); _._("EDX\n");\
				  _._("PUSH"); _._(" "); _._("EBX\n");

#define UINT_MINUS _._("POP"); _._(" "); _._("EDX\n");\
				   _._("POP"); _._(" "); _._("EBX\n");\
				   _._("CMP"); _._(" "); _._("EBX"); _._(","); _._("EDX\n");\
				   _._("JB"); _._(" "); _._("error_les_zero\n");\
				   _._("SUB"); _._(" "); _._("EBX"); _._(","); _._("EDX\n");\
				   _._("PUSH"); _._(" "); _._("EBX\n");

#define UINT_STAR _._("POP"); _._(" "); _._("EDX\n");\
				  _._("POP"); _._(" "); _._("EBX\n");\
				  _._("IMUL"); _._(" "); _._("EBX"); _._(","); _._("EDX\n");\
				  _._("PUSH"); _._(" "); _._("EBX\n");

#define STRING_PLUS _._("POP"); _._(" "); _._("EDX\n");\
				    _._("POP"); _._(" "); _._("EBX\n");\
					_._("INVOKE"); _._(" "); _._("strconcat"); _._(","); _._("EBX, EDX\n");\
					_._("PUSH"); _._(" "); _._("EAX\n");

#define CHECK_STRING_PLUS _._("POP"); _._(" "); _._("EDX\n");\
						  _._("POP"); _._(" "); _._("EBX\n");\
						  _._("INVOKE"); _._(" "); _._("checkstringconcat"); _._(","); _._("EBX, EDX\n");\
					      _._("CMP"); _._(" "); _._("EAX"); _._(","); _._("0\n");\
						  _._("JE"); _._(" "); _._("error_str_literal_length_exceeded\n");\
						

#define UINT_L_LOG_SHIFT _._("POP"); _._(" "); _._("EDX\n");\
						 _._("POP"); _._(" "); _._("EBX\n");\
						 _._("SHL"); _._(" "); _._("EBX"); _._(","); _._("EDX\n");\
						 _._("PUSH"); _._(" "); _._("EAX\n");

#define UINT_R_LOG_SHIFT _._("POP"); _._(" "); _._("EDX\n");\
						 _._("POP"); _._(" "); _._("EBX\n");\
						 _._("SHR"); _._(" "); _._("EBX"); _._(","); _._("EDX\n");\
						 _._("PUSH"); _._(" "); _._("EAX\n");

#define CHAR_L_LOG_SHIFT  _._("POP"); _._(" "); _._("EDX\n");\
						  _._("POP"); _._(" "); _._("EBX\n");\
					      _._("INVOKE"); _._(" "); _._("shiftChar"); _._(","); _._("OFFSET charLeftShift, EBX, EDX\n");\
						  _._("PUSH"); _._(" "); _._("EAX\n");


#define CHAR_R_LOG_SHIFT _._("POP"); _._(" "); _._("EDX\n");\
						 _._("POP"); _._(" "); _._("EBX\n");\
					     _._("INVOKE"); _._(" "); _._("shiftChar"); _._(","); _._("OFFSET charRightShift, EDX, EBX\n");\
						 _._("PUSH"); _._(" "); _._("EAX\n");


namespace Translation
{
	LT::LexTable LT;
	IT::IdTable IT;


	//Вспомогательные для .CODE
	void printHandle(size_t printPosition, debugFilePrint::File& _)
	{
		_._(";---Распечатка---\n");

		switch (IT.table[LT.table[printPosition + 1].idxTI].iddatatype)
		{
			case IT::IDDATATYPE::UINT:
			{
				if (IT.table[LT.table[printPosition + 1].idxTI].idtype == IT::IDTYPE::L)
				{
					_._("PUSH"); _._(" "); _._(IT.table[LT.table[printPosition + 1].idxTI].id); _._("\n");
					_._("CALL"); _._(" "); _._("printUint\n");
				}

				else if (IT.table[LT.table[printPosition + 1].idxTI].idtype == IT::IDTYPE::V || 
					IT.table[LT.table[printPosition + 1].idxTI].idtype == IT::IDTYPE::P)
				{
					_._("PUSH"); _._(" "); _._(IT.table[LT.table[printPosition + 1].idxTI].visibility); _._("@");
					_._(IT.table[LT.table[printPosition + 1].idxTI].id); _._("\n");

					_._("CALL"); _._(" "); _._("printUint\n");
				}

				break;
			}

			/*case IT::IDDATATYPE::CHAR:
			{
				if (IT.table[LT.table[printPosition + 1].idxTI].idtype == IT::IDTYPE::L)
				{
					_._("PUSH"); _._(" "); _._(IT.table[LT.table[printPosition + 1].idxTI].id); _._("\n");
					_._("CALL"); _._(" "); _._("printChar\n");
				}

				else if (IT.table[LT.table[printPosition + 1].idxTI].idtype == IT::IDTYPE::V ||
					IT.table[LT.table[printPosition + 1].idxTI].idtype == IT::IDTYPE::P)
				{
					_._("PUSH"); _._(" "); _._(IT.table[LT.table[printPosition + 1].idxTI].visibility); _._("@");
					_._(IT.table[LT.table[printPosition + 1].idxTI].id); _._("\n");

					_._("CALL"); _._(" "); _._("printChar\n");
				}

				break;
			}*/

			case IT::IDDATATYPE::STR:
			case IT::IDDATATYPE::CHAR:
			{
				if (IT.table[LT.table[printPosition + 1].idxTI].idtype == IT::IDTYPE::L)
				{
					_._("PUSH"); _._(" "); _._("OFFSET"); _._(" "); _._(IT.table[LT.table[printPosition + 1].idxTI].id); _._("\n");
					_._("CALL"); _._(" "); _._("printString\n");
				}

				else if (IT.table[LT.table[printPosition + 1].idxTI].idtype == IT::IDTYPE::V ||
					IT.table[LT.table[printPosition + 1].idxTI].idtype == IT::IDTYPE::P)
				{
					_._("PUSH"); _._(" "); _._("OFFSET"); _._(" "); _._(IT.table[LT.table[printPosition + 1].idxTI].visibility); _._("@");
					_._(IT.table[LT.table[printPosition + 1].idxTI].id); _._("\n");

					_._("CALL"); _._(" "); _._("printString\n");
				}

				break;
			}
		}
	}

	void cycleStart(size_t startCyclePosition, debugFilePrint::File& _)
	{
		if (IT.table[LT.table[startCyclePosition + 2].idxTI].idtype == IT::IDTYPE::L)
		{
			_._("MOV"); _._(" "); _._("ECX"); _._(", "); _._(IT.table[LT.table[startCyclePosition + 2].idxTI].id); _._("\n");
		}
		else
		{
			_._("MOV"); _._(" "); _._("ECX"); _._(", "); _._(IT.table[LT.table[startCyclePosition + 2].idxTI].visibility); _._("@"); _._(IT.table[LT.table[startCyclePosition + 2].idxTI].id); _._("\n");
		}
		
		_._("CMP"); _._(" "); _._("ECX"); _._(","); _._("0\n");
		_._("JE"); _._(" "); _._("skipcycle\n");

		_._("\t"); _._("CYCLE:\n");
		_._("MOV"); _._(" "); _._("RemECX"); _._(", "); _._("ECX\n");
	}

	void cycleEnd(debugFilePrint::File& _)
	{
		_._("MOV"); _._(" "); _._("ECX"); _._(","); _._(" "); _._("RemECX\n");
		_._("\t"); _._("loop"); _._(" "); _._("CYCLE\n");
		_._("skipcycle:\n");
	}

	void voidFuncHandle(size_t voidPosition, debugFilePrint::File& _)
	{
		_._(";---вызов---void---функции---\n");

		int param = IT.table[LT.table[voidPosition + 2].idxTI].funcParams.size();

		for (size_t i = voidPosition - param, j = 0; j < param; j++, i++)
		{
			if (IT.table[LT.table[i].idxTI].idtype == IT::IDTYPE::L)
			{
				if (IT.table[LT.table[i].idxTI].iddatatype == IT::IDDATATYPE::UINT)
				{
					_._("PUSH"); _._(" "); _._(IT.table[LT.table[i].idxTI].id); _._("\n");
				}

				else if (IT.table[LT.table[i].idxTI].iddatatype == IT::IDDATATYPE::STR ||
					IT.table[LT.table[i].idxTI].iddatatype == IT::IDDATATYPE::CHAR)
				{
					_._("PUSH"); _._(" "); _._("OFFSET"); _._(" "); _._(IT.table[LT.table[i].idxTI].id); _._("\n");
				}
			}

			else if (IT.table[LT.table[i].idxTI].idtype == IT::IDTYPE::V || 
					 IT.table[LT.table[i].idxTI].idtype == IT::IDTYPE::P)
			{
				if (IT.table[LT.table[i].idxTI].iddatatype == IT::IDDATATYPE::UINT)
				{
					_._("PUSH"); _._(" "); _._(IT.table[LT.table[i].idxTI].visibility); _._("@");
					_._(IT.table[LT.table[i].idxTI].id); _._("\n");
				}

				else if (IT.table[LT.table[i].idxTI].iddatatype == IT::IDDATATYPE::STR ||
					IT.table[LT.table[i].idxTI].iddatatype == IT::IDDATATYPE::CHAR)
				{
					_._("PUSH"); _._(" "); _._("OFFSET"); _._(" "); _._(IT.table[LT.table[i].idxTI].visibility); _._("@");
					_._(IT.table[LT.table[i].idxTI].id); _._("\n");
				}
			}
		}

		_._("CALL"); _._(" "); _._(IT.table[LT.table[voidPosition + 2].idxTI].id); _._("\n");
	}

	void assignHandle(size_t& AP, debugFilePrint::File& _)
	{
		_._(";---обработка---выражения---после---=\n"); 

		if (IT.table[LT.table[AP - 1].idxTI].iddatatype == IT::IDDATATYPE::UINT)
		{
			for (size_t i = AP + 1;; i++)
			{
				if (LT.table[i].lexema == LEX_SEMICOLON)
				{
					assignUint(AP, i, _);

					break;
				}

				else if (LT.table[i].lexema == LEX_PLUS)
				{
					UINT_PLUS
				}

				else if (LT.table[i].lexema == LEX_MINUS)
				{
					UINT_MINUS
				}

				else if (LT.table[i].lexema == LEX_STAR)
				{
					UINT_STAR
				}

				else if (LT.table[i].lexema == LEX_LEFT_SHIFT)
				{
					UINT_L_LOG_SHIFT
				}

				else if (LT.table[i].lexema == LEX_RIGHT_SHIFT)
				{
					UINT_R_LOG_SHIFT
				}

				else if (LT.table[i].lexema == '@')
				{
					funcCall(i, _);
				}

				//Попался casual
				else if (LT.table[i].lexema == LEX_CASUAL)
				{
					casualHandle(i, _);
				}

				//ПОПАЛСЯ ЛИТЕРАЛ
				else if (IT.table[LT.table[i].idxTI].idtype == IT::IDTYPE::L)
				{
					litpInsert(i, _);
				}

				//ПОПАЛАСЬ ПЕРЕМЕННАЯ
				else if (IT.table[LT.table[i].idxTI].idtype == IT::IDTYPE::V ||
					IT.table[LT.table[i].idxTI].idtype == IT::IDTYPE::P)
				{
					parVarInsert(i, _);
				}
			}
		}

		else if (IT.table[LT.table[AP - 1].idxTI].iddatatype == IT::IDDATATYPE::STR)

		{
			for (size_t i = AP + 1;; i++)
			{
				//stringCopy
				if (LT.table[i].lexema == LEX_SEMICOLON)
				{
					assignString(AP, _);
					break;
				}

				//strconcat
				else if (LT.table[i].lexema == LEX_PLUS)
				{
					CHECK_STRING_PLUS

					STRING_PLUS
				}

				//ПОПАЛСЯ ВЫЗОВ ФУНКЦИИ
				else if (LT.table[i].lexema == '@')
				{
					funcCall(i, _);
				}
				
				//ПОПАЛСЯ ЛИТЕРАЛ
				else if (IT.table[LT.table[i].idxTI].idtype == IT::IDTYPE::L)
				{
					litpInsert(i, _);
				}

				//ПОПАЛАСЬ ПЕРЕМЕННАЯ
				else if (IT.table[LT.table[i].idxTI].idtype == IT::IDTYPE::V ||
					IT.table[LT.table[i].idxTI].idtype == IT::IDTYPE::P)
				{
					parVarInsert(i, _);
				}
			}
		}

		else if (IT.table[LT.table[AP - 1].idxTI].iddatatype == IT::IDDATATYPE::CHAR)
		{
		
			for (size_t i = AP + 1;; i++)
			{
				//stringCopy
				if (LT.table[i].lexema == LEX_SEMICOLON)
				{
					assignString(AP, _);
					break;
				}

				else if (LT.table[i].lexema == LEX_LEFT_SHIFT)
				{
					CHAR_L_LOG_SHIFT
				}

				else if (LT.table[i].lexema == LEX_RIGHT_SHIFT)
				{
					CHAR_R_LOG_SHIFT
				}

				//ПОПАЛСЯ ВЫЗОВ ФУНКЦИИ
				else if (LT.table[i].lexema == '@')
				{
					funcCall(i, _);
				}

				//ПОПАЛСЯ ЛИТЕРАЛ
				else if (IT.table[LT.table[i].idxTI].idtype == IT::IDTYPE::L)
				{
					litpInsert(i, _);
				}

				//ПОПАЛАСЬ ПЕРЕМЕННАЯ
				else if (IT.table[LT.table[i].idxTI].idtype == IT::IDTYPE::V ||
					IT.table[LT.table[i].idxTI].idtype == IT::IDTYPE::P)
				{
					parVarInsert(i, _);
				}
			}

		}
	}
	//|
	void parVarInsert(size_t pV, debugFilePrint::File& _)
	{
		if (IT.table[LT.table[pV].idxTI].iddatatype == IT::IDDATATYPE::UINT)
		{
			_._("PUSH"); _._(" "); _._(IT.table[LT.table[pV].idxTI].visibility); _._("@");
			_._(IT.table[LT.table[pV].idxTI].id); _._("\n");
		}

		else if (IT.table[LT.table[pV].idxTI].iddatatype == IT::IDDATATYPE::STR ||
			IT.table[LT.table[pV].idxTI].iddatatype == IT::IDDATATYPE::CHAR)
		{
			_._("PUSH"); _._(" "); _._("OFFSET"); _._(" "); _._(IT.table[LT.table[pV].idxTI].visibility); _._("@");
			_._(IT.table[LT.table[pV].idxTI].id); _._("\n");
		}
	}
	void litpInsert(size_t l, debugFilePrint::File& _)
	{
		if (IT.table[LT.table[l].idxTI].iddatatype == IT::IDDATATYPE::UINT)
		{
			_._("PUSH"); _._(" "); _._(IT.table[LT.table[l].idxTI].id); _._("\n");
		}

		else if (IT.table[LT.table[l].idxTI].iddatatype == IT::IDDATATYPE::STR ||
			IT.table[LT.table[l].idxTI].iddatatype == IT::IDDATATYPE::CHAR)
		{
			_._("PUSH"); _._(" "); _._("OFFSET"); _._(" "); _._(IT.table[LT.table[l].idxTI].id); _._("\n");
		}
	}
	void funcCall(size_t fc, debugFilePrint::File& _)
	{
		_._("CALL"); _._(" "); _._(IT.table[LT.table[fc + 2].idxTI].id); _._("\n");
		_._("PUSH"); _._(" "); _._("EAX\n");
	}
	void breakCall(debugFilePrint::File& _)
	{
		_._("CALL BREAKL\n");
	}
	//|
	//|
	//|
	void assignUint(size_t& exCount, size_t currCount, debugFilePrint::File& _)
	{
		_._("POP"); _._(" "); _._("EAX\n");
		_._("MOV"); _._(" "); _._(IT.table[LT.table[exCount - 1].idxTI].visibility); _._("@");
		_._(IT.table[LT.table[exCount - 1].idxTI].id); _._(", "); _._("EAX"); _._("\n");

		exCount = currCount;
	}
	void casualHandle(size_t& c, debugFilePrint::File& _)
	{
		if (IT.table[LT.table[c + 1].idxTI].idtype == IT::IDTYPE::L)
		{
			_._("PUSH"); _._(" "); _._(IT.table[LT.table[c + 1].idxTI].id); _._("\n");
		}

		else if (IT.table[LT.table[c + 1].idxTI].idtype == IT::IDTYPE::V ||
			IT.table[LT.table[c + 1].idxTI].idtype == IT::IDTYPE::P)
		{
			_._("PUSH"); _._(" "); _._(IT.table[LT.table[c + 1].idxTI].visibility); _._("@");
			_._(IT.table[LT.table[c + 1].idxTI].id); _._("\n");
		}

		_._("CALL"); _._(" "); _._("casual\n");
		_._("PUSH"); _._(" "); _._("EAX\n");

		//Чтобы перепрыгнуть casual ... ->
		++c;
	}
	//|
	//|
	//|
	void assignString(size_t& exCount, debugFilePrint::File& _)
	{
		_._("PUSH"); _._(" "); _._("OFFSET"); _._(" "); _._(IT.table[LT.table[exCount - 1].idxTI].visibility); _._("@");
		_._(IT.table[LT.table[exCount - 1].idxTI].id); _._("\n");

		_._("CALL"); _._(" "); _._("stringCopy\n");
	}

	void TranslateASM(LT::LexTable first, IT::IdTable second)
	{
		LT = first;
		IT = second;

		File _("C:\\2курс, 1 сем\\КПО\\20\\Lexer_2\\Lexer\\ASM\\asm.asm");

		Head(_);

		Const(_);

		Data(_);

		Code(_);
	}


	void Head(debugFilePrint::File& _) {
		_._(".586\n");
		_._(".MODEL FLAT, STDCALL\n");

		_._("includelib libucrt.lib\n");
		_._("includelib kernel32.lib\n");
		_._("includelib \"C:\\2курс, 1 сем\\КПО\\20\\Lexer_2\\Lexer\\Debug\\StaticLibrary.lib\"\n");
		_._("ExitProcess PROTO :DWORD\n\n");

		_._("casual PROTO : DWORD\n");
		_._("printUint PROTO : DWORD\n");
		_._("printString PROTO : DWORD\n");
		_._("shiftChar PROTO : DWORD, : DWORD, : DWORD\n");
		_._("stringCopy PROTO : DWORD, : DWORD\n");
		_._("strconcat PROTO : DWORD, :DWORD\n");
		_._("STRLEN PROTO : DWORD\n");
		_._("BREAKL PROTO\n\n");
		_._("print_error_les_zero PROTO\n");
		_._("checkstringconcat PROTO : DWORD, :DWORD\n");
		_._("print_error_str_literal_length_exceeded PROTO\n\n");

		//Прототипы пользовательских функций
		for (size_t i = 0; i < IT.size; i++)
		{
			if (IT.table[i].idtype == IT::IDTYPE::F)
			{
				_._(IT.table[i].id); _._(" PROTO");
				for (size_t j = 1; j <= IT.table[i].funcParams.size(); j++)
				{
					if (j == IT.table[i].funcParams.size())
					{
						_._(" : DWORD\n");
						break;
					}

					_._(" : DWORD,");
				}
			}
		}

		_._("\n.STACK 4096\n\n");
	}

	//Литералы
	void Const(debugFilePrint::File& _)
	{
		_._(".CONST\n");

		for (size_t i = 0; i < IT.size; i++)
		{
			if (IT.table[i].idtype == IT::IDTYPE::L)
			{
				_._(IT.table[i].id);
				if (IT.table[i].iddatatype == IT::IDDATATYPE::STR)
				{
					_._(" BYTE "); _._((int)IT.table[i].value.vstr.len); _._(", \"");
					_._(IT.table[i].value.vstr.str); _._("\"\n");
				}
				
				if (IT.table[i].iddatatype == IT::IDDATATYPE::CHAR)
				{
					_._(" BYTE 1,\'"); _._(IT.table[i].value.vchar); _._("\'\n");
				}

				if (IT.table[i].iddatatype == IT::IDDATATYPE::UINT)
				{
					_._(" DWORD "); _._((int)IT.table[i].value.vint); _._("\n");
				}
			}
		}

		_._("\n\n;Константы для предачи в shiftChar\n");
		_._("charLeftShift BYTE 1,'['\n");
		_._("charRightShift BYTE 1,']'\n");
	}

	//Переменные
	void Data(debugFilePrint::File& _)
	{
		_._("\n.data\n\n");
		for (int i = 0; i < IT.size; i++) 
		{
			if (IT.table[i].idtype == IT::IDTYPE::V || IT.table[i].idtype == IT::IDTYPE::P)
			{
				_._(IT.table[i].visibility); _._("@"); _._(
					IT.table[i].id); _._(" ");
				if (IT.table[i].iddatatype == IT::IDDATATYPE::UINT)
				{
					_._("DWORD "); _._("0\n");
					continue;
				}
				if (IT.table[i].iddatatype == IT::IDDATATYPE::STR)
				{
					_._("BYTE "); _._(TI_STR_MAXSIZE); _._(" DUP(0)\n");
					continue;
				}
				if (IT.table[i].iddatatype == IT::IDDATATYPE::CHAR)
				{
					_._("BYTE "); _._(TI_STR_MAXSIZE); _._(" DUP(0)\n");
					continue;
				}

			}
		}

		//На случай если мы вызываем функцию из статической библиотеки 
		_._("\n;Чтобы не сбивался счетчик, когда мы заходим в функцию SL\n");
		 _._("RemECX"); _._(" "); _._("DWORD"); _._(" "); _._("0\n");
	}

	//УЖААААС
	void Code(debugFilePrint::File& _)
	{
		CODE_MAIN_FUNCTION_START
		
		//Ищем main
		for (size_t i = 0; i < LT.size; i++)
		{
			//Разберем сначала main
			if (LT.table[i].lexema == LEX_MAIN)
			{
				for (size_t m = i; ; m++)
				{	
					//Признак окончания функции }
					if ((LT.table[m].lexema == LEX_BRACELET) && (LT.table[m + 1].lexema != LEX_SEMICOLON))
						break;
					
					//Присвоение =
					else if (LT.table[m].lexema == LEX_EQUALS)
					{
						assignHandle(m, _);
					}

					//Вызов void
					else if (LT.table[m].lexema == '@' &&
					IT.table[LT.table[m + 2].idxTI].iddatatype == IT::IDDATATYPE::VOID)
					{
						voidFuncHandle(m, _);
					}

					//Print
					else if (LT.table[m].lexema == LEX_PRINT)
					{
						printHandle(m, _);
					}

					//Начало цикла
					else if (LT.table[m].lexema == LEX_WORK)
					{
						cycleStart(m, _);
					}

					//Конец цикла };
					else if (LT.table[m].lexema == LEX_BRACELET)
					{
						cycleEnd(_);
					}

					else if (LT.table[m].lexema == LEX_BREAKL)
					{
						breakCall(_);
					}
				}

				break;
			}
		}

		MAIN_FUNCTION_END

		//Ищем пользовательские функции
		for (size_t f = 0; f < LT.size; f++)
		{
			if (LT.table[f].lexema == LEX_FUNCTION)
			{
				//Заполнил сигнатуру функции
				_._(";-----------FUNCTION"); _._(" "); _._(IT.table[LT.table[f + 1].idxTI].id); _._("-----------\n\n");

				_._(IT.table[LT.table[f + 1].idxTI].id);
				_._(" ");
				_._("PROC");
				_._(" ");
				_._("uses");
				_._(" ");
				_._("EBX"); _._(" "); _._("ECX"); _._(" "); _._("EDX");
				
				//Запомнили позицию старта функции
				size_t fStartP;

				std::vector<std::string> tempStr;

				//Стартуем с 1 параметра и идем, пока не встретим )

				//Запомнили параметры
				for (size_t j = f + 3; ; j++)
				{
					if (LT.table[j].lexema == LEX_RIGHTTHESIS)
					{
						//Запомнили позицию старта функции
						fStartP = j + 2;
						break;
					}

					if (LT.table[j].lexema == LEX_ID)
					{
						if (IT.table[LT.table[j].idxTI].iddatatype == IT::IDDATATYPE::UINT)
						{
							tempStr.push_back(", " + IT.table[LT.table[j].idxTI].visibility + "@" + IT.table[LT.table[j].idxTI].id + "_" + "UINT" + " : " + "DWORD");
						}

						else if (IT.table[LT.table[j].idxTI].iddatatype == IT::IDDATATYPE::STR)
						{
							tempStr.push_back(", " + IT.table[LT.table[j].idxTI].visibility + "@" + IT.table[LT.table[j].idxTI].id + "_" + "STRING" + " : " + "DWORD");
						}

						else if (IT.table[LT.table[j].idxTI].iddatatype == IT::IDDATATYPE::CHAR)
						{
							tempStr.push_back(", " + IT.table[LT.table[j].idxTI].visibility + "@" + IT.table[LT.table[j].idxTI].id + "_" + "CHAR" + " : " + "DWORD");
						}
					}
				}

				//Выводим параметры в обратном порядке
				for (int i = tempStr.size()-1; i >= 0; i--)
				{
					_._(tempStr[i]);
				}

				_._("\n");

				//Присвоение параметрам
				for (size_t j = f + 3; LT.table[j].lexema != LEX_RIGHTTHESIS; j++)
				{
					if (LT.table[j].lexema == LEX_ID)
					{
						if (IT.table[LT.table[j].idxTI].iddatatype == IT::IDDATATYPE::UINT)
						{
							_._("MOV"); _._(" "); _._("EAX, "); _._(IT.table[LT.table[j].idxTI].visibility); _._("@"); _._(IT.table[LT.table[j].idxTI].id); _._("_"); _._("UINT\n");
							_._("MOV"); _._(" "); _._(IT.table[LT.table[j].idxTI].visibility); _._("@"); _._(IT.table[LT.table[j].idxTI].id); _._(", "); _._("EAX"); _._("\n");
						}

						else if (IT.table[LT.table[j].idxTI].iddatatype == IT::IDDATATYPE::STR)
						{
							_._("PUSH"); _._(" "); _._(IT.table[LT.table[j].idxTI].visibility); _._("@"); _._(IT.table[LT.table[j].idxTI].id); _._("_"); _._("STRING\n");
							_._("PUSH"); _._(" "); _._("OFFSET"); _._(" "); _._(IT.table[LT.table[j].idxTI].visibility); _._("@"); _._(IT.table[LT.table[j].idxTI].id); _._("\n");
							_._("CALL"); _._(" "); _._("stringCopy\n");
						}

						else if (IT.table[LT.table[j].idxTI].iddatatype == IT::IDDATATYPE::CHAR)
						{

							_._("PUSH"); _._(" "); _._(IT.table[LT.table[j].idxTI].visibility); _._("@"); _._(IT.table[LT.table[j].idxTI].id); _._("_"); _._("CHAR\n");
							_._("PUSH"); _._(" "); _._("OFFSET"); _._(" "); _._(IT.table[LT.table[j].idxTI].visibility); _._("@"); _._(IT.table[LT.table[j].idxTI].id); _._("\n");
							_._("CALL"); _._(" "); _._("stringCopy\n");
						}
					}
				}


				//Действия в функции
				for (size_t m = fStartP; ; m++)
				{
					//Признак окончания функции return
					if (LT.table[m].lexema == LEX_RETURN)
					{
						if (IT.table[LT.table[f + 1].idxTI].iddatatype == IT::IDDATATYPE::VOID)
						{
							_._("RET\n");
							_._(IT.table[LT.table[f+1].idxTI].id); _._(" "); _._("ENDP\n");
							
							//Основной цикл
							f = m;
							break;
						}

						else if(IT.table[LT.table[f + 1].idxTI].iddatatype == IT::IDDATATYPE::UINT)
						{
							for (size_t i = m + 1;; i++)
							{
								/////////////////////////////////////////////////////////////////////////////////////////////////////////
								if (LT.table[i].lexema == LEX_SEMICOLON)
								{
									_._("POP"); _._(" "); _._("EAX\n");
									
									_._("RET\n");
									_._(IT.table[LT.table[f + 1].idxTI].id); _._(" "); _._("ENDP\n");


									//Основной цикл
									f = m;

									break;
								}

								else if (LT.table[i].lexema == LEX_PLUS)
								{
									UINT_PLUS
								}

								else if (LT.table[i].lexema == LEX_MINUS)
								{
									UINT_MINUS
								}

								else if (LT.table[i].lexema == LEX_STAR)
								{
									UINT_STAR
								}

								else if (LT.table[i].lexema == LEX_LEFT_SHIFT)
								{
									UINT_L_LOG_SHIFT
								}

								else if (LT.table[i].lexema == LEX_RIGHT_SHIFT)
								{
									UINT_R_LOG_SHIFT
								}

								//ПОПАЛСЯ ВЫЗОВ ФУНКЦИИ
								else if (LT.table[i].lexema == '@')
								{
									funcCall(i, _);
								}

								//Попался casual
								else if (LT.table[i].lexema == LEX_CASUAL)
								{
									casualHandle(i, _);
								}

								//ПОПАЛСЯ ЛИТЕРАЛ
								else if (IT.table[LT.table[i].idxTI].idtype == IT::IDTYPE::L)
								{
									litpInsert(i, _);
								}

								//ПОПАЛАСЬ ПЕРЕМЕННАЯ
								else if (IT.table[LT.table[i].idxTI].idtype == IT::IDTYPE::V ||
									IT.table[LT.table[i].idxTI].idtype == IT::IDTYPE::P)
								{
									parVarInsert(i, _);
								}
							}							
						}

						else if (IT.table[LT.table[f + 1].idxTI].iddatatype == IT::IDDATATYPE::STR)
						{
							for (size_t i = m + 1;; i++)
							{
								if (LT.table[i].lexema == LEX_SEMICOLON)
								{
									_._("POP"); _._(" "); _._("EAX\n");

									_._("RET\n");
									_._(IT.table[LT.table[f + 1].idxTI].id); _._(" "); _._("ENDP\n");


									//Основной цикл
									f = m;

									break;
								}

								else if (LT.table[i].lexema == LEX_PLUS)
							{
									STRING_PLUS
							}

								//ПОПАЛСЯ ВЫЗОВ ФУНКЦИИ
								else if (LT.table[i].lexema == '@')
							{
									funcCall(i, _);
							}

								//ПОПАЛСЯ ЛИТЕРАЛ
								else if (IT.table[LT.table[i].idxTI].idtype == IT::IDTYPE::L)
							{
									litpInsert(i, _);
							}
									
								//ПОПАЛАСЬ ПЕРЕМЕННАЯ
								else if (IT.table[LT.table[i].idxTI].idtype == IT::IDTYPE::V ||
									IT.table[LT.table[i].idxTI].idtype == IT::IDTYPE::P)
							{
									parVarInsert(i, _);
							}
							}
						}

						else if (IT.table[LT.table[f + 1].idxTI].iddatatype == IT::IDDATATYPE::CHAR)
						{
							for (size_t i = m + 1;; i++)
							{

								//stringCopy
								if (LT.table[i].lexema == LEX_SEMICOLON)
								{
									_._("POP"); _._(" "); _._("EAX\n");

									_._("RET\n");
									_._(IT.table[LT.table[f + 1].idxTI].id); _._(" "); _._("ENDP\n");


									//Основной цикл
									f = m;

									break;
								}

								else if (LEX_LEFT_SHIFT)
								{
									CHAR_L_LOG_SHIFT
								}

								else if (LEX_RIGHT_SHIFT)
								{
									CHAR_R_LOG_SHIFT
								}

								//ПОПАЛСЯ ВЫЗОВ ФУНКЦИИ
								else if (LT.table[i].lexema == '@')
								{
									funcCall(i, _);
								}

								//ПОПАЛСЯ ЛИТЕРАЛ
								else if (IT.table[LT.table[i].idxTI].idtype == IT::IDTYPE::L)
								{
									litpInsert(i, _);
								}

								//ПОПАЛАСЬ ПЕРЕМЕННАЯ
								else if (IT.table[LT.table[i].idxTI].idtype == IT::IDTYPE::V ||
									IT.table[LT.table[i].idxTI].idtype == IT::IDTYPE::P)
								{
									parVarInsert(i, _);
								}
							}
						}

						break;
					}
					
					//Присвоение =
					else if (LT.table[m].lexema == LEX_EQUALS)
					{
						assignHandle(m, _);
					}

					//Вызов void функции
					else if (LT.table[m].lexema == '@' &&
						IT.table[LT.table[m + 2].idxTI].iddatatype == IT::IDDATATYPE::VOID)
					{
						voidFuncHandle(m, _);
					}

					//Print
					else if (LT.table[m].lexema == LEX_PRINT)
					{
						printHandle(m, _);
					}

					//Начало цикла
					else if (LT.table[m].lexema == LEX_WORK)
					{
						cycleStart(m, _);
					}

					//Конец цикла };
					else if (LT.table[m].lexema == LEX_BRACELET)
					{
						cycleEnd(_);
					}

					else if (LT.table[m].lexema == LEX_BREAKL)
					{
						breakCall(_);
					}
				}
			}


		}

		MAIN_PROCESS_END
	}
}