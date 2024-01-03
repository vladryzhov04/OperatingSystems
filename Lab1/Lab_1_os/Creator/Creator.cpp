#include <iostream>
#include <string>
#include <fstream>

struct employee
{
    int num;
    char name[10];
    double hours;
};

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Using: " << argv[0] << " file_name countEmployee" << std::endl;
        return 1;
    }

    const char* file_name = argv[1];
    int countEmployee = std::stoi(argv[2]);

    std::ofstream outputFileStream(file_name, std::ios::out | std::ios::binary);
    if (!outputFileStream.is_open())
    {
        std::cerr << "The file could not be opened for writing." << std::endl;
        return 1;
    }

    outputFileStream.write(reinterpret_cast<char*>(&countEmployee), sizeof(int));

    for (int i = 0; i < countEmployee; ++i)
    {
        employee emp;

        std::cout << "Enter id of employee: ";
        std::cin >> emp.num;
        std::cout << "Enter name of employee: ";
        std::cin >> emp.name;
        std::cout << "Enter number of hours of employee: ";
        std::cin >> emp.hours;

        outputFileStream.write(reinterpret_cast<char*>(&emp), sizeof(employee));
    }

    outputFileStream.close();
    std::cout << "The file was created successfully." << std::endl;

    return 0;
}