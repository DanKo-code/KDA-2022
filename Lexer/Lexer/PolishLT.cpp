#include "PolishLT.h"

namespace PolishNatation
{
	void convertToPN(LT::LexTable& lexTable, IT::IdTable& idTable)
	{
		//�������� ����� ����� return = � void 
		for (size_t i = 0; i < lexTable.size; i++)
		{
			

			if (lexTable.table[i].lexema == LEX_EQUALS ||
				lexTable.table[i].lexema == LEX_RETURN ||
				idTable.table[lexTable.table[i].idxTI].iddatatype == IT::IDDATATYPE::VOID && lexTable.table[i-1].lexema != LEX_FUNCTION)
			{
				stack<LT::Entry> operations;

				string reservOperations = "()[]+-*";

				size_t numOfVoid = 0;

				//������� void �����
				if (idTable.table[lexTable.table[i].idxTI].iddatatype == IT::IDDATATYPE::VOID && lexTable.table[i-1].lexema != LEX_FUNCTION)
				{
					numOfVoid += idTable.table[lexTable.table[i].idxTI].funcParams.size() - 1;

					LT::Entry remembFId = lexTable.table[i];

					//���� ����� ���. �������, ����� �� ������� ��������(i)
					size_t j = i + 2;

					////����� ����������, ����� ����� �������� �� �������	//////////////////////////////////////////////////////////////////
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
						

					////������������ ��������� � �������� �������
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


				//(i - ���� �� �������, j - ��� �������������� � ��������)
				 
				//+1 ����� ������� � ����, ��� ����� return ��� =
				size_t j = i+1;
				for (; lexTable.table[j].lexema != LEX_SEMICOLON; j++)
				{
					if (reservOperations.find(lexTable.table[j].lexema) == -1)
					{
						//���� ��������� �������, ���������� ������� ���������, � ����� @ + ���-�� ������.
						if (idTable.table[lexTable.table[j].idxTI].idtype == IT::IDTYPE::F)
						{
							// 2 ������ - 1(�.� ����������� @) -1(�.� ����������� ����� ���-��) + ���-�� �������(���-�� �����. - 1)
							numOfVoid += idTable.table[lexTable.table[j].idxTI].funcParams.size() - 1;

							//��������� ������ ������� ��� ������, ��� ��� ������� �������� (!����������� ����!)
							LT::Entry remembFId = lexTable.table[j];

							//������� �� ��������� ����� ( ������ �������(�� ������ i)
							j+=2;

							while (lexTable.table[j].lexema != LEX_RIGHTTHESIS)
							{
								if(lexTable.table[j].lexema != LEX_COMMA) 
									lexTable.table[++i] = lexTable.table[j];

								//���� �� ����������
								++j;
								
							}
							
							
							//������ ����� ��� �� �����
							////������� �� ��������� ����� ) ������ �������
							//++j;
							
							//������� @
							lexTable.table[++i] = LT::Entry('@');

							//������� ���-�� ����������
							lexTable.table[++i] = to_string( idTable.table[remembFId.idxTI].funcParams.size() )[0];
																								
							//������� ��� id �������
							lexTable.table[++i] = remembFId;
						}
						else
						lexTable.table[++i] = lexTable.table[j];
					}
						
					
					else
					{
						//���� ��������� ) ( ����������� ������� ������ ��������
						if (lexTable.table[j].lexema == '(' || lexTable.table[j].lexema == ')') ++numOfVoid;

						if (operations.empty() || lexTable.table[j].lexema == '(')
							operations.push(lexTable.table[j]);

						else if (/*!operations.empty() &&*/ lexTable.table[j].lexema == ')')
						{
							//����������
							while (/*!operations.empty() &&*/ operations.top().lexema != '(')
							{
								lexTable.table[++i] = operations.top();
								operations.pop();

							}
							operations.pop();
						}

						else
						{
							//����������
							while (!operations.empty() && operations.top().priority >= lexTable.table[j].priority)
							{
								lexTable.table[++i] = operations.top();
								operations.pop();
							}
							operations.push(lexTable.table[j]);
						}

					}

				}

				//���������, ��� �������� � �����
				while (!operations.empty())
				{
					lexTable.table[++i] = operations.top();
					operations.pop();
				}

				//�������  ; � ����� ��������
				lexTable.table[++i] = lexTable.table[j];

				//���������� ����� �� ; ����� � ��� �������� #
				while (numOfVoid--)
				{
					lexTable.table[++i] = LT::Entry('#');
				}

			}
			
		}

	}

}