#include "SemanticsAnalysis.h"

#include "Error.h"
#include <string>
#include <vector>

#include <iostream>


namespace SemanticsAnalysis
{
	LT::LexTable lexTable;
	IT::IdTable idTable;

	void SemAnalys(LT::LexTable LT, IT::IdTable ID)
	{
		lexTable = LT;
		idTable = ID;

		MainCheck();

		CompLit_VarType();

		CheckParamsNumb();

		CheckVoidFuncRet();

		std::cout << std::endl << std::endl << "Семантический анализ: Без ошибок" << std::endl << std::endl;
	}

	// Ниодного main. больше 1 main
	//Проверил
	void MainCheck()
	{
		int mainCount = 0;

		for (size_t i = 0; i < lexTable.size; i++)
		{
			if(lexTable.table[i].lexema == LEX_MAIN) ++mainCount;

			if (mainCount > 1) 
				throw ERROR_THROW_IN(300, lexTable.table[i].sn, lexTable.table[i].csn);
		}
		
		if(!mainCount) throw ERROR_THROW(301);
	}

	// Проверка на соответствие типа переменной и rvalue
	void CompLit_VarType()
	{
		for (size_t i = 0; i < lexTable.size; i++)
		{
			//если нашел = или return
			if (lexTable.table[i].lexema == LEX_EQUALS || lexTable.table[i].lexema == LEX_RETURN)
			{
				IT::IDDATATYPE type;

				//если попался =
				if (lexTable.table[i].lexema == LEX_EQUALS)
					type = idTable.table[lexTable.table[i - 1].idxTI].iddatatype;
				//если попался return
				else if (lexTable.table[i].lexema == LEX_RETURN)
				{
					//ищу в ТЛ function и смотрю какого типа идент.(наша функция) - > function + 1 

					//ввел, чтобы не сбить общий счетчик
					int j = i;

					for (;; j--)
					{
						if (lexTable.table[j].lexema == LEX_FUNCTION)
						{
							type = idTable.table[lexTable.table[j + 1].idxTI].iddatatype;
							break;
						}
							
					}
				}
					
				//смотрю какой тип данных до = или в возвращаемом значении функции
				switch (type)
				{


					case IT::IDDATATYPE::UINT:
					{
						//берем следующую лексему после =/return
						++i;

						for (; lexTable.table[i].lexema != LEX_SEMICOLON; i++)
						{
							//Вставка с CASUAL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

							//При этом условии выбираю только идент. . В uint можно использовать все операторы 
							if (lexTable.table[i].idxTI != LT_TI_NULLIDX)
							{
								if (idTable.table[lexTable.table[i].idxTI].iddatatype != IT::IDDATATYPE::UINT)
									throw ERROR_THROW_IN(307, lexTable.table[i].sn, lexTable.table[i].csn);

								//для обработки парам. вызываемой функц.
								funcParmProcess(i);
							}
						}

						break;
					}

					case IT::IDDATATYPE::STR:
					{
						//берем следующую лексему после =/return
						++i;

						for (; lexTable.table[i].lexema != LEX_SEMICOLON; i++)
						{
							//Вставка с CASUAL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

							//допускаем ( )
							if (lexTable.table[i].lexema == LEX_LEFTTHESIS || lexTable.table[i].lexema == LEX_RIGHTTHESIS)
								continue;

							//в string может быть только +
							if (lexTable.table[i].idxTI == LT_TI_NULLIDX)
							{
								if(lexTable.table[i].lexema != LEX_PLUS) 
									throw ERROR_THROW_IN(308, lexTable.table[i].sn, lexTable.table[i].csn);
							}

							//в string только string
							if (lexTable.table[i].idxTI != LT_TI_NULLIDX)
							{
								if (idTable.table[lexTable.table[i].idxTI].iddatatype != IT::IDDATATYPE::STR)
									throw ERROR_THROW_IN(307, lexTable.table[i].sn, lexTable.table[i].csn);

								//для обработки парам. вызываемой функц.
								funcParmProcess(i);
							}
						}

						break;
					}

					case IT::IDDATATYPE::CHAR:
					{
						// в char может быть только 1) = char 
						//							2) = char [ uint 
						//                          3) = uint ] char

						//передвинул на то, что после =/return
						++i;

						//Вставка с CASUAL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
						if (lexTable.table[i].lexema == LEX_CASUAL)
							throw ERROR_THROW_IN(314, lexTable.table[i].sn, lexTable.table[i].csn);

						if (idTable.table[lexTable.table[i].idxTI].iddatatype == IT::IDDATATYPE::CHAR &&
							 lexTable.table[i + 1].lexema == LEX_SEMICOLON)
						{
							break;
						}

						bool first_char = false;
						bool first_uint = false;

						
						if (idTable.table[lexTable.table[i].idxTI].iddatatype == IT::IDDATATYPE::CHAR) first_char = true;
						else if (idTable.table[lexTable.table[i].idxTI].iddatatype == IT::IDDATATYPE::UINT) first_uint = true;

						//для обработки парам. вызываемой функц.
						funcParmProcess(i);
						++i;				//для перехода к [ ]

						if (first_char && lexTable.table[i].lexema == LEX_LEFT_SHIFT)
						{
							//Вставка с CASUAL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
							if (lexTable.table[i+1].lexema == LEX_CASUAL)
								throw ERROR_THROW_IN(314, lexTable.table[i].sn, lexTable.table[i].csn);

							if (idTable.table[lexTable.table[++i].idxTI].iddatatype == IT::IDDATATYPE::UINT)
							{
								//для обработки парам. вызываемой функц.
								funcParmProcess(i);
								++i;		//для перехода к [ ]

								if (lexTable.table[i].lexema == LEX_SEMICOLON) break;
							}
						}

						else if (first_uint && lexTable.table[i].lexema == LEX_RIGHT_SHIFT)
						{
							//Вставка с CASUAL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
							if (lexTable.table[i + 1].lexema == LEX_CASUAL)
								throw ERROR_THROW_IN(314, lexTable.table[i].sn, lexTable.table[i].csn);

							if (idTable.table[lexTable.table[++i].idxTI].iddatatype == IT::IDDATATYPE::CHAR)
							{
								//для обработки парам. вызываемой функц.
								funcParmProcess(i);
								++i;		//для перехода к [ ]

								if (lexTable.table[i].lexema == LEX_SEMICOLON) break;
							}
						}
						
						// так как не знаю, где именно ошибка в данной строчке
						throw ERROR_THROW_IN(309, lexTable.table[i].sn, -1);
					}
						
				}

			}
		}
	}

	//Проверка на void return
	void CheckVoidFuncRet()
	{
		bool voidFunc = false;

		for (size_t i = 0; i < lexTable.size; i++)
		{
			if (lexTable.table[i].idxTI != LT_TI_NULLIDX &&
				idTable.table[lexTable.table[i].idxTI].iddatatype == IT::IDDATATYPE::VOID)
				voidFunc = !voidFunc;
			
			else if (lexTable.table[i].lexema == LEX_RETURN &&
				voidFunc &&
				lexTable.table[i + 1].lexema != LEX_SEMICOLON)
				throw ERROR_THROW_IN(316, lexTable.table[i].sn, lexTable.table[i].csn);	
		}
	}

	void funcParmProcess(size_t& index)
	{
		if (idTable.table[lexTable.table[index].idxTI].idtype == IT::IDTYPE::F)
		{
			//Если функция запомним ее id в ТЛ
			int fIdIndex = index;

			//для подсчета на соответствие
			int i = 0;

			//пропустил саму фунц. и ( 
			index+=2;

			while (lexTable.table[index].lexema != LEX_RIGHTTHESIS)
			{
				//если идент. 
				if (lexTable.table[index].idxTI != LT_TI_NULLIDX)
				{
					//на случай, если передали больше пар-ов чем нужно
					if(i+1 > idTable.table[lexTable.table[fIdIndex].idxTI].funcParams.size())
						throw ERROR_THROW_IN(312, lexTable.table[index].sn, lexTable.table[index].csn);
				
					//смотрим совпадает ли его тип с типом параметра вызываемой функц.
					if (idTable.table[lexTable.table[index].idxTI].iddatatype !=
						idTable.table[lexTable.table[fIdIndex].idxTI].funcParams[i++])	//если встретили идент. нужно увеличить, для соответствия с индексом в vector<IT::IDDATATYPE> funcParams
					{
						throw ERROR_THROW_IN(310, lexTable.table[index].sn, lexTable.table[index].csn);
					}
				}

				++index;
			}

			//для проверки на соответствие кол-ва передаваемых пар-ов с зарезервироваными(если передаваемых меньше)
			if( i != idTable.table[lexTable.table[fIdIndex].idxTI].funcParams.size()) 
				throw ERROR_THROW_IN(311, lexTable.table[index].sn, lexTable.table[index].csn);

			return;
		}

		return;
	}

	void CheckParamsNumb()
	{
		for (size_t i = 0; i < lexTable.size; i++)
		{
			if (lexTable.table[i].lexema == LEX_FUNCTION && idTable.table[lexTable.table[i+1].idxTI].funcParams.size() > 3)
			{
				//будет указывать на id функции
				throw ERROR_THROW_IN(313, lexTable.table[i].sn, lexTable.table[i+1].csn);
			}
		}
	}
}