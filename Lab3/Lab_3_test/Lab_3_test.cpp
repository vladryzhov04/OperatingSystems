#include "pch.h"
#include "CppUnitTest.h"
#include "../Lab_3_os/Lab_3_os.cpp"  


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace YourProjectTest
{
    TEST_CLASS(YourTestClass)
    {
    public:

        // “ест дл€ проверки основных аспектов кода
        TEST_METHOD(BasicFunctionalityTest)
        {
            // »нициализаци€ данных дл€ теста
            MasSize = 5;
            countOfThreads = 3;
            mas.resize(MasSize, 0);
            masOfStates.resize(countOfThreads, true);
            markedElements.resize(countOfThreads);

            // «апуск тестовых потоков (используем фиктивные данные)
            std::thread thread1(FThread, 1);
            std::thread thread2(FThread, 2);
            std::thread thread3(FThread, 3);

            // ќжидание завершени€ потоков
            if (thread1.joinable()) thread1.join();
            if (thread2.joinable()) thread2.join();
            if (thread3.joinable()) thread3.join();

            // ѕроверка, что массив mas был изменен
            bool anyChanges = std::any_of(mas.begin(), mas.end(), [](int val) { return val != 0; });
            Assert::IsTrue(anyChanges, L"ћассив mas не изменилс€ после выполнени€ потоков");

            
        }
    };
}
