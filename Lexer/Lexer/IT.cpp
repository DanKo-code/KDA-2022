#include "IT.h"
#include "Error.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

namespace IT
{
	IdTable Create(int size)
	{
		if (size > TI_MAXSIZE)
		{
			throw ERROR_THROW(102)
		}
		IdTable tableID;
		tableID.table = new Entry[size];
		tableID.maxsize = size;
		tableID.size = NULL;
		return tableID;
	}
	void Add(IdTable& idtable, Entry entry)
	{
		if (idtable.size >= idtable.maxsize)
		{
			throw ERROR_THROW(103)
		}
		idtable.table[idtable.size++] = entry;
	}
	Entry GetEntry(IdTable& idtable, int n)
	{
		if (n >= idtable.maxsize)
		{
			throw ERROR_THROW(105)
		}
		return idtable.table[n];
	}
	
	string Mysetw(size_t width)
	{
		string temp;

		for (size_t i = 0; i < width; i++)
		{
			temp.push_back(' ');
		}

		return temp;
	}

	void PrintIdTable(const wchar_t* in, IdTable& idtable)
	{
		std::ofstream* idStream = new std::ofstream;
		idStream->open(in);

		if (idStream->is_open())
		{
			(*idStream) << "--------- Таблица идентификаторов ---------"<<endl;

						 //12							 //14							    //19								  //11							 //18								   //22										 //9
			*(idStream)<<"Индекс в ТИ:" << Mysetw(10) << "Идентификатор:" << Mysetw(10) << "Тип идентификатора:" << Mysetw(10) << "Тип данных:" << Mysetw(10) << "Область видимости:" << Mysetw(10) << "Первое вхождение в ТЛ:" << Mysetw(10) << "Значение:" << std::endl;

			for (size_t i = 0; i < idtable.size; i++)
			{
				*(idStream) << i<<"."<< Mysetw(12 - (to_string(i).size()+1) + 10) << idtable.table[i].id << Mysetw(14 - idtable.table[i].id.size() + 10);

					switch (idtable.table[i].idtype)
					{
					case 1:
					{
						*(idStream) << "V";
						break;
					}

					case 2:
					{
						*(idStream) << "F";
						break;
					}

					case 3:
					{
						*(idStream) << "P";
						break;
					}

					case 4:
					{
						*(idStream) << "L";
						break;
					}

					}

					*(idStream) << Mysetw(18+10);

					switch (idtable.table[i].iddatatype)
					{
					case 1:
					{
						*(idStream) << "UINT";
						break;
					}

					case 2:
					{
						*(idStream) << "STR";
						break;
					}

					case 3:
					{
						*(idStream) << "CHAR";
						break;
					}

					}

					*(idStream) << Mysetw(11-3+10);
					
					*(idStream) << idtable.table[i].visibility << Mysetw(18-idtable.table[i].visibility.size()+10) << idtable.table[i].idxfirstLE << Mysetw(22- to_string(idtable.table[i].idxfirstLE).size() +10);
				switch (idtable.table[i].iddatatype)
				{
				case 1:
				{
					*(idStream) << idtable.table[i].value.vint;
					break;
				}

				case 2:
				{
					*(idStream) << idtable.table[i].value.vstr.len <<"\""<< idtable.table[i].value.vstr.str<< "\"";
					break;
				}

				case 3:
				{
					*(idStream) << "\'" << idtable.table[i].value.vchar << "\'";
					break;
				}

				} 

				*(idStream) << endl<<endl;
			}
		}
		else
			throw ERROR_THROW(107);
		idStream->close();
		delete idStream;
	}

	

	void Delete(IdTable& idtable)
	{
		delete[] idtable.table;
	}
}