#pragma warning(disable : 4996) // _itoa unsafe

#include "BasketManager.h"
#include <fstream>

// ---
BasketManager::BasketManager(int minValue, int maxValue, unsigned int memorySize)
	: mMemorySize(memorySize)
{
	CreateBaskets(minValue, maxValue, memorySize);
}

// ---
BasketManager::~BasketManager()
{
	// Освобождаем память
	for (int index = 0; index < (int)mBaskets.size(); ++index)
	{
		delete mBaskets[index];
	}
}

// ---
void BasketManager::CreateBaskets(int minValue, int maxValue, unsigned int memorySize)
{
	// Используем 50% доступной памяти, чтобы избежать тот случай где размер файла-корзины 
	// превышает размер доступной памяти и невозможно загрузить файл в память целиком.
	const unsigned int allowedMemory = memorySize >> 1;

	// Пр: max=1, min=0 это 2 элемента
	const unsigned int elementsRange = maxValue - minValue + 1;

	if (elementsRange == 0)
	{
		// Весь файл забит одним числом!
	}

	// Кол-во чисел которое мы можем загрузить в память машины
	const int elementCountInMachineMemory = allowedMemory / sizeof(int);

	// Кол-во необходимых корзин
	const int basketCount = elementsRange / elementCountInMachineMemory + 1;

	// Если количество корзин меньше 2х, то файл полностью может быть загружен в память. Пропускаю этот случай...

	// Вычисляем размер вспомогательного кэша в корзинах
	const int allowMemoryPerBasket = memorySize / basketCount;

	char bufferName[10];
	
	// Создаём корзины
	for (int index = 0; index < basketCount; ++index)
	{
		int min = minValue + index * elementCountInMachineMemory;
		
		int max = min + elementCountInMachineMemory;
		
		char *fileName = _itoa(index, bufferName, 10);

		Basket* basket = new Basket(min, max, std::string(fileName).c_str(), allowMemoryPerBasket, allowedMemory);
		
		mBaskets.push_back(basket);
	}
}

// ---
void BasketManager::AddValue(int value)
{
	// Для лучшей производительности значения в векторе можно
	// посортировать и реализовать здесь бинарный поиск, но для простоты
	// просто линейный поиск..
	for (int index = 0; index < (int)mBaskets.size(); ++index)
	{
		if (0 == mBaskets[index]->IsInRange(value))
		{
			mBaskets[index]->AddValue(value);

			break;
		}
	}
}

// ---
void BasketManager::FillBaskets(const std::string& filePath)
{
	const int bufferSize = mMemorySize / sizeof(int);
	
	FILE* file = fopen(filePath.c_str(), "rb");

	if (file)
	{
		int* buffer = new int[bufferSize];

		size_t read(0);

		fseek(file, 0, SEEK_SET);

		while (read = fread(buffer, sizeof(int), bufferSize, file))
		{
			for (size_t index = 0; index < read; ++index)
			{
				AddValue(buffer[index]);
			}
		}

		delete [] buffer;

		fclose(file);
	}

	for (int index = 0; index < (int)mBaskets.size(); ++index)
	{
		mBaskets[index]->FileWrite();
	}
}

// ---
void BasketManager::SortBaskets()
{
	for (int index = 0; index < (int)mBaskets.size(); ++index)
	{
		mBaskets[index]->FileSort();
	}
}

// ---
void BasketManager::CombineBaskets(const std::string& filePath)
{
	FILE* file = ::fopen(filePath.c_str(), "wb");

	if (!file)
	{
		return;
	}

	for (size_t index = 0; index < mBaskets.size(); ++index)
	{
		mBaskets[index]->FileAppendTo(file);
	}

	fclose(file);
}
