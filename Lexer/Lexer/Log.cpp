#include "Log.h"

#pragma warning(disable:4996)

namespace Log
{
	//��������� (�������) �������� �����
	//���������� ������ � ��������� LOG. 
	LOG getlog(wchar_t logfile[]) //logfile � ��� �������� �����
	{
		LOG log;
		log.stream = new ofstream;
		log.stream->open(logfile);
		if (log.stream->fail()) //���� ����� �� ��������, ������������ ����������(ERRROR_THROW, ���	������ 112);
			throw ERROR_THROW(7);
		wcscpy_s(log.logfile, logfile); //������� wcscpy () ������������ ��� ����������� 
		//������ ������� �������� �� ��������� � ����� ����������. logfile - �������� ����, log.logfile - ���� �������� ������

		return log; //������� ����������� ��������� LOG.
	}

	//������������ ������������ ���� �����, �������� �����������, ��������� ������ � ������� �� � ��������.
	void WriteLine(LOG log, const char* c, ...) // ���������: ��������� LOG, ���������� ����� ���������� ���� char*
	{
		const char** ptr = &c;
		int i = 0;
		while (ptr[i] != "")
			*log.stream << ptr[i++];
		*log.stream << endl;
	}
	void WriteLine(LOG log, const wchar_t* c, ...)
	{
		const wchar_t** ptr = &c;
		char temp[100];
		int i = 0;
		while (ptr[i] != L"")
		{
			wcstombs(temp, ptr[i++], sizeof(temp)); //��� ������� ����������� ������ ������� ��������, 
			//������ ������� ������� ������ ��� ptr[i++], � �� ������������� �������������. ��������� ����������� � temp, 
			//sizeof(temp) - ������������ ���-�� �������� ��� ��������������
			*log.stream << temp;
		}
		*log.stream << endl;
	}

	// ������� ������ ��������� � ��������
	void WriteLog(LOG log) //��������:��������� LOG
	{
		//��� ��������� ������� ���� � ������� � ������� ������ ������������ ������� time, localtime_s � strftime.
		tm* timeinf;	//��������� �������� ������� �����
		time_t timet;
		time(&timet);	//��������� ���������
		timeinf = localtime(&timet);	//������� ��������� �����, �������������� � ���������
		char temp[100];
		strftime(temp, sizeof(temp), "\n------------��������------------\n ----%d.%m.%y %T----\n ", timeinf);
		*log.stream << temp;
	}
	void WriteIn(LOG log, In::IN in)
	{
		*log.stream << "\n---- �������� ������ ----\n"
			<< "\n\n����� ��������: " << in.size
			<< "\n\n����� �����: " << in.lines
			<< "\n\n���������: " << in.ignor << endl;
	}

	//������������ ��� ������ � �������� ���������� � ������� ����������
	void WriteParm(LOG log, Parm::PARM parm) //���������: ��������� LOG � ��������� PARM
	{
		char in_text[PARM_MAX_SIZE];
		char out_text[PARM_MAX_SIZE];
		char log_text[PARM_MAX_SIZE];
		wcstombs(in_text, parm.in, PARM_MAX_SIZE);
		wcstombs(out_text, parm.out, PARM_MAX_SIZE);
		wcstombs(log_text, parm.log, PARM_MAX_SIZE);
		*log.stream << "\n ---- ��������� ---- \n\n-in: " << in_text
			<< "\n-out: " << out_text
			<< "\n-log: " << log_text << endl;
	}

	//������������ ��� ������ � �������� ��� �� ������� ���������� �� ������
	void WriteError(LOG log, Error::ERROR error)//���������: ��������� LOG � ��������� IN
	{
		if (log.stream)
		{
			//WriteLog(log);

			*log.stream << "������ " << error.id
				<< ": " << error.message
				<< "\n������ " << error.inext.line
				<< " ������� " << error.inext.col;
			Close(log);
		}
		else //���� �������� �� ������, ������� ���������� �� �������
		{
			cout << "\n������ " << error.id << ": " << error.message << "\n������ " << error.inext.line << " ������� " << error.inext.col << endl;
		}
	}

	//������������ ��� �������� ��������� ������ ���������
	void Close(LOG log) //��������: ��������� LOG
	{
		log.stream->close();
		delete log.stream;
	}

}