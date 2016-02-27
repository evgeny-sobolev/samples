#pragma once
#include <stdio.h>

// Для решения этой задачи интерфейс не тредуется, но для демонстративных целей создадим
class IBasket
{
public:
	
	IBasket() {}

	~IBasket() {}

	// Тестирует число на принадлежность к диапазону
	// @param value	 Тестируемое значение
	// @return -1 до диапазона, 0 в диапазоне, +1 за диапазоном
	virtual int IsInRange(int value) = 0;

	// Добавляет число в корзину
	// @param value	Добавляемое значение
	virtual void AddValue(int value) = 0;

	// Сортирует файл в восходящем порядке
	virtual void FileSort() = 0;

	// Удаляет файл с диска
	virtual void FileDelete() = 0;

	// Записывает кэш корзины в файл
	virtual void FileWrite() = 0;

	// Записывает файл-корзину в конец указанного файла
	// @param file	Файл в который записывается содержимое корзины
	virtual void FileAppendTo(FILE* const file) = 0;
};
