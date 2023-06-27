#include <iostream>
#include <fstream>

#include "Error.h"
#include "In.h"

using namespace std;

namespace In
{
	IN getin(wchar_t* infile)
	{
		In::IN val{0,0};

		unsigned char uch;
		bool lexemeFlag = false;
		bool spaceFlag = false;

		ifstream fin;
		fin.open(infile);

		if (!fin.is_open())throw ERROR_THROW(4);

		//Убирает втупую пробелы до 1, если 1 то не убирает + обработка string лексемы
		while ((uch = fin.get()) && (!fin.eof()))
		{
			switch (val.code[uch])
			{
				case IN::F:
				{
					throw ERROR_THROW_IN(6, val.lines+1, (int)(val.text.size()+1) + val.ignor);
					continue;
				}

				case IN::T:
				{
					if (uch == '\n') val.lines++;
					val.text.push_back(uch);
					spaceFlag = false;
					continue;
				}

				case IN::L:
				{
					val.text.push_back(uch);
					lexemeFlag = !lexemeFlag;
					continue;
				}

				case IN::I:
				{
					++val.ignor;
					continue;
				}

				case IN::S:
				{
					if (spaceFlag)
					{
						if (lexemeFlag)
						{
							val.text.push_back(uch);
							continue;
						}

						++val.ignor;
						continue;
					}

					val.text.push_back(uch);
					spaceFlag = true;
					continue;
				}
			}
		}

		val.size = val.text.size();

		// можешь добавлять новые операторы
		string strD = "(){}*+,-;[]";
		
		//заполняем "красивый" - выходной буфер
		for (size_t i = 0; i < val.text.size(); i++)
		{
			if (strD.find(val.text[i]) != -1) 
			{
				if (i != 0 && val.text[i - 1] == ' ')
				{
					val.text.erase(i - 1, 1);

					++val.ignor;
					--i;
				}

				if (val.text[i + 1] == ' ')
				{
					val.text.erase(i + 1, 1);
					++val.ignor;
				}

				
			}
		}

		//если встречаю ' или " - чтобы не разделять rvalue(string, char)
		spaceFlag = false;
		val.spaceLexeme = val.text;

		//я так обрабатываю \n для корректности сопоставления строк с лексемами в исходнике 
		strD.push_back('\n');
		for (size_t i = 0; i < val.spaceLexeme.size(); i++)
		{
			if (strD.find(val.spaceLexeme[i]) != -1)
			{
				bool next = false;
				bool previous = false;

				// для того, чтобы в string литерале могли быть 
				//if (spaceFlag && val.spaceLexeme[i] == '\n') continue;

				if (val.spaceLexeme[i + 1] != '$')
				{
					val.spaceLexeme.insert(i + 1, "$");
					next = true;
				}

				if ( i!=0 && val.spaceLexeme[i - 1] != '$')
				{
					val.spaceLexeme.insert(i, "$");
					previous = true;
				}

				if (next && previous) i += 2;
				else ++i;
			}

			else if (val.spaceLexeme[i] == '\"' || val.spaceLexeme[i] == '\'') spaceFlag = !spaceFlag;
			else if ((val.spaceLexeme[i] == ' ') && !spaceFlag) val.spaceLexeme[i] = '$';

			//костыль, чтобы убрать подряд идущие $ сепараторы
			if(val.spaceLexeme[i] == '$' && val.spaceLexeme[i-1] == '$') val.spaceLexeme.erase(i, 1);
		}

		//если в конце не встретится (){}*+,-/<=>;[]\n, чтобы не терять лексему
		if(val.size != '$') val.spaceLexeme.push_back('$');
		
		fin.close();
		return val;
	}

}