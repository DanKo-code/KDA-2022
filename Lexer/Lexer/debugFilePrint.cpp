#include "debugFilePrint.h"

std::ofstream inputFile;

debugFilePrint::File::File(const char* fileName)
{
	this->fileName = fileName;
	inputFile.open(this->fileName);
	inputFile.clear();
	inputFile.close();
}

void debugFilePrint::File::_(std::string item)
{
	inputFile.open(fileName, std::ios_base::app);

	inputFile << item;

	inputFile.close();
}

void debugFilePrint::File::_(char item)
{
	inputFile.open(fileName, std::ios_base::app);

	inputFile << item;

	inputFile.close();
}

void debugFilePrint::File::_(int item)
{
	inputFile.open(fileName, std::ios_base::app);

	inputFile << item;

	inputFile.close();
}