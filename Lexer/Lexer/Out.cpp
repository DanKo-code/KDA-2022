#include "Out.h"

#include "Error.h"

#pragma warning(disable:4996)
using namespace std;

namespace Out {

	OUT getout(wchar_t outfile[]) //logfile – имя входного файла
	{
		OUT out;
		out.stream = new ofstream;
		out.stream->open(outfile);
		if (out.stream->fail()) //если поток не создался, генерируется исключение(ERRROR_THROW, код	ошибки 112);
			throw ERROR_THROW(8);
		wcscpy_s(out.outfile, outfile); //Функция wcscpy () используется для копирования 
		//строки широких символов из источника в место назначения. logfile - исходный файл, log.logfile - куда копируем строку

		return out; //возврат заполненной структуры LOG.
	}

	void WriteOut(OUT out, In::IN in)
	{
		*out.stream << in.spaceLexeme;
	}

	////Используется для вывода в протокол или на консоль информации об ошибке
	//void WriteError(OUT out, Error::ERROR error)//параметры: структура LOG и структура IN
	//{
	//	if (out.stream)
	//	{
	//		*out.stream << "Ошибка " << error.id
	//			<< ": " << error.message
	//			<< "\nСтрока " << error.inext.line
	//			<< " позиция " << error.inext.col << endl;
	//		Close(out);
	//	}
	//	else //если протокол не открыт, выводит информацию на консоль
	//	{
	//		cout << "\nОшибка " << error.id << ": " << error.message << "\nСтрока " << error.inext.line << " позиция " << error.inext.col << endl;
	//	}
	//}

	////Используется для закрытия выходного потока протокола
	//void Close(OUT out) //параметр: структура LOG
	//{
	//	out.stream->close();
	//	delete out.stream;
	//}
}