#include "Basket.h"
#include <algorithm>
#include <vector>

// ---
Basket::Basket(int minValue, int maxValue, const std::string& filePath, unsigned int allowedBasketSize, unsigned int memorySize)
	: mMinValue(minValue)
	, mMaxValue(maxValue)
	, mFilePath(filePath)
	, mBasketMemory(allowedBasketSize)
	, mMachineMemory(memorySize)
{
	if (0 != fopen_s(&mFile, filePath.c_str(), "wb+") || allowedBasketSize < sizeof(int))
	{
		throw std::exception("Не удалось создать файл-корзину"); 
	}

	mCache.maxSize = allowedBasketSize / sizeof(int);

	mCache.ptr = new int[mCache.maxSize];
}

// ---
Basket::~Basket()
{
	if (mFile)
	{
		fclose(mFile);
	}
	
	FileDelete();
}

// ---
int Basket::IsInRange(int value)
{
	if (value < mMinValue)
	{
		return -1;
	}
	
	if (value > mMaxValue)
	{
		return 1;
	}
	
	return 0;
}

// ---
void Basket::FileSort()
{
	// Сорт:  просто загружаем числа в вектор и вызываем функцию сорта (std::sort)
	fseek(mFile, 0, SEEK_END);

	long fileSize = ::ftell(mFile);

	//if (fileSize > Max Machine Memory Size)
	//{
	//	TODO:
	//	Это может случиться если файл забит числами из одного диапозона. В этом случае нам необходимо
	//	повторить процесс разбиения файла на файлы-корзины до тех пор пока каждая корзина не будет
	//	превышать размер дуступной памяти. 
	//}
	
	if (fileSize < sizeof(int))
	{
		return;
	}
	
	fseek(mFile, 0, SEEK_SET);

	const int elementsCount = fileSize / sizeof(int);

	int* buffer = new int[elementsCount];

	if (fread(buffer, sizeof(int), elementsCount, mFile) == elementsCount)
	{
		std::vector<int> vect((size_t)elementsCount);

		vect.assign(buffer, buffer + elementsCount);
			
		sort(vect.begin(), vect.end());

		std::copy(vect.begin(), vect.end(), buffer);

		fseek(mFile, 0, SEEK_SET);
			
		fwrite(buffer, sizeof(int), elementsCount, mFile);
	}
}

// ---
void Basket::FileDelete()
{
	remove(mFilePath.c_str());
}

// ---
void Basket::AddValue(int value)
{
	if (mCache.size == mCache.maxSize)
	{
		// Кэш достиг лимита, сбрасываем данные в файл
		FileWrite();
	}

	mCache.ptr[mCache.size] = value;

	mCache.size++;
}

// ---
void Basket::FileWrite()
{
	if (mFile && mCache.ptr && mCache.size > 0)
	{
		fwrite(mCache.ptr, sizeof(int), mCache.size, mFile);

		mCache.size = 0;
	}
}

// ---
void Basket::FileAppendTo(FILE* const file)
{
	if (!file || !mFile)
	{
		return;
	}
	
	fseek(mFile, 0, SEEK_END);

	long size = ftell(mFile);

	if (size == 0)
	{
		// Нечего копировать
		return;
	}

	fseek(mFile, 0, SEEK_SET);

	char* buffer = new char[size];

	fread(buffer, 1, size, mFile);
		
	fwrite(buffer, 1, size, file);

	delete [] buffer;
}
