#include <fstream> 
#include <windows.h> 
#include <conio.h> 
#include <iostream> 

class FileHandler {
public:
    FileHandler(const std::string& file_name) : file_name_(file_name) {}

    std::fstream OpenFile(std::ios_base::openmode mode) {
        return std::fstream(file_name_, mode);
    }

private:
    std::string file_name_;
};

class MessageProcessor {
public:
    static void ProcessMessages(const std::string& file_name, HANDLE hStartEvent, HANDLE hInputReadySemaphore, HANDLE hOutputReadySemaphore, HANDLE hMutex) {
        std::fstream file;
        std::cout << "Напишите «K», чтобы написать сообщение;\nНапишите «L», чтобы завершить процесс.\n";
        std::string key;
        std::cin >> key;

        while (true) {
            if (key == "K") {
                WaitForSingleObject(hMutex, INFINITE);
                file.open(file_name, std::ios::out | std::ios::app);

                std::string msg;
                std::cout << "Введите ваше сообщение: ";
                std::cin >> msg;

                char message[21];
                for (int i = 0; i < msg.length(); i++) {
                    message[i] = msg[i];
                }

                for (int i = msg.length(); i < 21; i++) {
                    message[i] = '\0';
                }

                message[20] = '\n';
                ReleaseMutex(hMutex);
                ReleaseSemaphore(hOutputReadySemaphore, 1, NULL);

                if (ReleaseSemaphore(hInputReadySemaphore, 1, NULL) != 1) {
                    std::cout << "Вы превысили количество  сообщений";
                    ReleaseSemaphore(hOutputReadySemaphore, 1, NULL);
                    WaitForSingleObject(hOutputReadySemaphore, INFINITE);
                    ReleaseSemaphore(hOutputReadySemaphore, 1, NULL);
                    ReleaseSemaphore(hInputReadySemaphore, 1, NULL);

                    for (int i = 0; i < 21; i++) {
                        file << message[i];
                    }
                }
                else {
                    for (int i = 0; i < 21; i++) {
                        file << message[i];
                    }
                }

                file.close();
                std::cout << "Напишите «K», чтобы написать сообщение;\nНапишите «L», чтобы завершить процесс.\n";
                std::cin >> key;
            }

            if (key != "L" && key != "K") {
                std::cout << "\nНеверное значение\nНапишите «K», чтобы написать сообщение;\nНапишите «L», чтобы завершить процесс.\n";
                std::cin >> key;
            }

            if (key == "L") {
                std::cout << "Процесс завершен.";
                break;
            }
        }
    }
};

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "RU");
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <file_name>\n";
        return 1;
    }

    std::string file_name = argv[1];
    FileHandler file_handler(file_name);
    HANDLE hStartEvent = OpenEventA(EVENT_MODIFY_STATE, FALSE, "StartPocess");

    if (hStartEvent == NULL) {
        std::cout << "Открыть event не удалось. \nНажмите любую клавишу, чтобы выйти.\n";
        std::cin.get();
        return GetLastError();
    }

    HANDLE hInputReadySemaphore = OpenSemaphoreA(EVENT_ALL_ACCESS, FALSE, "EnterSemaphoreStarted");
    if (hInputReadySemaphore == NULL)
        return GetLastError();

    HANDLE hOutputReadySemaphore = OpenSemaphoreA(EVENT_ALL_ACCESS, FALSE, "OutputSemaphoreStarted");
    if (hOutputReadySemaphore == NULL)
        return GetLastError();

    HANDLE hMutex = OpenMutexA(SYNCHRONIZE, FALSE, "mut ex");

    SetEvent(hStartEvent);
    std::cout << "Event был начат\n";

    MessageProcessor::ProcessMessages(file_name, hStartEvent, hInputReadySemaphore, hOutputReadySemaphore, hMutex);

    CloseHandle(hInputReadySemaphore);
    CloseHandle(hOutputReadySemaphore);
    CloseHandle(hMutex);

    return 0;
}
