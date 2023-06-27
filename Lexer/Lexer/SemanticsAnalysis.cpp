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

		std::cout << std::endl << std::endl << "������������� ������: ��� ������" << std::endl << std::endl;
	}

	// �������� main. ������ 1 main
	//��������
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

	// �������� �� ������������ ���� ���������� � rvalue
	void CompLit_VarType()
	{
		for (size_t i = 0; i < lexTable.size; i++)
		{
			//���� ����� = ��� return
			if (lexTable.table[i].lexema == LEX_EQUALS || lexTable.table[i].lexema == LEX_RETURN)
			{
				IT::IDDATATYPE type;

				//���� ������� =
				if (lexTable.table[i].lexema == LEX_EQUALS)
					type = idTable.table[lexTable.table[i - 1].idxTI].iddatatype;
				//���� ������� return
				else if (lexTable.table[i].lexema == LEX_RETURN)
				{
					//��� � �� function � ������ ������ ���� �����.(���� �������) - > function + 1 

					//����, ����� �� ����� ����� �������
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
					
				//������ ����� ��� ������ �� = ��� � ������������ �������� �������
				switch (type)
				{


					case IT::IDDATATYPE::UINT:
					{
						//����� ��������� ������� ����� =/return
						++i;

						for (; lexTable.table[i].lexema != LEX_SEMICOLON; i++)
						{
							//������� � CASUAL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

							//��� ���� ������� ������� ������ �����. . � uint ����� ������������ ��� ��������� 
							if (lexTable.table[i].idxTI != LT_TI_NULLIDX)
							{
								if (idTable.table[lexTable.table[i].idxTI].iddatatype != IT::IDDATATYPE::UINT)
									throw ERROR_THROW_IN(307, lexTable.table[i].sn, lexTable.table[i].csn);

								//��� ��������� �����. ���������� �����.
								funcParmProcess(i);
							}
						}

						break;
					}

					case IT::IDDATATYPE::STR:
					{
						//����� ��������� ������� ����� =/return
						++i;

						for (; lexTable.table[i].lexema != LEX_SEMICOLON; i++)
						{
							//������� � CASUAL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

							//��������� ( )
							if (lexTable.table[i].lexema == LEX_LEFTTHESIS || lexTable.table[i].lexema == LEX_RIGHTTHESIS)
								continue;

							//� string ����� ���� ������ +
							if (lexTable.table[i].idxTI == LT_TI_NULLIDX)
							{
								if(lexTable.table[i].lexema != LEX_PLUS) 
									throw ERROR_THROW_IN(308, lexTable.table[i].sn, lexTable.table[i].csn);
							}

							//� string ������ string
							if (lexTable.table[i].idxTI != LT_TI_NULLIDX)
							{
								if (idTable.table[lexTable.table[i].idxTI].iddatatype != IT::IDDATATYPE::STR)
									throw ERROR_THROW_IN(307, lexTable.table[i].sn, lexTable.table[i].csn);

								//��� ��������� �����. ���������� �����.
								funcParmProcess(i);
							}
						}

						break;
					}

					case IT::IDDATATYPE::CHAR:
					{
						// � char ����� ���� ������ 1) = char 
						//							2) = char [ uint 
						//                          3) = uint ] char

						//���������� �� ��, ��� ����� =/return
						++i;

						//������� � CASUAL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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

						//��� ��������� �����. ���������� �����.
						funcParmProcess(i);
						++i;				//��� �������� � [ ]

						if (first_char && lexTable.table[i].lexema == LEX_LEFT_SHIFT)
						{
							//������� � CASUAL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
							if (lexTable.table[i+1].lexema == LEX_CASUAL)
								throw ERROR_THROW_IN(314, lexTable.table[i].sn, lexTable.table[i].csn);

							if (idTable.table[lexTable.table[++i].idxTI].iddatatype == IT::IDDATATYPE::UINT)
							{
								//��� ��������� �����. ���������� �����.
								funcParmProcess(i);
								++i;		//��� �������� � [ ]

								if (lexTable.table[i].lexema == LEX_SEMICOLON) break;
							}
						}

						else if (first_uint && lexTable.table[i].lexema == LEX_RIGHT_SHIFT)
						{
							//������� � CASUAL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
							if (lexTable.table[i + 1].lexema == LEX_CASUAL)
								throw ERROR_THROW_IN(314, lexTable.table[i].sn, lexTable.table[i].csn);

							if (idTable.table[lexTable.table[++i].idxTI].iddatatype == IT::IDDATATYPE::CHAR)
							{
								//��� ��������� �����. ���������� �����.
								funcParmProcess(i);
								++i;		//��� �������� � [ ]

								if (lexTable.table[i].lexema == LEX_SEMICOLON) break;
							}
						}
						
						// ��� ��� �� ����, ��� ������ ������ � ������ �������
						throw ERROR_THROW_IN(309, lexTable.table[i].sn, -1);
					}
						
				}

			}
		}
	}

	//�������� �� void return
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
			//���� ������� �������� �� id � ��
			int fIdIndex = index;

			//��� �������� �� ������������
			int i = 0;

			//��������� ���� ����. � ( 
			index+=2;

			while (lexTable.table[index].lexema != LEX_RIGHTTHESIS)
			{
				//���� �����. 
				if (lexTable.table[index].idxTI != LT_TI_NULLIDX)
				{
					//�� ������, ���� �������� ������ ���-�� ��� �����
					if(i+1 > idTable.table[lexTable.table[fIdIndex].idxTI].funcParams.size())
						throw ERROR_THROW_IN(312, lexTable.table[index].sn, lexTable.table[index].csn);
				
					//������� ��������� �� ��� ��� � ����� ��������� ���������� �����.
					if (idTable.table[lexTable.table[index].idxTI].iddatatype !=
						idTable.table[lexTable.table[fIdIndex].idxTI].funcParams[i++])	//���� ��������� �����. ����� ���������, ��� ������������ � �������� � vector<IT::IDDATATYPE> funcParams
					{
						throw ERROR_THROW_IN(310, lexTable.table[index].sn, lexTable.table[index].csn);
					}
				}

				++index;
			}

			//��� �������� �� ������������ ���-�� ������������ ���-�� � �����������������(���� ������������ ������)
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
				//����� ��������� �� id �������
				throw ERROR_THROW_IN(313, lexTable.table[i].sn, lexTable.table[i+1].csn);
			}
		}
	}
}