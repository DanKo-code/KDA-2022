#include "Out.h"

#include "Error.h"

#pragma warning(disable:4996)
using namespace std;

namespace Out {

	OUT getout(wchar_t outfile[]) //logfile � ��� �������� �����
	{
		OUT out;
		out.stream = new ofstream;
		out.stream->open(outfile);
		if (out.stream->fail()) //���� ����� �� ��������, ������������ ����������(ERRROR_THROW, ���	������ 112);
			throw ERROR_THROW(8);
		wcscpy_s(out.outfile, outfile); //������� wcscpy () ������������ ��� ����������� 
		//������ ������� �������� �� ��������� � ����� ����������. logfile - �������� ����, log.logfile - ���� �������� ������

		return out; //������� ����������� ��������� LOG.
	}

	void WriteOut(OUT out, In::IN in)
	{
		*out.stream << in.spaceLexeme;
	}

	////������������ ��� ������ � �������� ��� �� ������� ���������� �� ������
	//void WriteError(OUT out, Error::ERROR error)//���������: ��������� LOG � ��������� IN
	//{
	//	if (out.stream)
	//	{
	//		*out.stream << "������ " << error.id
	//			<< ": " << error.message
	//			<< "\n������ " << error.inext.line
	//			<< " ������� " << error.inext.col << endl;
	//		Close(out);
	//	}
	//	else //���� �������� �� ������, ������� ���������� �� �������
	//	{
	//		cout << "\n������ " << error.id << ": " << error.message << "\n������ " << error.inext.line << " ������� " << error.inext.col << endl;
	//	}
	//}

	////������������ ��� �������� ��������� ������ ���������
	//void Close(OUT out) //��������: ��������� LOG
	//{
	//	out.stream->close();
	//	delete out.stream;
	//}
}