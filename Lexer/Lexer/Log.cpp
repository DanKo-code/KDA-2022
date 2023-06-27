#include "Log.h"

#pragma warning(disable:4996)

namespace Log
{
	//открывает (создает) выходной поток
	//записывает данные в структуру LOG. 
	LOG getlog(wchar_t logfile[]) //logfile – имя входного файла
	{
		LOG log;
		log.stream = new ofstream;
		log.stream->open(logfile);
		if (log.stream->fail()) //если поток не создался, генерируется исключение(ERRROR_THROW, код	ошибки 112);
			throw ERROR_THROW(7);
		wcscpy_s(log.logfile, logfile); //Функция wcscpy () используется для копирования 
		//строки широких символов из источника в место назначения. logfile - исходный файл, log.logfile - куда копируем строку

		return log; //возврат заполненной структуры LOG.
	}

	//осуществляет конкатенацию всех строк, заданных параметрами, формирует строку и выводит ее в протокол.
	void WriteLine(LOG log, const char* c, ...) // параметры: структура LOG, переменное число параметров типа char*
	{
		const char** ptr = &c;
		int i = 0;
		while (ptr[i] != "")
			*log.stream << ptr[i++];
		*log.stream << endl;
	}
	void WriteLine(LOG log, const wchar_t* c, ...)
	{
		const wchar_t** ptr = &c;
		char temp[100];
		int i = 0;
		while (ptr[i] != L"")
		{
			wcstombs(temp, ptr[i++], sizeof(temp)); //Эта функция преобразует строку широких символов, 
			//первый элемент которой указан как ptr[i++], в ее многобайтовое представление. Результат сохраняется в temp, 
			//sizeof(temp) - максимальное кол-во символов для преобразования
			*log.stream << temp;
		}
		*log.stream << endl;
	}

	// выводит строку заголовка в протокол
	void WriteLog(LOG log) //параметр:структура LOG
	{
		//для получения текущей даты и времени в формате строки используются функции time, localtime_s и strftime.
		tm* timeinf;	//структура хранящая текущее время
		time_t timet;
		time(&timet);	//заполнякм структуру
		timeinf = localtime(&timet);	//текущее локальное время, представленное в структуре
		char temp[100];
		strftime(temp, sizeof(temp), "\n------------ПРОТОКОЛ------------\n ----%d.%m.%y %T----\n ", timeinf);
		*log.stream << temp;
	}
	void WriteIn(LOG log, In::IN in)
	{
		*log.stream << "\n---- Исходные данные ----\n"
			<< "\n\nВсего символов: " << in.size
			<< "\n\nВсего строк: " << in.lines
			<< "\n\nПропущено: " << in.ignor << endl;
	}

	//Используется для вывода в протокол информации о входных параметрах
	void WriteParm(LOG log, Parm::PARM parm) //параметры: структура LOG и структура PARM
	{
		char in_text[PARM_MAX_SIZE];
		char out_text[PARM_MAX_SIZE];
		char log_text[PARM_MAX_SIZE];
		wcstombs(in_text, parm.in, PARM_MAX_SIZE);
		wcstombs(out_text, parm.out, PARM_MAX_SIZE);
		wcstombs(log_text, parm.log, PARM_MAX_SIZE);
		*log.stream << "\n ---- Параметры ---- \n\n-in: " << in_text
			<< "\n-out: " << out_text
			<< "\n-log: " << log_text << endl;
	}

	//Используется для вывода в протокол или на консоль информации об ошибке
	void WriteError(LOG log, Error::ERROR error)//параметры: структура LOG и структура IN
	{
		if (log.stream)
		{
			//WriteLog(log);

			*log.stream << "Ошибка " << error.id
				<< ": " << error.message
				<< "\nСтрока " << error.inext.line
				<< " позиция " << error.inext.col;
			Close(log);
		}
		else //если протокол не открыт, выводит информацию на консоль
		{
			cout << "\nОшибка " << error.id << ": " << error.message << "\nСтрока " << error.inext.line << " позиция " << error.inext.col << endl;
		}
	}

	//Используется для закрытия выходного потока протокола
	void Close(LOG log) //параметр: структура LOG
	{
		log.stream->close();
		delete log.stream;
	}

}