#include "stdafx.h"

//#define TEST_ERROR
//#define TEST_GETPARM
//#define TEST_GETIN
#define TEST_LOG
//#define TEST_OUT

#ifdef TEST_ERROR

////////////////////////////////////////////////// TEST ERROR ///////////////////////////////////////////
int main()
{
	setlocale(0, "rus");

	cout << "---���� Error::geterror---" << endl << endl;
	try { throw ERROR_THROW(106); }
	catch (Error::ERROR e)
	{
		cout << "������ " << e.id << ": " << e.message << endl << endl;
	}

	cout << "---���� Error::geterrorin---" << endl << endl;

	try { throw ERROR_THROW_IN(111, 7, 7); }
	catch (Error::ERROR e)
	{
		cout << "������ " << e.id << ": " << e.message
			<< ", ������ " << e.inext.line
			<< ", ������� " << e.inext.col << endl << endl;
	}
}
////////////////////////////////////////////////// TEST ERROR ///////////////////////////////////////////

#endif // TEST_ERROR



#ifdef TEST_GETPARM

////////////////////////////////////////////////// TEST GETPARM ///////////////////////////////////////////
int wmain(int argc, _TCHAR* argv[])
{
	setlocale(0, "rus");

	cout << "--- ���� Parm::getparm ---" << endl << endl;

	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		wcout << "-in:" << parm.in << ", -out:" << parm.out << ", -log:" << parm.log << endl << endl;
	}
	catch (Error::ERROR e)
	{
		cout << "������ " << e.id << ": " << e.message << endl << endl;
	}

	return 0;
}
////////////////////////////////////////////////// TEST GETPARM ///////////////////////////////////////////

#endif // TEST_GETPARM



#ifdef TEST_GETIN

////////////////////////////////////////////////// TEST GETIN /////////////////////////////////////////////
int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "rus");
	cout << "----  ���� In::getin  ----" << endl << endl;
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		In::IN in = In::getin(parm.in);
		cout << in.text << endl;
		cout << "����� ��������: " << in.size << endl;
		cout << "����� �����: " << in.lines << endl;
		cout << "���������: " << in.ignor << endl;
	}
	catch (Error::ERROR e)
	{
		cout << "������ " << e.id << ": " << e.message << endl;
		cout << "������ " << e.inext.line << " ������� " << e.inext.col << endl << endl;
	}
	catch (...)
	{
		cout << "���� ����!!!";
	}

}
////////////////////////////////////////////////// TEST GETIN /////////////////////////////////////////////

#endif // TEST_GETIN



#ifdef TEST_LOG

////////////////////////////////////////////////// TEST LOG ///////////////////////////////////////////////
int wmain(int argc, _TCHAR* argv[])
{
	

	system("mode con cols=160 lines=30000");

	Log::LOG log = Log::INITLOG;
	Out::OUT out = Out::INITOUT;

	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		In::IN in = In::getin(parm.in);
		log = Log::getlog(parm.log);

		Log::WriteLine(log, "����:", " ��� ������ \n", "");
		Log::WriteLog(log);
		Log::WriteParm(log, parm);
		Log::WriteIn(log, in);
		LexAnalys::LT_IT_Tables lex_id = LexAnalys::GetTables(in, 1000, 1000);
		
		LT::PrintLexTable("C:\\2����, 1���\\���\\20\\Lexer_2\\Lexer\\Lexer\\TableOfLexems.txt", lex_id.lexTable);
		IT::PrintIdTable(L"TableOfIdentificators.txt", lex_id.idTable);

		MFST_TRACE_START
			MFST::Mfst mfst(lex_id.lexTable, GRB::getGreibach());
		if(!mfst.start()) return 0; //����� �� ������ ������ ���. ������
		mfst.savededucation();
		mfst.printRules();

		

		SemanticsAnalysis::SemAnalys(lex_id.lexTable, lex_id.idTable);
		
		PolishNatation::convertToPN(lex_id.lexTable, lex_id.idTable);
		
		LT::PrintLexTable("C:\\2����, 1 ���\\���\\20\\Lexer_2\\Lexer\\Lexer\\PolishTableOfLexems.txt", lex_id.lexTable);

		Translation::TranslateASM(lex_id.lexTable, lex_id.idTable);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// ���������� �����
		/*cout << "������� ������" << endl << endl;
		for (size_t i = 0; i < lex_id.lexTable.size; i++)
		{
			cout <<i << ".\t";
			cout << "����������� �������" << "\t" << "����� ������" << "\t" << "������ � ��"<< "\t" << "�������" << endl;
			cout<<"\t" << lex_id.lexTable.table[i].lexema << "\t\t\t";
			cout<<lex_id.lexTable.table[i].sn<<"\t\t";
			cout<<lex_id.lexTable.table[i].idxTI<<"\t\t";
			cout<<lex_id.lexTable.table[i].tekst;

			cout << endl<<endl;
		}

		cout << "������� ���������������" << endl << endl;

		int indexIT = 0;

		cout << "��������" << endl<<"********************************************************************************************************" << endl;
		for (size_t i = 0; i < lex_id.idTable.size; i++)
		{
			if (lex_id.idTable.table[i].idtype == 4)
			{
				cout << "������ � ��: " << i << endl;
				cout << "�������������: " << lex_id.idTable.table[i].id << endl;

				if (lex_id.idTable.table[i].iddatatype == 1)
				{
					cout << "��� ������: " << lex_id.idTable.table[i].iddatatype << " INT" << endl;

					cout << "��������: " << lex_id.idTable.table[i].value.vint << endl;
				}
					
				if (lex_id.idTable.table[i].iddatatype == 2)
				{
					cout << "��� ������: " << lex_id.idTable.table[i].iddatatype << " STR" << endl;

					cout << "������: " << lex_id.idTable.table[i].value.vstr.len << endl;
					cout << "��������: "<<"'"<< lex_id.idTable.table[i].value.vstr.str << "'" << endl;
				}
					
				cout << "������ ��������� � ��: " << lex_id.idTable.table[i].idxfirstLE << endl;
				cout << "������� ���������: " << lex_id.idTable.table[i].visibility << endl;

				cout << endl << endl;
			}
		}

		cout << endl << endl<<endl;
		cout << "�������" << endl << "********************************************************************************************************" << endl;
		for (size_t i = 0; i < lex_id.idTable.size; i++)
		{
			if (lex_id.idTable.table[i].idtype == 2)
			{
				cout << "������ � ��: " << i << endl;
				cout << "�������������: " << lex_id.idTable.table[i].id << endl;

				if (lex_id.idTable.table[i].iddatatype == 1)
					cout << "��� ������ ������������� ��������: " << lex_id.idTable.table[i].iddatatype << " INT" << endl;

				if (lex_id.idTable.table[i].iddatatype == 2)
					cout << "��� ������ ������������� ��������: " << lex_id.idTable.table[i].iddatatype << " STR" << endl;

				cout << "������ ��������� � ��: " << lex_id.idTable.table[i].idxfirstLE << endl;
				cout << "������� ���������: " << lex_id.idTable.table[i].visibility << endl;

				cout << endl << endl;
			}
		}

		cout << endl << endl << endl;
		cout << "��������� �������" << endl << "********************************************************************************************************" << endl;
		for (size_t i = 0; i < lex_id.idTable.size; i++)
		{
			if (lex_id.idTable.table[i].idtype == 3)
			{
				cout << "������ � ��: " << i << endl;
				cout << "�������������: " << lex_id.idTable.table[i].id << endl;

				if (lex_id.idTable.table[i].iddatatype == 1)
					cout << "��� ������: " << lex_id.idTable.table[i].iddatatype << " INT" << endl;

				if (lex_id.idTable.table[i].iddatatype == 2)
					cout << "��� ������: " << lex_id.idTable.table[i].iddatatype << " STR" << endl;

				cout << "������ ��������� � ��: " << lex_id.idTable.table[i].idxfirstLE << endl;
				cout << "������� ���������: " << lex_id.idTable.table[i].visibility << endl;

				cout << endl << endl;
			}
		}

		cout << endl << endl << endl;
		cout << "����������" << endl << "********************************************************************************************************" << endl;
		for (size_t i = 0; i < lex_id.idTable.size; i++)
		{
			if (lex_id.idTable.table[i].idtype == 1)
			{
				cout << "������ � ��: " << i << endl;
				cout << "�������������: " << lex_id.idTable.table[i].id << endl;

				if (lex_id.idTable.table[i].iddatatype == 1)
					cout << "��� ������: " << lex_id.idTable.table[i].iddatatype << " INT" << endl;

				if (lex_id.idTable.table[i].iddatatype == 2)
					cout << "��� ������: " << lex_id.idTable.table[i].iddatatype << " STR" << endl;

				cout << "������ ��������� � ��: " << lex_id.idTable.table[i].idxfirstLE << endl;
				cout << "������� ���������: " << lex_id.idTable.table[i].visibility << endl;

				cout << endl << endl;
			}
		}*/

		out = Out::getout(parm.out);
		Out::WriteOut(out, in);

		Log::Close(log);
	}
	catch (Error::ERROR e)
	{
		Log::WriteError(log, e);
	}
}
////////////////////////////////////////////////// TEST LOG ///////////////////////////////////////////////

#endif // TEST_LOG

////////////////////////////////////////////////// TEST OUT ///////////////////////////////////////////////
#ifdef TEST_OUT

int wmain(int argc, _TCHAR* argv[])
{
	Out::OUT out = Out::INITOUT;
	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);

		out = Out::getout(parm.out);

		In::IN in = In::getin(parm.in);


		Out::WriteOut(out, in);
		Out::Close(out);
	}
	catch (Error::ERROR e)
	{
		Parm::PARM parm = Parm::getparm(argc, argv);

		Out::WriteError(out, e);
	}
}

#endif // TEST_OUT
////////////////////////////////////////////////// TEST OUT ///////////////////////////////////////////////