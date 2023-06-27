#include "PolishLT.h"

namespace PolishNatation
{
	void convertToPN(LT::LexTable& lexTable, IT::IdTable& idTable)
	{
		//польская нужна после return = и void 
		for (size_t i = 0; i < lexTable.size; i++)
		{
			

			if (lexTable.table[i].lexema == LEX_EQUALS ||
				lexTable.table[i].lexema == LEX_RETURN ||
				idTable.table[lexTable.table[i].idxTI].iddatatype == IT::IDDATATYPE::VOID && lexTable.table[i-1].lexema != LEX_FUNCTION)
			{
				stack<LT::Entry> operations;

				string reservOperations = "()[]+-*";

				size_t numOfVoid = 0;

				//Отловил void вызов
				if (idTable.table[lexTable.table[i].idxTI].iddatatype == IT::IDDATATYPE::VOID && lexTable.table[i-1].lexema != LEX_FUNCTION)
				{
					numOfVoid += idTable.table[lexTable.table[i].idxTI].funcParams.size() - 1;

					LT::Entry remembFId = lexTable.table[i];

					//Тоже нужен доп. счетчик, чтобы не сбивать основной(i)
					size_t j = i + 2;

					////Буфер параметров, чтобы потом поменять их местами	//////////////////////////////////////////////////////////////////
					//vector<LT::Entry> tempEbuffer;

					while (lexTable.table[j].lexema != LEX_RIGHTTHESIS)
					{
						if (lexTable.table[j].lexema != LEX_COMMA)
						{
							lexTable.table[i++] = lexTable.table[j];
							//tempEbuffer.push_back(lexTable.table[j]);
						}
							
							++j;
					}
						

					////Перезаполняю параметры в обратном порядке
					//--j;
					//for (size_t i = 0; i < idTable.table[remembFId.idxTI].funcParams.size() ; j--, i++)
					//{
					//	lexTable.table[j] = tempEbuffer[i];
					//}


					lexTable.table[i++] = LT::Entry('@');

					//
					lexTable.table[i++] = to_string(idTable.table[remembFId.idxTI].funcParams.size())[0];

					lexTable.table[i] = remembFId;

					while (numOfVoid--)
					{
						lexTable.table[++i] = LT::Entry('#');
					}

					continue;
				}


				//(i - идем по таблице, j - для преобразования в польской)
				 
				//+1 чтобы перейти к тому, что после return или =
				size_t j = i+1;
				for (; lexTable.table[j].lexema != LEX_SEMICOLON; j++)
				{
					if (reservOperations.find(lexTable.table[j].lexema) == -1)
					{
						//если встретили функцию, записываем сначала параметры, а мотом @ + кол-во парамс.
						if (idTable.table[lexTable.table[j].idxTI].idtype == IT::IDTYPE::F)
						{
							// 2 скобки - 1(т.к добавляется @) -1(т.к добавляется число пар-ов) + кол-во запятых(кол-во парам. - 1)
							numOfVoid += idTable.table[lexTable.table[j].idxTI].funcParams.size() - 1;

							//запомнили индекс функции ТАК НЕЛЬЗЯ, ТАК КАК ИНДЕКСЫ СЪЕЗЖАЮТ (!скопировать узел!)
							LT::Entry remembFId = lexTable.table[j];

							//перешли на следующий после ( вызова функции(на первый i)
							j+=2;

							while (lexTable.table[j].lexema != LEX_RIGHTTHESIS)
							{
								if(lexTable.table[j].lexema != LEX_COMMA) 
									lexTable.table[++i] = lexTable.table[j];

								//идем по параметрам
								++j;
								
							}
							
							
							//скорее всего это НЕ ВЕРНО
							////перешли на следующий после ) вызова функции
							//++j;
							
							//добавил @
							lexTable.table[++i] = LT::Entry('@');

							//добавил кол-во параметров
							lexTable.table[++i] = to_string( idTable.table[remembFId.idxTI].funcParams.size() )[0];
																								
							//добавил сам id функции
							lexTable.table[++i] = remembFId;
						}
						else
						lexTable.table[++i] = lexTable.table[j];
					}
						
					
					else
					{
						//Если встретили ) ( увеличиваем счетчик пустых символов
						if (lexTable.table[j].lexema == '(' || lexTable.table[j].lexema == ')') ++numOfVoid;

						if (operations.empty() || lexTable.table[j].lexema == '(')
							operations.push(lexTable.table[j]);

						else if (/*!operations.empty() &&*/ lexTable.table[j].lexema == ')')
						{
							//аналогично
							while (/*!operations.empty() &&*/ operations.top().lexema != '(')
							{
								lexTable.table[++i] = operations.top();
								operations.pop();

							}
							operations.pop();
						}

						else
						{
							//аналогично
							while (!operations.empty() && operations.top().priority >= lexTable.table[j].priority)
							{
								lexTable.table[++i] = operations.top();
								operations.pop();
							}
							operations.push(lexTable.table[j]);
						}

					}

				}

				//записываю, что осталось в стеке
				while (!operations.empty())
				{
					lexTable.table[++i] = operations.top();
					operations.pop();
				}

				//добавил  ; в конец польской
				lexTable.table[++i] = lexTable.table[j];

				//Оставшееся место до ; нужно ж еще заполнит #
				while (numOfVoid--)
				{
					lexTable.table[++i] = LT::Entry('#');
				}

			}
			
		}

	}

}