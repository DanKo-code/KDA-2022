#include <iostream>
#include <Windows.h>


extern "C"
{
	unsigned int _stdcall casual(unsigned int max)
	{
		srand(time(NULL));
		return rand() % max;
	}

	void _stdcall printUint(unsigned int a)
	{
		std::cout << a;
	}

	void _stdcall printString(char* ptr)
	{
		for (size_t i = 1; i <= (unsigned char)ptr[0]; i++)
		{
			std::cout << ptr[i];
		}
	}
						
	 char* _stdcall shiftChar(char* direction, char* symbol, unsigned int siftsNum)
	 {										//18
		char charLiteralValue[] = {		     'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
											 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
											 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' ', '.',
											 ',', '?', '!', ';', ':', '-', '_', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };

		unsigned int index = 0;
		for (size_t i = 0; i < 71; i++)
		{
			if (symbol[1] == charLiteralValue[i])
			{
				index = i;
				break;
			}
		}

		if (direction[1] == '[')
		{
			for (size_t i = 0; i < siftsNum; i++)
			{
				if (!index)
				{
					index = 70;
				}
				else
					--index;
			}
		}
		else
		{
			for (size_t i = 0; i < siftsNum; i++)
			{
				if (index == 70)
				{
					index = 0;
				}
				else
					++index;
			}
		}

		char* ret = (char*)malloc(256);

		ret[0] = symbol[0];
		ret[1] = charLiteralValue[index];

		return ret;
	 }

	 void _stdcall stringCopy(char* buffer, char* cop)
	 {
		 memset(buffer, 0, 256);

		 buffer[0] = cop[0];

		 for (size_t i = 1; i <= (unsigned char)cop[0]; i++)
		 {
			 buffer[i] = cop[i];
		 }
	 }

	 char* __stdcall strconcat(char* s1, char* s2)
	 {
		 char* ret = (char*)malloc(256);
		 
		 memset(ret, 0, 256);

		 ret[0] = (unsigned char)s1[0] + (unsigned char)s2[0];

		 for (int i = 0; i < (unsigned char)s1[0]; i++)
			 ret[i + 1] = s1[i + 1];

		 for (int i = 0; i < (unsigned char)s2[0]; i++)
			 ret[i + 1 + (unsigned char)s1[0]] = s2[i + 1];
		 
		 return ret;
	 }
 
	 unsigned int __stdcall checkstringconcat(char* s1, char* s2)
	 {
		 if ((unsigned int)(unsigned char)s1[0] + (unsigned int)(unsigned char)s2[0] > 251)
		 {
			 return 0;
		 }
		 return 1;
	 }

	 void _stdcall BREAKL()
	{
		std::cout << std::endl;
	}

	 void _stdcall print_error_les_zero()
	 {
		 SetConsoleCP(1251);
		 SetConsoleOutputCP(1251);

		 std::cout << "Результат вычислений меньше нуля\n";
	 }

	 void _stdcall print_error_str_literal_length_exceeded()
	 {
		 SetConsoleCP(1251);
		 SetConsoleOutputCP(1251);

		 std::cout << "При конкатенации строк была превышена максимальная длинна литерала. Максимальная длинна string литерала = 251\n";
	 }
}