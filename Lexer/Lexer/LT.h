#pragma once

#include <string>
#include "debugFilePrint.h"

using namespace debugFilePrint;

#define LEXEMA_FIXSIZE 1				// ���������y��� ������ �������
#define LT_MAXSIZE 4096					// ������������ ���������� ����� � ������� ������
#define LT_TI_NULLIDX 0xffffffff		// ��� �������� ������� ��������������



#define LEX_U_INTEGER 't'	//
#define LEX_CHAR 't'		//
#define LEX_STRING 't'		//
#define LEX_VOID 't'		//

#define LEX_ID 'i'			
#define LEX_LITERAL 'l'

#define LEX_FUNCTION 'f'

#define LEX_DECLARE 'v'
#define LEX_RETURN 'r'
#define LEX_WORK 'w'		//

#define LEX_BREAKL 'b'

#define LEX_PRINT 'p' 
#define LEX_CASUAL 'c'		// 

#define LEX_SEMICOLON ';'
#define LEX_COMMA ','
#define LEX_LEFTBRACE '{'
#define LEX_BRACELET '}'
#define LEX_LEFTTHESIS '('
#define LEX_RIGHTTHESIS ')'

#define LEX_PLUS '+'
#define LEX_MINUS '-'
#define LEX_STAR '*'
#define LEX_LEFT_SHIFT '['		//
#define LEX_RIGHT_SHIFT ']'		//

#define LEX_MAIN 'm'
#define LEX_EQUALS '='

//// ������ �� �� ���
//#define VOID_P_ID_TABLE -1

namespace LT {					// ��	

	struct Entry					 // ������ ��
	{
		char lexema;		// �������
		int sn;								// ����� ������ � �������� ������
		int csn;							// ����� ������� � ������ � �������� ������

		int idxTI;							// ������ � �� ��� LT_TI_NULLIDX

		std::string tekst;

		int priority;						// ��������� ��������

		Entry(unsigned char lexema, int sn, int csn, int idxTI, std::string tekst, int priority)
		{
			this->lexema = lexema;
			this->sn = sn;
			this->csn = csn;
			this->idxTI = idxTI;
			this->tekst = tekst;

			this->priority = priority;
		}

		Entry(unsigned char lexema, int sn, int csn, int idxTI, std::string tekst)
		{
			this->lexema = lexema;
			this->sn = sn;
			this->csn = csn;
			this->idxTI = idxTI;
			this->tekst = tekst;
		}

		Entry()
		{
			lexema = '#';
			sn = -1;
			csn = -1;
			idxTI = LT_TI_NULLIDX;
			tekst = "-";

			priority = -1;
		}

		Entry(char lexema)
		{
			this->lexema = lexema;
			sn = -1;
			csn = -1;
			idxTI = LT_TI_NULLIDX;
			tekst = "-";

			priority = -1;
		}
	};
	
	struct LexTable					  // ��������� ��
	{
		int maxsize; // ������� ������� ������ < LT_MAXSIZE
		int size; // ������� ������ ������� ������ < maxsize
		Entry* table; // ������ ����� ������� ������
	};

	LexTable Create(int size); // ������� ������� ������

	void Add(LexTable& lextable, Entry entry); // �������� ������ � ������� ������

	Entry GetEntry(LexTable& lextable, int n); // �������� ������ �� ������� ������

	void PrintLexTable(const char* in, LexTable& lextable);
	 
	void Delete(LexTable& lextable); // ������� ������� ������(���������� ������)

};
