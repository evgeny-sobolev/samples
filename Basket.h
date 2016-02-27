#pragma once
#include "IBasket.h"
#include <string>
#include <memory>

// Корзина значений
class Basket : IBasket
{
public:
	
	// Конструктор
	//
	// @param minValue				Минимальная граница диапазона (включительно)
	// @param maxValue				Максимальная граница диапазона (включительно)
	// @param filePath				Путь к файлу-корзине на диске
	// @param allowedBasketSize		Размер кэша корзины
	// @param memorySize			Размер памяти машины
	Basket(int minValue, int maxValue, const std::string& filePath, unsigned int allowedBasketSize, unsigned int memorySize);
	
	// Деструктор
	virtual ~Basket();

	// Тестирует число на принадлежность к диапазону
	// @param value	 Тестируемое значение
	// @return -1 до диапазона, 0 в диапазоне, +1 за диапазоном
	int IsInRange(int value);

	// Сортирует файл в восходящем порядке
	void FileSort();

	// Удаляет файл с диска
	void FileDelete();

	// Добавляет число в корзину
	// @param value	Добавляемое значение
	void AddValue(int value);

	// Записывает кэш корзины в файл
	void FileWrite();

	// Записывает файл-корзину в конец указанного файла
	// @param file	Файл в который записывается содержимое корзины
	void FileAppendTo(FILE* const file);


private:
	// Начало диапазона корзины
	int mMinValue;
	
	// Конец диапазона корзины
	int mMaxValue;

	// Файл корзины
	FILE *mFile;

	// Путь к файлу корзины
	std::string mFilePath;

	// Размер памяти машины
	unsigned int mMachineMemory;

	// Размер кэша корзины
	unsigned int mBasketMemory;

	// Кэш хранения чисел
	struct Cache
	{
		int* ptr;		// Указатель на данные
		int size;		// Текущий размер данных
		int maxSize;	// Размер кэша

		Cache() 
			: ptr(0)
			, size(0)
			, maxSize(0)
		{ }

		~Cache()
		{
			if (ptr)
			{
				delete[] ptr;
			}
		}

	} mCache;
};
