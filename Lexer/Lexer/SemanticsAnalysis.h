#pragma once

#include "LT.h"
#include "IT.h"

namespace SemanticsAnalysis
{
	void SemAnalys(LT::LexTable LT, IT::IdTable ID);

	void MainCheck();

	// Проверка на соответствие типа переменной и rvalue
	void CompLit_VarType();             //<- - - - они связаны!  
	//для обработки парам. вызываемой функц.	  |
	void funcParmProcess(size_t& index);//- - -> -  

	//Проверка на кол-во пар-ов(Их должно быть < 9)
	void CheckParamsNumb();

	//Проверка на void return
	void CheckVoidFuncRet();
}