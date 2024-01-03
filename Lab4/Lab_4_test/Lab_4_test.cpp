#include "pch.h"
#include "CppUnitTest.h"
#include "../Lab_4_os/Main.cpp"  

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Lab4test
{
    TEST_CLASS(Lab4test)
    {
    public:

        TEST_METHOD(TestMethod1)
        {
            // �������� ������� ��� ������������
            FileHandler file_handler("test_file.txt");

            // ��������, ��� ���� ������� ������
            file_handler.CreateFile(5);

            // ��������, ��� ���� ����� ������� �������
            std::fstream file = file_handler.OpenFile(std::ios::in | std::ios::out);
            Assert::IsTrue(file.is_open());

            

            // �������� �����
            file.close();
        }
    };
}
