#pragma once

#include <fstream>
#include <iostream>
#include <string>

namespace debugFilePrint
{
	struct File
	{
		File(const char*);
	
		const char* fileName;

		void _(std::string);
		void _(char);
		void _(int);
	};

	
}