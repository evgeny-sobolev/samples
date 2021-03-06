﻿#pragma once
#include "Basket.h"
#include <vector>


class BasketManager
{
public:
	
	// Конструктор
	//
	// @param minValue		// Минимальное значение
	// @param maxValue		// Максимальное значение
	// @param memorySize	// Размер памяти машины
	BasketManager(int minValue, int maxValue, unsigned int memorySize);
	
	// Деструктор
	~BasketManager();

	// Считывает значения из файла и разносит по корзинам
	//
	// @param filePath	Путь к файлу
	void FillBaskets(const std::string& filePath);

	// Сортирует корзины в возрастающем порядке порядке
	void SortBaskets();

	// Объединяет корзины в один файл
	//
	// @param filePath	Результативный файл
	void CombineBaskets(const std::string& filePath);

// Функции
private:

	// Добавляет значение в корзину
	//
	// @param value	Добавляемое значение
	inline void AddValue(int value);

	// Создает корзины
	//
	// @param minValue		Минимальное значение
	// @param maxValue		Максимальное значение
	// @param memorySize	Размер памяти машины
	void CreateBaskets(int minValue, int maxValue, unsigned int memorySize);
	
// Дата
private:

	// Коллекция корзин
	std::vector<Basket*> mBaskets;

	// Память машины
	unsigned int mMemorySize;
};


