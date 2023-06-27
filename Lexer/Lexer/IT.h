#pragma once

#include <string>
#include <vector>

#define ID_MAXSIZE 30				// ������������ ���������� �������� � �������������� 
#define TI_MAXSIZE 4096				// ������������ ���������� ����� � ������� ��������������
#define TI_INT_DEFAULT 0x00000000   // �������� �� ��������� ��� ���� integer
#define TI_STR_DEFAULT 0x00			// �������� �� ��������� ��� ���� string
#define TI_NULLIDX 0xffffffff		// ��� �������� ������� ��������������� 
#define TI_STR_MAXSIZE 252

//#define GLOBAL_VISIBILITY_AREA "Global";

namespace IT		// ��
{
	enum IDDATATYPE		// ���� ������ ���������������:
	{
		UINT = 1,	// uint
		STR = 2,		// string
		CHAR = 3,	// char
		VOID = 4	// void
	};

	enum IDTYPE		// ���� ���������������: 
	{
		V = 1,                        // ����������
		F = 2,                        // �������
		P = 3,                        // ��������
		L = 4                         // �������
	};

	struct Entry	// ������ ��
	{
		int idxfirstLE;               // ������ ������ ������ � ��

		std::string id = "NaN";				  // ������������� (������������� ��������� �� ID_MAXISZE)

		IDDATATYPE iddatatype;        // ��� ������
		IDTYPE idtype;                // ��� ��������������
		union
		{
			unsigned int vint;				  // �������� uint
			struct
			{
				unsigned int len;						// ���������� �������� � string
				char str[TI_STR_MAXSIZE];	// ������� � string

			}vstr;//[TI_STR_MAXSIZE];				// �������� string

			char vchar;
		}value;						  // �������� ��������������

		std::vector<IT::IDDATATYPE> funcParams;        // ���� ���������� �������

		std::string visibility;   // ������� ���������
	};

	struct IdTable					  // ��������� ��
	{
		int maxsize;				  // ������� ��  <  TI_MAXSIZE
		int size;					  // ������� ������ ��  <  maxsize
		Entry* table;				  // ������ ����� ��
	};

	IdTable Create(			// ������� ��
		int size			// ������� ��  <  TI_MAXSIZE
	);

	void Add(				// �������� ������ � ��
		IdTable& idtable,	// ��������� ��
		Entry entry				// ������ ��
	);

	Entry GetEntry(			// �������� ������ ��
		IdTable& idtable,	// ��������� ��
		int n				// ����� ���������� ������
	);

	int IsId(
		IdTable& idtable,
		const char* id,
		int start
	);

	void PrintIdTable(const wchar_t* in, IdTable& idtable);
	std::string Mysetw(size_t width);

	void Delete(IdTable& idtable); // ������� �� (���������� ������)
}