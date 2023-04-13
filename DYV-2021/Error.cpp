#include "stdafx.h"
#include "Error.h"
namespace Error
{
	ERROR errors[ERROR_MAX_ENTRY] =
	{
		ERROR_ENTRY(0, "Недопустимый код ошибки"),
		ERROR_ENTRY(1, "Системный сбой"),
		ERROR_ENTRY(2, "Параметр -in должен быть задан"),
		ERROR_ENTRY(3, "Превышена длина входного параметра"),
		ERROR_ENTRY(4, "Ошибка при открытии файла с исходным кодом (-in)"),
		ERROR_ENTRY(5, "Ошибка при создании файла протокола (-log)"),
		ERROR_ENTRY(6, "Ошибка при создании .asm файла"),
		ERROR_ENTRY_NODEF(7), ERROR_ENTRY_NODEF(8), ERROR_ENTRY_NODEF(9),
		ERROR_ENTRY_NODEF10(10), ERROR_ENTRY_NODEF10(20), ERROR_ENTRY_NODEF10(30), ERROR_ENTRY_NODEF10(40), ERROR_ENTRY_NODEF10(50),
		ERROR_ENTRY_NODEF10(60), ERROR_ENTRY_NODEF10(70), ERROR_ENTRY_NODEF10(80), ERROR_ENTRY_NODEF10(90),
		ERROR_ENTRY_NODEF(100),
		ERROR_ENTRY(101, "Ошибка при открытии файла с промежуточным кодом (-out)"),
		ERROR_ENTRY(102, "Недопустимый символ в исходном файле (-in)"),
		ERROR_ENTRY(103, "Слишком много лексем"),
		ERROR_ENTRY(104, "Запрашивается несуществующая строка таблицы лексем"),
		ERROR_ENTRY(105, "Запрашивается несуществующий литерал"),
		ERROR_ENTRY(106, "Слишком много литералов"),
		ERROR_ENTRY_NODEF(107), ERROR_ENTRY_NODEF(108), ERROR_ENTRY_NODEF(109),
		ERROR_ENTRY(110, "Слишком много объектов"),
		ERROR_ENTRY(111, "Слишком много свойств"),
		ERROR_ENTRY(112, "Запрашивается несуществующий объект"),
		ERROR_ENTRY_NODEF(113),	ERROR_ENTRY_NODEF(114),	ERROR_ENTRY_NODEF(115),	ERROR_ENTRY_NODEF(116),	ERROR_ENTRY_NODEF(117),
		ERROR_ENTRY_NODEF(118),	ERROR_ENTRY_NODEF(119),
		ERROR_ENTRY_NODEF10(120), ERROR_ENTRY_NODEF10(130), ERROR_ENTRY_NODEF10(140), ERROR_ENTRY_NODEF10(150),
		ERROR_ENTRY_NODEF10(160), ERROR_ENTRY_NODEF10(170), ERROR_ENTRY_NODEF10(180), ERROR_ENTRY_NODEF10(190),
		ERROR_ENTRY(200, "Слишком длинный литерал"),
		ERROR_ENTRY(201, "Имя объекта или свойства слишком длинное"),
		ERROR_ENTRY(202, "Слишком глубокое дерево свойств"),
		ERROR_ENTRY(203, "Оператор ?:: может принимать как условие только .EQUALS"),
		ERROR_ENTRY(204, ".equals не имеет значения. Проверьте наличие .CHECK"),
		ERROR_ENTRY(205, "Вывод глобального объекта невозможен"),
		ERROR_ENTRY(206, "Недопустимо использование print"),
		ERROR_ENTRY(207, "EQUALS содержится только в глобальном объекте"),
		ERROR_ENTRY(208, "LOOP содержится только в глобальном объекте"),
		ERROR_ENTRY(209, "Использование операторов в левой части недоступно"),
		ERROR_ENTRY(210, "Деление на 0 запрещено"),
		ERROR_ENTRY(211, "CHECK содержится только в глобальном объекте"),
		ERROR_ENTRY(212, "Попытка создать системное свойство"),
		ERROR_ENTRY(213, "Неправильное использование ?::"),
		ERROR_ENTRY(214, "Обращение к необъявленному свойству"),
		ERROR_ENTRY(215, "Ошибка задания литерала"),
		ERROR_ENTRY(216, "APPEND может принимать только литералы"),
		ERROR_ENTRY_NODEF(217), ERROR_ENTRY_NODEF(218), ERROR_ENTRY_NODEF(219),
		ERROR_ENTRY_NODEF10(220), ERROR_ENTRY_NODEF10(230), ERROR_ENTRY_NODEF10(240), ERROR_ENTRY_NODEF10(250),
		ERROR_ENTRY_NODEF10(260), ERROR_ENTRY_NODEF10(270), ERROR_ENTRY_NODEF10(280), ERROR_ENTRY_NODEF10(290),
		ERROR_ENTRY_NODEF100(300), ERROR_ENTRY_NODEF100(400), ERROR_ENTRY_NODEF100(500),
		ERROR_ENTRY(600, "Неверная структура программы"),
		ERROR_ENTRY(601, "Ошибочный оператор"),
		ERROR_ENTRY(602, "Ошибка в выражении"),
		ERROR_ENTRY(603, "Ошибка разбора"),
		ERROR_ENTRY(604, "Некорректное множественное перечисление"),
		ERROR_ENTRY(605, "Критическая ошибка: лексема потеряна"),//может появиться лишь в коде компилятора
		ERROR_ENTRY_NODEF(606), ERROR_ENTRY_NODEF(607), ERROR_ENTRY_NODEF(608), ERROR_ENTRY_NODEF(609),
		ERROR_ENTRY_NODEF10(610), ERROR_ENTRY_NODEF10(620), ERROR_ENTRY_NODEF10(630), ERROR_ENTRY_NODEF10(640), ERROR_ENTRY_NODEF10(650),
		ERROR_ENTRY_NODEF10(660), ERROR_ENTRY_NODEF10(670), ERROR_ENTRY_NODEF10(680), ERROR_ENTRY_NODEF10(690),
		ERROR_ENTRY_NODEF100(700), ERROR_ENTRY_NODEF100(800), ERROR_ENTRY_NODEF100(900),
	};
	ERROR geterror(int id)
	{
		if ((id > 0) && (id < ERROR_MAX_ENTRY))
		{
			ERROR err = {errors[id].id};
			for (int i = 0; i < ERROR_MAXSIZE_MESSAGE; i++)
				err.message[i] = errors[id].message[i];
			throw err;
			return err;
		}
		else
		{
			ERROR err = {0};
			for (int i = 0; i < ERROR_MAXSIZE_MESSAGE; i++)
				err.message[i] = errors[0].message[i];
			throw err;
			return err;
		}
	}
	ERROR geterrorin(int id, int line = -1, int col = -1)
	{
		if ((id > 0) && (id < ERROR_MAX_ENTRY))
		{
			ERROR err = { errors[id].id};
			for (int i = 0; i < ERROR_MAXSIZE_MESSAGE; i++)
				err.message[i] = errors[id].message[i];
			err.index.line = line;
			err.index.col = col;
			throw err; 
			return err;
		}
		else
		{
			ERROR err = { 0 };
			for (int i = 0; i < ERROR_MAXSIZE_MESSAGE; i++)
				err.message[i] = errors[0].message[i];
			throw err;
			return err;
		}
	}
}