#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <random>

const int MAX_THREADS = 100;

std::vector<int> mas;
int MasSize;
std::vector<std::vector<int>> markedElements; 
std::vector<bool> masOfStates;
int countOfThreads;
std::mutex mt;
std::condition_variable condition;
int numbToEnd = -1;

bool consistMetk(int* vec, int size, int a) {
    for (int i = 0; i < size; i++) {
        if (vec[i] == a) {
            return true;
        }
    }
    return false;
}

bool checkAllThreadsStop() {
    return std::all_of(masOfStates.begin(), masOfStates.end(), [](bool state) { return !state; });
}

void FThread(int x) {
    srand(x);
    std::vector<int>& metki = markedElements[x - 1];

    while (true) {
        std::unique_lock<std::mutex> ul(mt);
        if (numbToEnd == x) {
            masOfStates[x - 1] = false;
            ul.unlock();
            condition.notify_all();

            std::cout << "Thread #" << x << " пометил элементы: ";
            for (int i : metki) {
                std::cout << i << " ";
                mas[i] = 0;  
            }
            std::cout << std::endl;

            return;
        }

        int ch = rand() % MasSize;

        if (mas[ch] == 0 && !consistMetk(metki.data(), metki.size(), ch)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            mas[ch] = x;
            metki.push_back(ch);
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        else {
            masOfStates[x - 1] = false;
            std::cout << "Thread #" << x << std::endl;
            for (int i : metki) {
                std::cout << i << " ";
            }
            std::cout << std::endl;
            std::cout << "Элементы, которые не были помечены: ";
            for (int i = 0; i < MasSize; i++) {
                if (!consistMetk(metki.data(), metki.size(), i) && mas[i] == 0) {
                    std::cout << i << " ";
                }
            }
            std::cout << std::endl;
            condition.notify_all();
            condition.wait(ul, [=]() {return masOfStates[x - 1] == true; });
        }
        ul.unlock();
    }
}

int main() {
    setlocale(LC_ALL, "rus");
    std::cout << "Введите размер массива" << std::endl;
    std::cin >> MasSize;
    mas.resize(MasSize, 0);

    std::cout << "Введите количество потоков maker" << std::endl;
    std::cin >> countOfThreads;
    masOfStates.resize(countOfThreads, true);
    markedElements.resize(countOfThreads);

    std::vector<std::thread> Threads;

    for (int i = 1; i <= countOfThreads; i++) {
        Threads.emplace_back(&FThread, i);
    }

    int numb;
    while (true) {
        std::unique_lock<std::mutex> ul(mt);
        condition.wait(ul, [=]() {return checkAllThreadsStop(); });

        std::cout << "mas: ";
        bool foundUnmarked = false;
        for (int i = 0; i < MasSize; i++) {
            std::cout << mas[i] << " ";
            if (mas[i] == 0) {
                foundUnmarked = true;
            }
        }
        std::cout << std::endl;

        if (foundUnmarked) {
            std::cout << "Элементы, которые не были помечены: ";
            for (int i = 0; i < MasSize; i++) {
                if (mas[i] == 0) {
                    std::cout << i << " ";
                }
            }
            std::cout << std::endl;
        }

        std::cout << "Номер потока для завершения (или 0 для выхода): ";
        std::cin >> numb;
        if (numb >= 1 && numb <= countOfThreads) {
            numbToEnd = numb;
            masOfStates[numb - 1] = true;
            condition.notify_all();
        }
        else if (numb == 0) {
           
            break;
        }
        else {
            std::cout << "Некорректный номер потока." << std::endl;
        }

        ul.unlock();  
    }

    for (auto& thread : Threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    return 0;
}
