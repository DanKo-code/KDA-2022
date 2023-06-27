#include "IT.h"
#include "LT.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

#include "debugFilePrint.h"

using namespace debugFilePrint;

namespace Translation
{
	

	void TranslateASM(LT::LexTable, IT::IdTable);

	void Head(debugFilePrint::File& _);
	void Const(debugFilePrint::File& _);
	void Data(debugFilePrint::File& _);
	void Code(debugFilePrint::File& _);

	///////////////////////////////////////////////////////////////////
	void printHandle(size_t, debugFilePrint::File&);

	void cycleStart(size_t, debugFilePrint::File&);

	void cycleEnd(debugFilePrint::File&);

	void voidFuncHandle(size_t, debugFilePrint::File&);

	void assignHandle(size_t&, debugFilePrint::File&);
	void parVarInsert(size_t, debugFilePrint::File&);
	void litpInsert(size_t, debugFilePrint::File&);
	void funcCall(size_t, debugFilePrint::File&);
	void breakCall(debugFilePrint::File&);
	//|
	//|
	//|
	void assignUint(size_t&, size_t, debugFilePrint::File&);
	void casualHandle(size_t&, debugFilePrint::File&);
	//|
	//|
	//|
	void assignString(size_t&, debugFilePrint::File&);
}