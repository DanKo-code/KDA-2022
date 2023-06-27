#include "Error.h"

namespace Error
{
	ERROR errors[ERROR_MAX_ENTRY]
	{
		//���������
		ERROR_ENTRY(0, "{System} ������������ ��� ������"),
		ERROR_ENTRY(1, "{System} ��������� ����"),
		ERROR_ENTRY(2, "{System} ��������� ����� �������� ���������"),//in.parm
		ERROR_ENTRY(3, "{System} �������� -in ������ ���� �����"),//in.parm
		ERROR_ENTRY(4, "{System} ������ ��� �������� ����� � �������� ����� (-in)"),//in.cpp
		ERROR_ENTRY(5, "{System} �������� ���� (-in) ����"),
		ERROR_ENTRY(6, "{System} �� ��������� ������ � �������� ����� (-in)"),//in.cpp
		ERROR_ENTRY(7, "{System} ������ ��� �������� ����� ��������� (-log)"),//log.cpp
		ERROR_ENTRY(8, "{System} ������ ��� �������� ��������� ����� (-out)"),//out.cpp
		ERROR_ENTRY_NODEF(9), ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40), ERROR_ENTRY_NODEF10(50),
		ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),
		
		//�����������
		ERROR_ENTRY(100, "{Lexis} ������� ������� ������ ������� ������"),//LT.cpp
		ERROR_ENTRY(101, "{Lexis} ������ �������  ������ �������� ������������ ���������"),//LT.cpp
		ERROR_ENTRY(102, "{Lexis} ������� ������� ������ ������� ���������������"),//IT.cpp
		ERROR_ENTRY(103, "{Lexis} ������ ������� ��������������� �������� ������������ ���������"),//IT.cpp
		ERROR_ENTRY(104, "{Lexis} ������ ������������ �������"),
		ERROR_ENTRY(105, "{Lexis} �������� ������ ������� ���������������"),//IT.cpp
		ERROR_ENTRY(106, "{Lexis} �������� ������ ������� ������"),//LT.cpp
		ERROR_ENTRY(107, "{Lexis} ������ ��� �������� ����� ������ ������� ���������������"),//IT.cpp
		ERROR_ENTRY(108, "{Lexis} ������ ��� �������� ����� ������ ������� ������"),//LT.cpp
		ERROR_ENTRY(109, "{Lexis} ������������������� �������"),//LexicalAnalysis.cpp
		ERROR_ENTRY(110, "{Lexis} ?"),
		ERROR_ENTRY(111, "{Lexis} ?"),
		ERROR_ENTRY(112, "{Lexis} ?"),
		ERROR_ENTRY(113, "{Lexis} ?"),
		ERROR_ENTRY(114, "{Lexis} ?"),
		ERROR_ENTRY(115, "{Lexis} ?"),
		ERROR_ENTRY(116, "{Lexis} ?"),
		ERROR_ENTRY(117, "{Lexis} ?"),
		ERROR_ENTRY(118, "{Lexis} ?"),
		ERROR_ENTRY(119, "{Lexis} ?"),
		ERROR_ENTRY_NODEF10(120), ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140), ERROR_ENTRY_NODEF10(150), ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170),
		ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),

		//��������������
		ERROR_ENTRY(200, "{Syntax} �������� ��������� ���������"),
		ERROR_ENTRY(201, "{Syntax} ��������� ��������"),
		ERROR_ENTRY(202, "{Syntax} ������ � ���������"),
		ERROR_ENTRY(203, "{Syntax} ������ � ��������� � �����������"),
		ERROR_ENTRY(204, "{Syntax} ������ � ��������� �������"),
		ERROR_ENTRY(205, "{Syntax} ������ � ������������"),
		ERROR_ENTRY(206, "{Syntax} ������ � ������ ���� �����"),
		ERROR_ENTRY(207, "{Syntax} ������� ��������� print/casual"),
		ERROR_ENTRY(208, "{Syntax} ?"),
		ERROR_ENTRY(209, "{Syntax} ?"),
		ERROR_ENTRY_NODEF10(210), ERROR_ENTRY_NODEF10(220), ERROR_ENTRY_NODEF10(230), ERROR_ENTRY_NODEF10(240), ERROR_ENTRY_NODEF10(250), ERROR_ENTRY_NODEF10(260),
		ERROR_ENTRY_NODEF10(270), ERROR_ENTRY_NODEF10(280), ERROR_ENTRY_NODEF10(290),

		//�������������
		ERROR_ENTRY(300, "{Semantics} ������������ ����� ����� ����� ��������� � ���������"),//SemanticsAnalysis.cpp
		ERROR_ENTRY(301, "{Semantics} �� ������� ������� ����� ����� � ���������"),			 //SemanticsAnalysis.cpp
		ERROR_ENTRY(302, "{Semantics} ��������������� �������������� �������"),		//LexicalAnalysis.cpp
		ERROR_ENTRY(303, "{Semantics} ������������� �� ���������"),					//LexicalAnalysis.cpp
		ERROR_ENTRY(304, "{Semantics} ������� ������ ���������� � �����������"),	//LexicalAnalysis.cpp
		ERROR_ENTRY(305, "{Semantics} ��������������� �������������� ����������"),	//LexicalAnalysis.cpp
		ERROR_ENTRY(306, "{Semantics} ���������� �� ����� ���� ���� void"),			//LexicalAnalysis.cpp
		ERROR_ENTRY(307, "{Semantics} ������������ �����"),		//SemanticsAnalysis.cpp
		ERROR_ENTRY(308, "{Semantics} ������������ ������� � string rvalue, �������� ������ +"), //SemanticsAnalysis.cpp
		ERROR_ENTRY(309, "{Semantics} ������������ ��������� char rvalue"),						 //SemanticsAnalysis.cpp

		ERROR_ENTRY(310, "{Semantics} �������������� ���� ������������� ���������"),			 //SemanticsAnalysis.cpp
		ERROR_ENTRY(311, "{Semantics} ������� ���� ���������� � ������ �������"),				 //SemanticsAnalysis.cpp
		ERROR_ENTRY(312, "{Semantics} ������� ����� ���������� � ������ �������"),
		ERROR_ENTRY(313, "{Semantics} �������� ������������ ����� ���������� �������(�������� ��������� 3)"),
		ERROR_ENTRY(314, "{Semantics} �������� ������������� casual"),
		ERROR_ENTRY(315, "{Semantics} ��������� ������ ���������� ��������(�������� 256)"),
		ERROR_ENTRY(316, "{Semantics} VOID ������� �� ����� ���������� ��������"),
		ERROR_ENTRY(317, "{Semantics} ��������� ������������ ������ ��������. ������������ ������ string �������� = 251"),
		ERROR_ENTRY(318, "{Semantics} ?"),
		ERROR_ENTRY(319, "{Semantics} ?"),
		ERROR_ENTRY_NODEF10(320), ERROR_ENTRY_NODEF10(330), ERROR_ENTRY_NODEF10(340), ERROR_ENTRY_NODEF10(350), ERROR_ENTRY_NODEF10(360), ERROR_ENTRY_NODEF10(370),
		ERROR_ENTRY_NODEF10(380), ERROR_ENTRY_NODEF10(390), ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500), ERROR_ENTRY_NODEF100(600), ERROR_ENTRY_NODEF100(700), ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900),
	};
	
	ERROR geterror(int id)
	{
		if (0 > id || id > ERROR_MAX_ENTRY) return errors[0];
		else return errors[id];
	}

	ERROR geterrorin(int id, int line = -1, int col = -1)
	{
		if (0 > id || id > ERROR_MAX_ENTRY) return errors[0];
		else
		{
			errors[id].inext.line = line;
			errors[id].inext.col = col;
			return errors[id];
		}
	}
};
