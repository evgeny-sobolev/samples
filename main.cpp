#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <time.h>
#include "BasketManager.h"

/*
	ОПИСАНИЕ:
	Небольшая программа, демонстрирующая написание программного кода на языке С++. Так как
	программа является демонстративной, используются основные средства языка без ссылок
	на нестандартные или сторонние библиотеки.

	ЗАДАЧА:
	Размер оперативной памяти компьютера 2 гигабайта. Размер жесткого диска - 3 терабайта.
	На жестком диски находится файл размером 1 терабайт который заполнен 32-битовыми, случайными,
	целыми числами. Необходимо создать новый файл в котором все числа из первого файла будут
	сортированы в возрастающем порядке. Сортировка файла должна быть эффективной по времени.

	РЕШЕНИЕ:
	Существует несколько эффективных решений для данной задачи. Наиболее простое - создание счетчиков
	для каждого значения в диапазоне. Так как все счетчики не поместятся в память машины, следует
	реализовать механизмы хранения некоторых счетчиков на диске и кэширования части счетчиков в
	доступной памяти.

	Другие решение, более интересное и реализованное в данной программе - корзины с определёнными
	диапазонами. Так как мы знаем минимальное и максимальное значения 32-битового целого значения,
	а так же количество доступной нам памяти, мы можем определить некоторые диапазоны чисел. Так
	как все корзины не поместятся в память, для каждой корзины будет создан файл на диске. В конечном
	итоге каждая корзина будет иметь значения только своего диапазона. Если корзина полностью помещается
	в память, значения в ней можно сортировать; если не помещается, корзина разбирается на дополнительные
	диапазоны и процесс повторяет себя до тех пор пока корзина полностью не помещается в память.
	Когда каждая из существующих корзин помещается в память, корзины сортируются и объединяются
	в финальный файл.

	ЗАМЕТКА:
	Для удобства тестирования и демонстрации решения, создадим файл размером 1 мегабайт и предположим,
	что доступная память компьютера равна 10 килобайтам.

	Автор: Соболев Е.В.
*/

// Прототипы функций
bool GenerateSourceFile(const std::string&, unsigned int);

bool FindMinMaxValues(const std::string&, int&, int&, const unsigned int);

// Входящая точка программы
int main()
{
	// Доступная память машины: 10 КБ
	const unsigned int MACHINE_MEMORY_SIZE = 0x2800;
	
	// Вызовите функцию для генерации тестового файла
	//GenerateSourceFile("x:\\input.dat", 0x100000 / sizeof(int));

	// Шаг 1: найдём min и max значения в файле
	int minValue(0);
	int maxValue(0);

	if (!FindMinMaxValues("x:\\input.dat", minValue, maxValue, MACHINE_MEMORY_SIZE))
	{
		std::cout << "Не удалось найти min и max значения в input.dat" << std::endl;
		
		return 0;
	}
				
	// Шаг 2: учитывая min, max и доспутную память, расчитаем сколько корзин нам необходимо и их диапазоны
	BasketManager bm(minValue, maxValue, MACHINE_MEMORY_SIZE);

	//  Шаг 3: считываем значения из файла кусками и забрасываем в корзины
	bm.FillBaskets("x:\\input.dat");

	//  Шаг 4: сорт корзин
	bm.SortBaskets();

	// Шаг финальный: объединяем все корзины в один файл
	bm.CombineBaskets("x:\\output.dat");

	std::cout << "output.dat создан успешно" << std::endl;

	return 0;
}

// Создаёт файл и заполняет случайными целыми числами типа int
//
// @param	filePath		Путь к файлу
// @param	sizeElements	Кол-во элементов
// @return	True если успех
bool GenerateSourceFile(const std::string& filePath, unsigned int elementCount)
{
	if (elementCount == 0)
	{
		return false;
	}

	FILE* file(0); 
		
	if (0 != fopen_s(&file, filePath.c_str(), "wb"))
	{
		return false;
	}
		
	int* buffer = new int[elementCount];

	srand(time(NULL));

	for (unsigned int index = 0; index < elementCount; ++index)
	{
		buffer[index] = rand();
	}

	fwrite(buffer, sizeof(int), (size_t)elementCount, file);

	delete [] buffer;

	fclose(file);

	return true;
}

// Сканирует все int значения в файле и находит min и max.
//
// @param	filePath		Путь к файлу
// @param	minValue [out]	Минимальное значение
// @param	maxValue [out]	Максимальное значение
// @param	memorySize		Доступный размер памяти машины
// @return	True если успех
bool FindMinMaxValues(const std::string& filePath, int& minValue, int& maxValue, const unsigned int memorySize)
{
	if (memorySize == 0)
	{
		return false;
	}
	
	FILE* file(0);

	if (0 != fopen_s(&file, filePath.c_str(), "rb"))
	{
		return false;
	}

	// 32-bit padding
	const int bufferSize = memorySize / sizeof(int);

	int* buffer = new int[bufferSize];

	size_t read(0);
	
	bool firstRead(true);

	bool result(false);

	while (read = fread(buffer, sizeof(int), bufferSize, file))
	{
		if (firstRead)
		{
			minValue = maxValue = buffer[0];
			
			firstRead = false;
			
			result = true;
		}

		for (size_t index = 0; index < read; ++index)
		{
			if (buffer[index] < minValue)
			{
				minValue = buffer[index];
			}

			if (buffer[index] > maxValue)
			{
				maxValue = buffer[index];
			}
		}
	}

	delete [] buffer;

	fclose(file);
	
	return result;
}
