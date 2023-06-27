#include "LexicalAnalysis.h"
#include "Error.h"
#include <string>

#include <sstream>

namespace LexAnalys
{
	LT_IT_Tables GetTables(In::IN in, size_t ltSize, size_t itSize)
	{
		LT_IT_Tables lt_it_Tables = { LT::Create(ltSize), IT::Create(itSize) };

		vector<string> lexemesBuffer = SeparateLexeme(in.spaceLexeme);

		char lexemeName;			// сокращенное имя лексемы 
		size_t literalIndex = 0;	// for literal name
		size_t lineNumber = 1;
		size_t lineColumn = 1;

		//Для определения областей видимости(область видимости на данный момент)
		string visibily;

		//счетчик для перебора lexemesBuffer
		size_t i = 0;

		//Флаг, сигнализирует, что была фунция(нужно запоминать типы пар-ов) 
		bool fParCheck = false;
		int fIdIndex = 0;	//индекс функции, куда нужно записывать типы пар-ов


		//Семантические проверки///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//Ищу идент. среди объявлений текущей функции, если среди них не нашел, ищу выше идент. прошлых функций(может чел хотел вызвать функцию :) )

		//Во всем этом добре нужно исключать из сравнений литералы, так как у них своя область видимости
		
		//!!!Ссылаться на верхние функции я то могу, но вот синтакс. анализ. ошибку не выдаст. Нужно как-то вычислять синтаксис функции здесь(хз, как это сделать втупую. Решил просто добавить проверку, что следующий символ '(')
		//303 304
		auto identifierDefinitionCheck = [&]()
		{
			int j = lt_it_Tables.idTable.size - 1;

			for (; ; j--)
			{
				if (lt_it_Tables.idTable.table[j].idtype == IT::IDTYPE::L) continue;

				if (lt_it_Tables.idTable.table[j].visibility != visibily) break;

				if (lt_it_Tables.idTable.table[j].id == lexemesBuffer[i].substr(0, ID_MAXSIZE))
				{
					Add(lt_it_Tables.lexTable, LT::Entry(lexemeName, lineNumber, lineColumn, j, lexemesBuffer[i]));
					return;
				}

			}

			// Возможно не будет работать, из-за строгово IT::IDTYPE::F, ведь есть еще просто лексема main
			// (скорее всего, как раз будет, т.к ,main мы не будем вызывать!)
			// 
			//Если прошлый не отработал, то пытаюсь найти идентификатор среди прошлых функий (нужно идти, пока не дойдем до функции в которой интересуемая переменная)
			for (int j = 0; ; j++)
			{
				if (lt_it_Tables.idTable.table[j].idtype == IT::IDTYPE::L) continue;

				//ИЗБЕГАЮ РЕКУРСИЮ!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
				//Нужно как-то обработать случай, когда у нас данная ситуация происходит в main(т.к main - лексема и ее нет в ТИ)
				//После || ввел проверку, если данная ситуация происходит в main
				if (lt_it_Tables.idTable.table[j].id == visibily || lt_it_Tables.idTable.table[j].id == "NaN") break;

				
				if (lt_it_Tables.idTable.table[j].idtype == IT::IDTYPE::F && lt_it_Tables.idTable.table[j].id == lexemesBuffer[i].substr(0, ID_MAXSIZE))
				{
					//Если чел хотел вызвать функцию, то уже на этом этапе нужно проверить есть ли после этого идент. синтаксис функции
					//это, чтобы не сбить общий счетчик(i)
					size_t temp_i = i+1;

					//на случай, если идут \n-ы   
					for (;; i++)
					{
						//и тут
						if (lexemesBuffer[temp_i][0] == '\n') continue;

						//и тут могут возникнуть вопросики, но их поидее должен обрубить синтакс. анализ
						if (lexemesBuffer[temp_i][0] == '(') break;

						throw ERROR_THROW_IN(304, lineNumber, lineColumn);
					}

					Add(lt_it_Tables.lexTable, LT::Entry(lexemeName, lineNumber, lineColumn, j, lexemesBuffer[i]));
					return;
				}
			}

			throw ERROR_THROW_IN(303, lineNumber, lineColumn);
		};

		//Для проверки на повторение идент. функций
		//302 
		auto funIdRedefiningCkeck = [&]()
		{
			for (size_t j = 0; j < lt_it_Tables.idTable.size; j++)
			{
				if(lt_it_Tables.idTable.table[j].idtype == IT::IDTYPE::F &&
					lt_it_Tables.idTable.table[j].id == lexemesBuffer[i].substr(0, ID_MAXSIZE))
					throw ERROR_THROW_IN(302, lineNumber, lineColumn);
			}
		};

		//Для проверки на повторение идент. переменной в функции
		//305 
		auto varIdRedefiningCkeck = [&]()
		{
			//для поиска индекса текущей функции
			size_t temp_i = lt_it_Tables.idTable.size - 1;

			for (;; temp_i--)
			{
				if (lt_it_Tables.idTable.table[temp_i].visibility != visibily)
				{
					++temp_i;
					break;
				}
			}

			//На случай, если в функции ничего не было
			if (lt_it_Tables.idTable.table[temp_i].id == "NaN")
			{
				return;
			}

			//когда нашли, идем вниз по идент-ам функции, пока не дойдем до позиции нового(последнего) элемента
			for (; temp_i != i; temp_i++)
			{
				if(lt_it_Tables.idTable.table[temp_i].id == lexemesBuffer[i]) 
					throw ERROR_THROW_IN(305, lineNumber, lineColumn);
			}
		};



		//Для заполнения типа данных и значения(по умолчанию) для идентификатора
		auto Set_IDDATATYPE_VALUE_F = [&](IT::Entry& temp)
		{
			if (lexemesBuffer[i - 2] == "uint")
			{
				temp.iddatatype = IT::IDDATATYPE::UINT;
				temp.value.vint = 0;
			}

			if (lexemesBuffer[i - 2] == "string")
			{
				temp.iddatatype = IT::IDDATATYPE::STR;
				temp.value.vstr.len = 0;
				//temp.value.vstr.str[0] = '\0';

				memset(temp.value.vstr.str, '\0', sizeof(char) * TI_STR_MAXSIZE);
			}

			if (lexemesBuffer[i - 2] == "char")
			{
				temp.iddatatype = IT::IDDATATYPE::CHAR;
				temp.value.vchar = '\0';
			}

			if (lexemesBuffer[i - 2] == "void")
			{
				temp.iddatatype = IT::IDDATATYPE::VOID;
			}
		};

		auto Set_IDDATATYPE_VALUE_V_P = [&](IT::Entry& temp)
		{
			if (lexemesBuffer[i - 1] == "uint")
			{
				temp.iddatatype = IT::IDDATATYPE::UINT;
				temp.value.vint = 0;
			}

			if (lexemesBuffer[i - 1] == "string")
			{
				temp.iddatatype = IT::IDDATATYPE::STR;
				temp.value.vstr.len = 0;
				//temp.value.vstr.str[0] = '\0';

				memset(temp.value.vstr.str, '\0', sizeof(char) * TI_STR_MAXSIZE);
			}

			if (lexemesBuffer[i - 1] == "char")
			{
				temp.iddatatype = IT::IDDATATYPE::CHAR;
				temp.value.vchar = '\0';
			}
		};

		for (; i < lexemesBuffer.size(); i++)
		{
			//подсчет идекса строки в исходном коде
			if (lexemesBuffer[i] == "\n")				
			{
				++lineNumber;
				lineColumn = 1;
				continue;
			}   
 
			lexemeName = GetLexemeName(lexemesBuffer[i], lineNumber, lineColumn);

			//идентификатор
			if (lexemeName == LEX_ID)
			{
				//Так как в самом начале файла мы не сможем определить тип идент. и выйдем за пределы массива
				if(i == 0 || i == 1) throw ERROR_THROW_IN(303, lineNumber, lineColumn);

				if (lexemesBuffer[i - 1] == "function")
				{
					//для запоминания кол-ва и типа пар-ов
					fParCheck = true;

					//запоминаем индекс функц.
					fIdIndex = lt_it_Tables.idTable.size;	 

					
					//Проверка на переопределение ф.
					funIdRedefiningCkeck();

					IT::Entry temp;

					temp.idxfirstLE = lt_it_Tables.lexTable.size;

					temp.id = lexemesBuffer[i].substr(0, ID_MAXSIZE);
					
					Set_IDDATATYPE_VALUE_F(temp);

						
					temp.idtype = IT::IDTYPE::F;

					temp.visibility = "-";

					IT::Add(lt_it_Tables.idTable, temp);

					visibily = (lexemesBuffer[i]);

					//add to lex table
					Add(lt_it_Tables.lexTable, LT::Entry(lexemeName, lineNumber, lineColumn, lt_it_Tables.idTable.size - 1, lexemesBuffer[i]));
				}		

				//переменная
				else if (lexemesBuffer[i - 1] == "string" || lexemesBuffer[i - 1] == "uint" || lexemesBuffer[i - 1] == "char" || lexemesBuffer[i - 1] == "void")
				{
					//проверка на тип void
					if (lexemesBuffer[i - 1] == "void") throw ERROR_THROW_IN(306, lineNumber, lineColumn);

					//Проверка на переопределение
					varIdRedefiningCkeck();

					if (lexemesBuffer[i - 2] == "var")
					{
						IT::Entry temp;

						temp.idxfirstLE = lt_it_Tables.lexTable.size;

						temp.id = lexemesBuffer[i].substr(0, ID_MAXSIZE);
						
						Set_IDDATATYPE_VALUE_V_P(temp);


						temp.idtype = IT::IDTYPE::V;

						temp.visibility = visibily;

						IT::Add(lt_it_Tables.idTable, temp);

						//add to lex table
						Add(lt_it_Tables.lexTable, LT::Entry(lexemeName, lineNumber, lineColumn, lt_it_Tables.idTable.size - 1, lexemesBuffer[i]));
					}

					//Параметр функции
					else
					{
						IT::Entry temp;

						temp.idxfirstLE = lt_it_Tables.lexTable.size;

						temp.id = lexemesBuffer[i].substr(0, ID_MAXSIZE);
						
						Set_IDDATATYPE_VALUE_V_P(temp);


						//запомнил один из пар-ов функции
						lt_it_Tables.idTable.table[fIdIndex].funcParams.push_back(temp.iddatatype);


						temp.idtype = IT::IDTYPE::P;

						temp.visibility = visibily;

						IT::Add(lt_it_Tables.idTable, temp);

						//add to lex table
						Add(lt_it_Tables.lexTable, LT::Entry(lexemeName, lineNumber, lineColumn, lt_it_Tables.idTable.size - 1, lexemesBuffer[i]));
					}
				}

				//на случай использования идентификатора после обьявления.
				else
				{
					identifierDefinitionCheck();
				}
			}

			//литерал
			else if (lexemeName == LEX_LITERAL)
			{
				//string литерал 
				if (lexemesBuffer[i][0] == '\"')
				{
					IT::Entry temp;

					temp.idxfirstLE = lt_it_Tables.lexTable.size;

					//Проверка на максимальную длинну
					if(lexemesBuffer[i].size() - 2 > 251)
						throw ERROR_THROW_IN(317, lineNumber, lineColumn);

					// create name
					temp.id = "L_"+ to_string(literalIndex++);
	
					temp.iddatatype = IT::IDDATATYPE::STR;

					temp.idtype = IT::IDTYPE::L;

					// забил длинну
					if ((lexemesBuffer[i].size() - 2) > TI_STR_MAXSIZE)
						throw ERROR_THROW_IN(316, lineNumber, lineColumn);

					temp.value.vstr.len = lexemesBuffer[i].size() - 2;

					// забил саму строку
					size_t count = 1;
					for (; count < lexemesBuffer[i].size()-1; count++)
					{
						temp.value.vstr.str[count - 1] = lexemesBuffer[i][count];
					}
					temp.value.vstr.str[count-1] = '\0';

					temp.visibility = "-";

					IT::Add(lt_it_Tables.idTable, temp);

					//add to lex table
					Add(lt_it_Tables.lexTable, LT::Entry(lexemeName, lineNumber, lineColumn, lt_it_Tables.idTable.size - 1, lexemesBuffer[i]));
				}
				
				//char литерал
				else if (lexemesBuffer[i][0] == '\'')
				{
					IT::Entry temp;

					temp.idxfirstLE = lt_it_Tables.lexTable.size;

					// create name
					temp.id = "L_" + to_string(literalIndex++);

					temp.iddatatype = IT::IDDATATYPE::CHAR;

					temp.idtype = IT::IDTYPE::L;

					temp.value.vchar = lexemesBuffer[i][1];

					temp.visibility = "-";

					IT::Add(lt_it_Tables.idTable, temp);

					//add to lex table
					Add(lt_it_Tables.lexTable, LT::Entry(lexemeName, lineNumber, lineColumn, lt_it_Tables.idTable.size - 1, lexemesBuffer[i]));
				}

				//uint литерал
				else
				{
					IT::Entry temp;

					temp.idxfirstLE = lt_it_Tables.lexTable.size;

					temp.id = "L_" + to_string(literalIndex++);

					temp.iddatatype = IT::IDDATATYPE::UINT;

					temp.idtype = IT::IDTYPE::L;

					// забил значение для int
					if (lexemesBuffer[i][lexemesBuffer[i].size()-1] == 'h')
					{
						lexemesBuffer[i].pop_back();
						lexemesBuffer[i].pop_back();

						string s = lexemesBuffer[i];
						stringstream ss; ss << std::hex << s;
						ss >> temp.value.vint;
					}
					else
					{	//stoi(lexemesBuffer[i])
						
						temp.value.vint = strtoul(lexemesBuffer[i].c_str(), nullptr, 10);
					}
					
					temp.visibility = "-";

					IT::Add(lt_it_Tables.idTable, temp);

					//add to lex table
					Add(lt_it_Tables.lexTable, LT::Entry(lexemeName, lineNumber, lineColumn, lt_it_Tables.idTable.size - 1, lexemesBuffer[i]));
				}
			}

			//просто лексема
			else
			{
				//для смены области видимости на main
				if (lexemeName == LEX_MAIN) visibily = lexemesBuffer[i];

				//сигнал о том, что нужно закончить запись кол-ва переменных в функцию
				if (fParCheck && lexemeName == LEX_RIGHTTHESIS) 
					fParCheck = false;
				
				//обработчик если лексема - операция
				int priority = 0;
				switch (lexemesBuffer[i][0])
				{
					
				case '(':
				{
					priority = 1;
					break;
				}

				case ')':
				{
					priority = 1;
					break;
				}

				case '+':
				{
					priority = 2;
					break;
				}

				case '-':
				{
					priority = 2;
					break;
				}

				case '*':
				{
					priority = 3;
					break;
				}

				case '[':
				{
					priority = 4;
					break;
				}

				case ']':
				{
					priority = 4;
					break;
				}

				}

				//если это операция
				if (priority) 
				{
					Add(lt_it_Tables.lexTable, LT::Entry(lexemeName, lineNumber, lineColumn, LT_TI_NULLIDX, lexemesBuffer[i], priority));
				}
				else
				Add(lt_it_Tables.lexTable, LT::Entry(lexemeName , lineNumber, lineColumn, LT_TI_NULLIDX, lexemesBuffer[i]));
			}

			// для подсчета номера лексемы в строчке в исходнике(для удобного поиска ошибки)
			++lineColumn;
		}



		return lt_it_Tables;
	}

	vector<string> SeparateLexeme(string lexemes)
	{
		std::string separator = "$";
		std::vector<std::string> tempBuffer;
		int position;

		while ((position = lexemes.find(separator)) && position != -1)
		{
			tempBuffer.push_back(lexemes.substr(0, position));
			lexemes.erase(0, position + separator.length());
		}

		return tempBuffer;
	}

	vector<regularExpression> REBuffer =
	{
		regularExpression(LEX_DECLARE, 4,
		FST::NODE(1, FST::RELATION('v',1)),
		FST::NODE(1, FST::RELATION('a',2)),
		FST::NODE(1, FST::RELATION('r',3)),
		FST::NODE()
		),

		regularExpression(LEX_MAIN, 5,
		FST::NODE(1, FST::RELATION('m',1)),
		FST::NODE(1, FST::RELATION('a',2)),
		FST::NODE(1, FST::RELATION('i',3)),
		FST::NODE(1, FST::RELATION('n',4)),
		FST::NODE()
		),

		regularExpression(LEX_STRING, 7,
		FST::NODE(1, FST::RELATION('s',1)), //string
		FST::NODE(1, FST::RELATION('t',2)),
		FST::NODE(1, FST::RELATION('r',3)),
		FST::NODE(1, FST::RELATION('i',4)),
		FST::NODE(1, FST::RELATION('n',5)),
		FST::NODE(1, FST::RELATION('g',6)),
		FST::NODE()
		),

		regularExpression(LEX_U_INTEGER, 5,
		FST::NODE(1, FST::RELATION('u',1)),	//uint
		FST::NODE(1, FST::RELATION('i',2)),
		FST::NODE(1, FST::RELATION('n',3)),
		FST::NODE(1, FST::RELATION('t',4)),
		FST::NODE()
		),

		regularExpression(LEX_CHAR, 5,
		FST::NODE(1, FST::RELATION('c',1)),	//char
		FST::NODE(1, FST::RELATION('h',2)),
		FST::NODE(1, FST::RELATION('a',3)),
		FST::NODE(1, FST::RELATION('r',4)),
		FST::NODE()
		),

		regularExpression(LEX_VOID, 5,
		FST::NODE(1, FST::RELATION('v',1)),	//void
		FST::NODE(1, FST::RELATION('o',2)),
		FST::NODE(1, FST::RELATION('i',3)),
		FST::NODE(1, FST::RELATION('d',4)),
		FST::NODE()
		),

		regularExpression(LEX_FUNCTION, 9,
		FST::NODE(1, FST::RELATION('f',1)),
		FST::NODE(1, FST::RELATION('u',2)),
		FST::NODE(1, FST::RELATION('n',3)),
		FST::NODE(1, FST::RELATION('c',4)),
		FST::NODE(1, FST::RELATION('t',5)),
		FST::NODE(1, FST::RELATION('i',6)),
		FST::NODE(1, FST::RELATION('o',7)),
		FST::NODE(1, FST::RELATION('n',8)),
		FST::NODE()
		),

		regularExpression(LEX_RETURN, 7,
		FST::NODE(1, FST::RELATION('r',1)),
		FST::NODE(1, FST::RELATION('e',2)),
		FST::NODE(1, FST::RELATION('t',3)),
		FST::NODE(1, FST::RELATION('u',4)),
		FST::NODE(1, FST::RELATION('r',5)),
		FST::NODE(1, FST::RELATION('n',6)),
		FST::NODE()
		),

		regularExpression(LEX_WORK, 5,
		FST::NODE(1, FST::RELATION('w',1)), //work
		FST::NODE(1, FST::RELATION('o',2)),
		FST::NODE(1, FST::RELATION('r',3)),
		FST::NODE(1, FST::RELATION('k',4)),
		FST::NODE()
		),

		regularExpression(LEX_CASUAL, 7,
		FST::NODE(1, FST::RELATION('c',1)),	//casual
		FST::NODE(1, FST::RELATION('a',2)),
		FST::NODE(1, FST::RELATION('s',3)),
		FST::NODE(1, FST::RELATION('u',4)),
		FST::NODE(1, FST::RELATION('a',5)),
		FST::NODE(1, FST::RELATION('l',6)),
		FST::NODE()
		),

		regularExpression(LEX_PRINT, 6,
		FST::NODE(1, FST::RELATION('p',1)),
		FST::NODE(1, FST::RELATION('r',2)),	///
		FST::NODE(1, FST::RELATION('i',3)),
		FST::NODE(1, FST::RELATION('n',4)),
		FST::NODE(1, FST::RELATION('t',5)),
		FST::NODE()
		),

		regularExpression(LEX_BREAKL, 7,
		FST::NODE(1, FST::RELATION('B',1)),
		FST::NODE(1, FST::RELATION('R',2)),	///
		FST::NODE(1, FST::RELATION('E',3)),
		FST::NODE(1, FST::RELATION('A',4)),
		FST::NODE(1, FST::RELATION('K',5)),
		FST::NODE(1, FST::RELATION('L',6)),
		FST::NODE()
		),

		regularExpression(LEX_SEMICOLON, 2,
		FST::NODE(1, FST::RELATION(';', 1)),
		FST::NODE()
		),

		regularExpression(LEX_COMMA, 2,
		FST::NODE(1, FST::RELATION(',', 1)),
		FST::NODE()
		),

		regularExpression(LEX_LEFTTHESIS, 2,
		FST::NODE(1, FST::RELATION('(', 1)),
		FST::NODE()
		),

		regularExpression(LEX_RIGHTTHESIS, 2,
		FST::NODE(1, FST::RELATION(')', 1)),
		FST::NODE()
		),

		regularExpression(LEX_EQUALS, 2,
		FST::NODE(1, FST::RELATION('=', 1)),
		FST::NODE()
		),

		regularExpression(LEX_BRACELET, 2,
		FST::NODE(1, FST::RELATION('}', 1)),
		FST::NODE()
		),

		regularExpression(LEX_LEFTBRACE, 2,
		FST::NODE(1, FST::RELATION('{', 1)),
		FST::NODE()
		),

		regularExpression(LEX_PLUS, 2,
		FST::NODE(1, FST::RELATION('+', 1)),
		FST::NODE()
		),

		regularExpression(LEX_MINUS, 2,
		FST::NODE(1, FST::RELATION('-', 1)),
		FST::NODE()
		),

		regularExpression(LEX_STAR, 2,
		FST::NODE(1, FST::RELATION('*', 1)),
		FST::NODE()
		),

		regularExpression(LEX_LEFT_SHIFT, 2,
		FST::NODE(1, FST::RELATION('[', 1)),
		FST::NODE()
		),

		regularExpression(LEX_RIGHT_SHIFT, 2,
		FST::NODE(1, FST::RELATION(']', 1)),
		FST::NODE()
		),

		//Пересчитал
		regularExpression(LEX_LITERAL, 4,
		
		FST::NODE(32,
		FST::RELATION('1', 0), FST::RELATION('2', 0), FST::RELATION('3', 0), FST::RELATION('4', 0), FST::RELATION('5', 0), FST::RELATION('6', 0),
		FST::RELATION('7', 0), FST::RELATION('8', 0), FST::RELATION('9', 0), FST::RELATION('0', 0),

		FST::RELATION('1', 1), FST::RELATION('2', 1), FST::RELATION('3', 1), FST::RELATION('4', 1), FST::RELATION('5', 1), FST::RELATION('6', 1),
		FST::RELATION('7', 1), FST::RELATION('8', 1), FST::RELATION('9', 1), FST::RELATION('0', 1),

		FST::RELATION('A', 0), FST::RELATION('B', 0), FST::RELATION('C', 0), FST::RELATION('D', 0), FST::RELATION('E', 0), FST::RELATION('F', 0),
		FST::RELATION('A', 1), FST::RELATION('B', 1), FST::RELATION('C', 1), FST::RELATION('D', 1), FST::RELATION('E', 1), FST::RELATION('F', 1)),

		FST::NODE(1, FST::RELATION(':', 2)),

		FST::NODE(1, FST::RELATION('h', 3)),

		FST::NODE()
		),

		//Пересчитал
		regularExpression(LEX_LITERAL, 2,
		FST::NODE(20,
		FST::RELATION('1', 0), FST::RELATION('2', 0), FST::RELATION('3', 0), FST::RELATION('4', 0), FST::RELATION('5', 0), FST::RELATION('6', 0),
		FST::RELATION('7', 0), FST::RELATION('8', 0), FST::RELATION('9', 0),FST::RELATION('0', 0),

		FST::RELATION('1', 1), FST::RELATION('2', 1), FST::RELATION('3', 1), FST::RELATION('4', 1), FST::RELATION('5', 1), FST::RELATION('6', 1),
		FST::RELATION('7', 1), FST::RELATION('8', 1), FST::RELATION('9', 1), FST::RELATION('0', 1)),
		FST::NODE()
		),

		regularExpression(LEX_ID, 3,
			FST::NODE(159,
				FST::RELATION('a', 0), FST::RELATION('b', 0), FST::RELATION('c', 0), FST::RELATION('d', 0), FST::RELATION('e', 0), FST::RELATION('f', 0),
				FST::RELATION('g', 0), FST::RELATION('h', 0), FST::RELATION('i', 0), FST::RELATION('j', 0), FST::RELATION('k', 0), FST::RELATION('l', 0),
				FST::RELATION('m', 0), FST::RELATION('n', 0), FST::RELATION('o', 0), FST::RELATION('p', 0), FST::RELATION('q', 0), FST::RELATION('r', 0),
				FST::RELATION('s', 0), FST::RELATION('t', 0), FST::RELATION('u', 0), FST::RELATION('v', 0), FST::RELATION('w', 0), FST::RELATION('x', 0),
				FST::RELATION('y', 0), FST::RELATION('z', 0), FST::RELATION('_', 0),

				FST::RELATION('A', 0), FST::RELATION('B', 0), FST::RELATION('C', 0), FST::RELATION('D', 0), FST::RELATION('E', 0), FST::RELATION('F', 0),
				FST::RELATION('G', 0), FST::RELATION('H', 0), FST::RELATION('I', 0), FST::RELATION('J', 0), FST::RELATION('K', 0), FST::RELATION('L', 0),
				FST::RELATION('M', 0), FST::RELATION('N', 0), FST::RELATION('O', 0), FST::RELATION('P', 0), FST::RELATION('Q', 0), FST::RELATION('R', 0),
				FST::RELATION('S', 0), FST::RELATION('T', 0), FST::RELATION('U', 0), FST::RELATION('V', 0), FST::RELATION('W', 0), FST::RELATION('X', 0),
				FST::RELATION('Y', 0), FST::RELATION('Z', 0),

				FST::RELATION('a', 1), FST::RELATION('b', 1), FST::RELATION('c', 1), FST::RELATION('d', 1), FST::RELATION('e', 1), FST::RELATION('f', 1),
				FST::RELATION('g', 1), FST::RELATION('h', 1), FST::RELATION('i', 1), FST::RELATION('j', 1), FST::RELATION('k', 1), FST::RELATION('l', 1),
				FST::RELATION('m', 1), FST::RELATION('n', 1), FST::RELATION('o', 1), FST::RELATION('p', 1), FST::RELATION('q', 1), FST::RELATION('r', 1),
				FST::RELATION('s', 1), FST::RELATION('t', 1), FST::RELATION('u', 1), FST::RELATION('v', 1), FST::RELATION('w', 1), FST::RELATION('x', 1),
				FST::RELATION('y', 1), FST::RELATION('z', 1), FST::RELATION('_', 1),

				FST::RELATION('A', 1), FST::RELATION('B', 1), FST::RELATION('C', 1), FST::RELATION('D', 1), FST::RELATION('E', 1), FST::RELATION('F', 1),
				FST::RELATION('G', 1), FST::RELATION('H', 1), FST::RELATION('I', 1), FST::RELATION('J', 1), FST::RELATION('K', 1), FST::RELATION('L', 1),
				FST::RELATION('M', 1), FST::RELATION('N', 1), FST::RELATION('O', 1), FST::RELATION('P', 1), FST::RELATION('Q', 1), FST::RELATION('R', 1),
				FST::RELATION('S', 1), FST::RELATION('T', 1), FST::RELATION('U', 1), FST::RELATION('V', 1), FST::RELATION('W', 1), FST::RELATION('X', 1),
				FST::RELATION('Y', 1), FST::RELATION('Z', 1),

				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				FST::RELATION('a', 2), FST::RELATION('b', 2), FST::RELATION('c', 2), FST::RELATION('d', 2), FST::RELATION('e', 2), FST::RELATION('f', 2),
				FST::RELATION('g', 2), FST::RELATION('h', 2), FST::RELATION('i', 2), FST::RELATION('j', 2), FST::RELATION('k', 2), FST::RELATION('l', 2),
				FST::RELATION('m', 2), FST::RELATION('n', 2), FST::RELATION('o', 2), FST::RELATION('p', 2), FST::RELATION('q', 2), FST::RELATION('r', 2),
				FST::RELATION('s', 2), FST::RELATION('t', 2), FST::RELATION('u', 2), FST::RELATION('v', 2), FST::RELATION('w', 2), FST::RELATION('x', 2),
				FST::RELATION('y', 2), FST::RELATION('z', 2), FST::RELATION('_', 2),

				FST::RELATION('A', 2), FST::RELATION('B', 2), FST::RELATION('C', 2), FST::RELATION('D', 2), FST::RELATION('E', 2), FST::RELATION('F', 2),
				FST::RELATION('G', 2), FST::RELATION('H', 2), FST::RELATION('I', 2), FST::RELATION('J', 2), FST::RELATION('K', 2), FST::RELATION('L', 2),
				FST::RELATION('M', 2), FST::RELATION('N', 2), FST::RELATION('O', 2), FST::RELATION('P', 2), FST::RELATION('Q', 2), FST::RELATION('R', 2),
				FST::RELATION('S', 2), FST::RELATION('T', 2), FST::RELATION('U', 2), FST::RELATION('V', 2), FST::RELATION('W', 2), FST::RELATION('X', 2),
				FST::RELATION('Y', 2), FST::RELATION('Z', 2)
				),

			FST::NODE(30,
				FST::RELATION('1', 0), FST::RELATION('2', 0), FST::RELATION('3', 0), FST::RELATION('4', 0),
				FST::RELATION('5', 0), FST::RELATION('6', 0), FST::RELATION('7', 0), FST::RELATION('8', 0), FST::RELATION('9', 0), FST::RELATION('0', 0),

				FST::RELATION('1', 1), FST::RELATION('2', 1), FST::RELATION('3', 1), FST::RELATION('4', 1),
				FST::RELATION('5', 1), FST::RELATION('6', 1), FST::RELATION('7', 1), FST::RELATION('8', 1), FST::RELATION('9', 1), FST::RELATION('0', 1),

				FST::RELATION('1', 2), FST::RELATION('2', 2), FST::RELATION('3', 2), FST::RELATION('4', 2),
				FST::RELATION('5', 2), FST::RELATION('6', 2), FST::RELATION('7', 2), FST::RELATION('8', 2), FST::RELATION('9', 2), FST::RELATION('0', 2)

				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			),

			FST::NODE()
		),

		//Пересчитал
		regularExpression(LEX_LITERAL, 4,
			FST::NODE(1, FST::RELATION('\'', 1)),
			FST::NODE(70,
				FST::RELATION('a', 2), FST::RELATION('b', 2), FST::RELATION('c', 2), FST::RELATION('d', 2), FST::RELATION('e', 2), FST::RELATION('f', 2),
				FST::RELATION('g', 2), FST::RELATION('h', 2), FST::RELATION('i', 2), FST::RELATION('j', 2), FST::RELATION('k', 2), FST::RELATION('l', 2),
				FST::RELATION('m', 2), FST::RELATION('n', 2), FST::RELATION('o', 2), FST::RELATION('p', 2), FST::RELATION('q', 2), FST::RELATION('r', 2),
				FST::RELATION('s', 2), FST::RELATION('t', 2), FST::RELATION('u', 2), FST::RELATION('v', 2), FST::RELATION('w', 2), FST::RELATION('x', 2),
				FST::RELATION('y', 2), FST::RELATION('z', 2), 

				FST::RELATION('A', 2), FST::RELATION('B', 2), FST::RELATION('C', 2), FST::RELATION('D', 2), FST::RELATION('E', 2), FST::RELATION('F', 2),
				FST::RELATION('G', 2), FST::RELATION('H', 2), FST::RELATION('I', 2), FST::RELATION('J', 2), FST::RELATION('K', 2), FST::RELATION('L', 2),
				FST::RELATION('M', 2), FST::RELATION('N', 2), FST::RELATION('O', 2), FST::RELATION('P', 2), FST::RELATION('Q', 2), FST::RELATION('R', 2),
				FST::RELATION('S', 2), FST::RELATION('T', 2), FST::RELATION('U', 2), FST::RELATION('V', 2), FST::RELATION('W', 2), FST::RELATION('X', 2),
				FST::RELATION('Y', 2), FST::RELATION('Z', 2),

				FST::RELATION(' ', 2), FST::RELATION('.', 2), FST::RELATION(',', 2),
				FST::RELATION('?', 2), FST::RELATION('!', 2), FST::RELATION(':', 2), FST::RELATION('-', 2),
				FST::RELATION('_', 2),

				FST::RELATION('1', 2), FST::RELATION('2', 2), FST::RELATION('3', 2), FST::RELATION('4', 2),
				FST::RELATION('5', 2), FST::RELATION('6', 2), FST::RELATION('7', 2), FST::RELATION('8', 2), FST::RELATION('9', 2), FST::RELATION('0', 2)
				),
			FST::NODE(1, FST::RELATION('\'', 3)),
			FST::NODE()
		),


		//Пересчитал
		regularExpression(LEX_LITERAL, 4,
			FST::NODE(1, FST::RELATION('\"', 1)),
			FST::NODE(141,
				FST::RELATION('a', 1), FST::RELATION('b', 1), FST::RELATION('c', 1), FST::RELATION('d', 1), FST::RELATION('e', 1), FST::RELATION('f', 1),
				FST::RELATION('g', 1), FST::RELATION('h', 1), FST::RELATION('i', 1), FST::RELATION('j', 1), FST::RELATION('k', 1), FST::RELATION('l', 1),
				FST::RELATION('m', 1), FST::RELATION('n', 1), FST::RELATION('o', 1), FST::RELATION('p', 1), FST::RELATION('q', 1), FST::RELATION('r', 1),
				FST::RELATION('s', 1), FST::RELATION('t', 1), FST::RELATION('u', 1), FST::RELATION('v', 1), FST::RELATION('w', 1), FST::RELATION('x', 1),
				FST::RELATION('y', 1), FST::RELATION('z', 1),

				FST::RELATION('A', 1), FST::RELATION('B', 1), FST::RELATION('C', 1), FST::RELATION('D', 1), FST::RELATION('E', 1), FST::RELATION('F', 1),
				FST::RELATION('G', 1), FST::RELATION('H', 1), FST::RELATION('I', 1), FST::RELATION('J', 1), FST::RELATION('K', 1), FST::RELATION('L', 1),
				FST::RELATION('M', 1), FST::RELATION('N', 1), FST::RELATION('O', 1), FST::RELATION('P', 1), FST::RELATION('Q', 1), FST::RELATION('R', 1),
				FST::RELATION('S', 1), FST::RELATION('T', 1), FST::RELATION('U', 1), FST::RELATION('V', 1), FST::RELATION('W', 1), FST::RELATION('X', 1),
				FST::RELATION('Y', 1), FST::RELATION('Z', 1),

				FST::RELATION('.', 1), FST::RELATION(',', 1),
				FST::RELATION('?', 1), FST::RELATION('!', 1), FST::RELATION(':', 1), FST::RELATION('-', 1),
				FST::RELATION('_', 1), FST::RELATION(' ', 1),

				FST::RELATION('1', 1), FST::RELATION('2', 1), FST::RELATION('3', 1), FST::RELATION('4', 1),
				FST::RELATION('5', 1), FST::RELATION('6', 1), FST::RELATION('7', 1), FST::RELATION('8', 1), FST::RELATION('9', 1), FST::RELATION('0', 1),

				FST::RELATION('a', 2), FST::RELATION('b', 2), FST::RELATION('c', 2), FST::RELATION('d', 2), FST::RELATION('e', 2), FST::RELATION('f', 2),
				FST::RELATION('g', 2), FST::RELATION('h', 2), FST::RELATION('i', 2), FST::RELATION('j', 2), FST::RELATION('k', 2), FST::RELATION('l', 2),
				FST::RELATION('m', 2), FST::RELATION('n', 2), FST::RELATION('o', 2), FST::RELATION('p', 2), FST::RELATION('q', 2), FST::RELATION('r', 2),
				FST::RELATION('s', 2), FST::RELATION('t', 2), FST::RELATION('u', 2), FST::RELATION('v', 2), FST::RELATION('w', 2), FST::RELATION('x', 2),
				FST::RELATION('y', 2), FST::RELATION('z', 2),

				FST::RELATION('A', 2), FST::RELATION('B', 2), FST::RELATION('C', 2), FST::RELATION('D', 2), FST::RELATION('E', 2), FST::RELATION('F', 2),
				FST::RELATION('G', 2), FST::RELATION('H', 2), FST::RELATION('I', 2), FST::RELATION('J', 2), FST::RELATION('K', 2), FST::RELATION('L', 2),
				FST::RELATION('M', 2), FST::RELATION('N', 2), FST::RELATION('O', 2), FST::RELATION('P', 2), FST::RELATION('Q', 2), FST::RELATION('R', 2),
				FST::RELATION('S', 2), FST::RELATION('T', 2), FST::RELATION('U', 2), FST::RELATION('V', 2), FST::RELATION('W', 2), FST::RELATION('X', 2),
				FST::RELATION('Y', 2), FST::RELATION('Z', 2),

				FST::RELATION('.', 2), FST::RELATION(',', 2),
				FST::RELATION('?', 2), FST::RELATION('!', 2), FST::RELATION(';', 2), FST::RELATION(':', 2), FST::RELATION('-', 2),
				FST::RELATION('_', 2), FST::RELATION(' ', 2),

				FST::RELATION('1', 2), FST::RELATION('2', 2), FST::RELATION('3', 2), FST::RELATION('4', 2),
				FST::RELATION('5', 2), FST::RELATION('6', 2), FST::RELATION('7', 2), FST::RELATION('8', 2), FST::RELATION('9', 2), FST::RELATION('0', 2)
				),
			FST::NODE(1, FST::RELATION('\"', 3)),
			FST::NODE()
		),
	};

	regularExpression::regularExpression(char lN, short tN, FST::NODE n, ...)
	{
		transitionsNumber = tN;
		nodes = new FST::NODE[tN];
		FST::NODE* p = &n;
		lexemeName = lN;

		for (short k = 0; k < tN; k++)
			nodes[k] = p[k];
	}

	char GetLexemeName(string lexeme, size_t l, size_t c)
	{
		for (size_t i = 0; i < REBuffer.size(); i++)
		{
			FST::FST tempFST(lexeme.c_str(), REBuffer[i].transitionsNumber, REBuffer[i].nodes);

			if (execute(tempFST)) return REBuffer[i].lexemeName;
		}

		throw ERROR_THROW_IN(109, l, c);
	}
}