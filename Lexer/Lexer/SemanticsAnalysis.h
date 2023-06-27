#pragma once

#include "LT.h"
#include "IT.h"

namespace SemanticsAnalysis
{
	void SemAnalys(LT::LexTable LT, IT::IdTable ID);

	void MainCheck();

	// �������� �� ������������ ���� ���������� � rvalue
	void CompLit_VarType();             //<- - - - ��� �������!  
	//��� ��������� �����. ���������� �����.	  |
	void funcParmProcess(size_t& index);//- - -> -  

	//�������� �� ���-�� ���-��(�� ������ ���� < 9)
	void CheckParamsNumb();

	//�������� �� void return
	void CheckVoidFuncRet();
}