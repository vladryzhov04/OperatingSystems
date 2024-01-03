#include <iostream>
#include <thread>
#include <cfloat>
#include <Windows.h>

void min_max_thread_function(double* arr, int size, double& minElement, double& maxElement) {
    minElement = DBL_MAX;
    maxElement = -DBL_MAX;

    for (int i = 0; i < size; i++) {
        if (arr[i] > maxElement) {
            maxElement = arr[i];
        }
        if (arr[i] < minElement) {
            minElement = arr[i];
        }
        Sleep(7); // Спим 7 миллисекунд
    }
}

void average_thread_function(double* arr, int size, double& averagesum) {
    averagesum = 0;
    for (int i = 0; i < size; i++) {
        averagesum += arr[i];
        Sleep(12); // Спим 12 миллисекунд
    }
    averagesum /= size;
}

int main() {
    std::setlocale(LC_ALL, "Russian");

    int size;

    std::cout << "Введите размер массива:" << std::endl;
    std::cin >> size;

    double* arr = new double[size];

    std::cout << "Введите элементы массива:\n";
    for (int i = 0; i < size; i++) {
        std::cin >> arr[i];
    }

    double minElement;
    double maxElement;
    double averagesum;

    std::thread min_max_thread(min_max_thread_function, arr, size, std::ref(minElement), std::ref(maxElement));
    std::thread average_thread(average_thread_function, arr, size, std::ref(averagesum));

    min_max_thread.join();
    average_thread.join();

    std::cout << "Минимум: " << minElement << " Максимум: " << maxElement << std::endl;
    std::cout << "Средняя сумма элементов массива: " << averagesum << std::endl;

    for (int i = 0; i < size; i++) {
        if (arr[i] == minElement || arr[i] == maxElement) {
            arr[i] = averagesum;
        }
    }

    std::cout << "Массив после замены элементов:\n";
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    delete[] arr;

    return 0;
}
