#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../Lab_2_os/Lab_2_os.cpp" 

namespace Lab2test
{
    TEST_CLASS(Lab2test)
    {
    public:

        // ���� ������ ������������ � ������������� ���������
        TEST_METHOD(FindMinMaxElementsTest)
        {
            const int size = 5;
            double arr[size] = { 3.0, 1.5, 4.2, 2.8, 5.1 };

            double minElement, maxElement;
            min_max_thread_function(arr, size, minElement, maxElement);

            // �������� �����������
            Assert::AreEqual(1.5, minElement, L"������������ ����������� �������");
            Assert::AreEqual(5.1, maxElement, L"������������ ������������ �������");
        }

        // ���� ���������� �������� ��������
        TEST_METHOD(CalculateAverageTest)
        {
            const int size = 4;
            double arr[size] = { 2.0, 4.0, 6.0, 8.0 };

            double averagesum;
            average_thread_function(arr, size, averagesum);

            // �������� �����������
            Assert::AreEqual(5.0, averagesum, L"������������ ������� ��������");
        }

        // ���� ������ ��������� � �������
        TEST_METHOD(ReplaceElementsTest)
        {
            const int size = 3;
            double arr[size] = { 1.0, 2.0, 3.0 };

            double minElement = 1.0, maxElement = 3.0, averagesum = 2.0;

            // �������� �������� �������
            for (int i = 0; i < size; i++) {
                if (arr[i] == minElement || arr[i] == maxElement) {
                    arr[i] = averagesum;
                }
            }

            // �������� �����������
            Assert::AreEqual(2.0, arr[0], L"������������ ������ ��������");
            Assert::AreEqual(2.0, arr[1], L"������������ ������ ��������");
            Assert::AreEqual(2.0, arr[2], L"������������ ������ ��������");
        }
    };
}
