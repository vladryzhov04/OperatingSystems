#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../Lab_2_os/Lab_2_os.cpp" 

namespace Lab2test
{
    TEST_CLASS(Lab2test)
    {
    public:

        // Тест поиска минимального и максимального элементов
        TEST_METHOD(FindMinMaxElementsTest)
        {
            const int size = 5;
            double arr[size] = { 3.0, 1.5, 4.2, 2.8, 5.1 };

            double minElement, maxElement;
            min_max_thread_function(arr, size, minElement, maxElement);

            // Проверка результатов
            Assert::AreEqual(1.5, minElement, L"Некорректный минимальный элемент");
            Assert::AreEqual(5.1, maxElement, L"Некорректный максимальный элемент");
        }

        // Тест вычисления среднего значения
        TEST_METHOD(CalculateAverageTest)
        {
            const int size = 4;
            double arr[size] = { 2.0, 4.0, 6.0, 8.0 };

            double averagesum;
            average_thread_function(arr, size, averagesum);

            // Проверка результатов
            Assert::AreEqual(5.0, averagesum, L"Некорректное среднее значение");
        }

        // Тест замены элементов в массиве
        TEST_METHOD(ReplaceElementsTest)
        {
            const int size = 3;
            double arr[size] = { 1.0, 2.0, 3.0 };

            double minElement = 1.0, maxElement = 3.0, averagesum = 2.0;

            // Заменяем элементы массива
            for (int i = 0; i < size; i++) {
                if (arr[i] == minElement || arr[i] == maxElement) {
                    arr[i] = averagesum;
                }
            }

            // Проверка результатов
            Assert::AreEqual(2.0, arr[0], L"Некорректная замена элемента");
            Assert::AreEqual(2.0, arr[1], L"Некорректная замена элемента");
            Assert::AreEqual(2.0, arr[2], L"Некорректная замена элемента");
        }
    };
}
