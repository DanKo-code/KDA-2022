#include "Error.h"

namespace Error
{
	ERROR errors[ERROR_MAX_ENTRY]
	{
		//Системные
		ERROR_ENTRY(0, "{System} Недопустимый код ошибки"),
		ERROR_ENTRY(1, "{System} Системный сбой"),
		ERROR_ENTRY(2, "{System} Превышена длина входного параметра"),//in.parm
		ERROR_ENTRY(3, "{System} Параметр -in должен быть задан"),//in.parm
		ERROR_ENTRY(4, "{System} Ошибка при открытии файла с исходным кодом (-in)"),//in.cpp
		ERROR_ENTRY(5, "{System} Исходный файл (-in) пуст"),
		ERROR_ENTRY(6, "{System} Не доступный символ в исходном файле (-in)"),//in.cpp
		ERROR_ENTRY(7, "{System} Ошибка при создании файла протокола (-log)"),//log.cpp
		ERROR_ENTRY(8, "{System} Ошибка при создании выходного файла (-out)"),//out.cpp
		ERROR_ENTRY_NODEF(9), ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40), ERROR_ENTRY_NODEF10(50),
		ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),
		
		//Лексические
		ERROR_ENTRY(100, "{Lexis} Слишком большой размер таблицы лексем"),//LT.cpp
		ERROR_ENTRY(101, "{Lexis} Размер таблицы  лексем привысел максимальный возможный"),//LT.cpp
		ERROR_ENTRY(102, "{Lexis} Слишком большой размер таблицы идентификаторов"),//IT.cpp
		ERROR_ENTRY(103, "{Lexis} Размер таблицы идентификаторов привысел максимальный возможный"),//IT.cpp
		ERROR_ENTRY(104, "{Lexis} Ошибка лексического анализа"),
		ERROR_ENTRY(105, "{Lexis} Неверный индекс таблицы идентификаторов"),//IT.cpp
		ERROR_ENTRY(106, "{Lexis} Неверный индекс таблицы лексем"),//LT.cpp
		ERROR_ENTRY(107, "{Lexis} Ошибка при создании файла вывода тиблицы идентификаторов"),//IT.cpp
		ERROR_ENTRY(108, "{Lexis} Ошибка при создании файла вывода тиблицы лексем"),//LT.cpp
		ERROR_ENTRY(109, "{Lexis} Незарезервированная лексема"),//LexicalAnalysis.cpp
		ERROR_ENTRY(110, "{Lexis} ?"),
		ERROR_ENTRY(111, "{Lexis} ?"),
		ERROR_ENTRY(112, "{Lexis} ?"),
		ERROR_ENTRY(113, "{Lexis} ?"),
		ERROR_ENTRY(114, "{Lexis} ?"),
		ERROR_ENTRY(115, "{Lexis} ?"),
		ERROR_ENTRY(116, "{Lexis} ?"),
		ERROR_ENTRY(117, "{Lexis} ?"),
		ERROR_ENTRY(118, "{Lexis} ?"),
		ERROR_ENTRY(119, "{Lexis} ?"),
		ERROR_ENTRY_NODEF10(120), ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140), ERROR_ENTRY_NODEF10(150), ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170),
		ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),

		//Синтаксические
		ERROR_ENTRY(200, "{Syntax} Неверная структура программы"),
		ERROR_ENTRY(201, "{Syntax} Ошибочный оператор"),
		ERROR_ENTRY(202, "{Syntax} Ошибка в выражении"),
		ERROR_ENTRY(203, "{Syntax} Ошибка в выражении с операторами"),
		ERROR_ENTRY(204, "{Syntax} Ошибка в параметры функции"),
		ERROR_ENTRY(205, "{Syntax} Ошибка в подвыражении"),
		ERROR_ENTRY(206, "{Syntax} Ошибка в логике тела цикла"),
		ERROR_ENTRY(207, "{Syntax} Неверая структура print/casual"),
		ERROR_ENTRY(208, "{Syntax} ?"),
		ERROR_ENTRY(209, "{Syntax} ?"),
		ERROR_ENTRY_NODEF10(210), ERROR_ENTRY_NODEF10(220), ERROR_ENTRY_NODEF10(230), ERROR_ENTRY_NODEF10(240), ERROR_ENTRY_NODEF10(250), ERROR_ENTRY_NODEF10(260),
		ERROR_ENTRY_NODEF10(270), ERROR_ENTRY_NODEF10(280), ERROR_ENTRY_NODEF10(290),

		//Семантический
		ERROR_ENTRY(300, "{Semantics} Присутствует более одной точки вхождения в программу"),//SemanticsAnalysis.cpp
		ERROR_ENTRY(301, "{Semantics} Не найдено ниодной точки входа в программу"),			 //SemanticsAnalysis.cpp
		ERROR_ENTRY(302, "{Semantics} Переопределение идентификатора функции"),		//LexicalAnalysis.cpp
		ERROR_ENTRY(303, "{Semantics} Идентификатор не определен"),					//LexicalAnalysis.cpp
		ERROR_ENTRY(304, "{Semantics} Функция должна вызываться с параметрами"),	//LexicalAnalysis.cpp
		ERROR_ENTRY(305, "{Semantics} Переопределение идентификатора переменной"),	//LexicalAnalysis.cpp
		ERROR_ENTRY(306, "{Semantics} Переменная не может быть типа void"),			//LexicalAnalysis.cpp
		ERROR_ENTRY(307, "{Semantics} Несовпадение типов"),		//SemanticsAnalysis.cpp
		ERROR_ENTRY(308, "{Semantics} Недопустимый операнд в string rvalue, допустим только +"), //SemanticsAnalysis.cpp
		ERROR_ENTRY(309, "{Semantics} Недопустимая структура char rvalue"),						 //SemanticsAnalysis.cpp

		ERROR_ENTRY(310, "{Semantics} Несоответствие типа передаваемого параметра"),			 //SemanticsAnalysis.cpp
		ERROR_ENTRY(311, "{Semantics} Слишком мало аргументов в вызове функции"),				 //SemanticsAnalysis.cpp
		ERROR_ENTRY(312, "{Semantics} Слишком много аргументов в вызове функции"),
		ERROR_ENTRY(313, "{Semantics} Превышен максимальный лимит параметров функции(максимум допустимо 3)"),
		ERROR_ENTRY(314, "{Semantics} Неверное использование casual"),
		ERROR_ENTRY(315, "{Semantics} Превышена длинна строкового литерала(максимум 256)"),
		ERROR_ENTRY(316, "{Semantics} VOID функция не может возвращать значение"),
		ERROR_ENTRY(317, "{Semantics} Превышена максимальная длинна литерала. Максимальная длинна string литерала = 251"),
		ERROR_ENTRY(318, "{Semantics} ?"),
		ERROR_ENTRY(319, "{Semantics} ?"),
		ERROR_ENTRY_NODEF10(320), ERROR_ENTRY_NODEF10(330), ERROR_ENTRY_NODEF10(340), ERROR_ENTRY_NODEF10(350), ERROR_ENTRY_NODEF10(360), ERROR_ENTRY_NODEF10(370),
		ERROR_ENTRY_NODEF10(380), ERROR_ENTRY_NODEF10(390), ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500), ERROR_ENTRY_NODEF100(600), ERROR_ENTRY_NODEF100(700), ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900),
	};
	
	ERROR geterror(int id)
	{
		if (0 > id || id > ERROR_MAX_ENTRY) return errors[0];
		else return errors[id];
	}

	ERROR geterrorin(int id, int line = -1, int col = -1)
	{
		if (0 > id || id > ERROR_MAX_ENTRY) return errors[0];
		else
		{
			errors[id].inext.line = line;
			errors[id].inext.col = col;
			return errors[id];
		}
	}
};
