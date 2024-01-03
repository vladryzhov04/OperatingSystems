#include <fstream> 
#include <iostream> 
#include <string> 
#include <vector> 
#include <windows.h> 

class FileHandler {
public:
    FileHandler(const std::string& file_name) : file_name_(file_name) {}

    void CreateFile(int number_of_notes) {
        std::fstream file(file_name_, std::ios::out);
        file.close();
    }

    std::fstream OpenFile(std::ios_base::openmode mode) {
        return std::fstream(file_name_, mode);
    }

private:
    std::string file_name_;
};

class Note {
public:
    Note() = default;

    Note(const std::string& message) : message_(message) {}

    const std::string& GetMessage() const {
        return message_;
    }

private:
    std::string message_;
};

class Sender {
public:
    Sender(const std::string& file_name, HANDLE hInputReadySemaphore, HANDLE hOutputReadySemaphore, HANDLE hMutex)
        : file_name_(file_name), hInputReadySemaphore_(hInputReadySemaphore),
        hOutputReadySemaphore_(hOutputReadySemaphore), hMutex_(hMutex) {}

    void SendNote() {
        std::string message;
        std::cout << "Введите секретное сообщение: ";
        std::cin.ignore(); 
        std::getline(std::cin, message);

        Note note(message);
        WriteNoteToFile(note);
        WaitForSingleObject(hOutputReadySemaphore_, INFINITE);
        ReleaseSemaphore(hInputReadySemaphore_, 1, NULL);
    }

private:
    void WriteNoteToFile(const Note& note) {
        WaitForSingleObject(hMutex_, INFINITE);
        std::fstream file(file_name_, std::ios::out | std::ios::app);

        const std::string& message = note.GetMessage();
        char messageBuffer[21];
        int i;
        for (i = 0; i < message.length(); i++) {
            messageBuffer[i] = message[i];
        }

        for (; i < 20; i++) {
            messageBuffer[i] = '\0';
        }

        messageBuffer[20] = '\n';

        for (i = 0; i < 21; i++) {
            file << messageBuffer[i];
        }

        file.close();
        ReleaseMutex(hMutex_);
    }

    std::string file_name_;
    HANDLE hInputReadySemaphore_;
    HANDLE hOutputReadySemaphore_;
    HANDLE hMutex_;
};

class MessageProcessor {
public:
    static void ProcessMessages(const std::string& file_name, HANDLE hInputReadySemaphore, HANDLE hOutputReadySemaphore, HANDLE hMutex, int max_messages) {
        std::fstream file;
        std::cout << "\nEnter 1 to read the secret message;\nEnter 0 to exit \n";
        int key;
        std::cin >> key;
        file.open(file_name, std::ios::in);

        int messageCount = 0;

        while (messageCount < max_messages) {
            if (key == 1) {
                std::string message;
                WaitForSingleObject(hInputReadySemaphore, INFINITE);
                WaitForSingleObject(hMutex, INFINITE);
                std::getline(file, message);
                std::cout << message;
                ReleaseSemaphore(hOutputReadySemaphore, 1, NULL);
                ReleaseMutex(hMutex);
                ++messageCount;
                if (messageCount < max_messages) {
                    std::cout << "\nEnter 1 to read the secret message; \nEnter 0 to exit \n";
                    std::cin >> key;
                }
            }

            if (key != 0 && key != 1) {
                std::cout << "\nValue Error!\nEnter 1 to read the secret message;\nEnter 0 to exit \n";
                std::cin >> key;
            }
            if (key == 0) {
                std::cout << "Process ended";
                break;
            }
        }
        file.close();
    }
};

class App {
public:
    static void Run() {
        std::string file_name;
        int number_of_notes;
        int number_of_senders;

        std::cout << "Имя вашего двоичного файла: ";
        std::cin >> file_name;
        std::cout << "Количество записей: ";
        std::cin >> number_of_notes;

        FileHandler file_handler(file_name);
        file_handler.CreateFile(number_of_notes);

        std::cout << "Количество процессов-отправителей: ";
        std::cin >> number_of_senders;

        HANDLE inputSemaphore = CreateSemaphoreA(NULL, 0, number_of_notes, "EnterSemaphoreStarted");
        if (inputSemaphore == NULL)
            exit(GetLastError());
        HANDLE outputSemaphore = CreateSemaphoreA(NULL, 0, number_of_notes, "OutputSemaphoreStarted");
        if (outputSemaphore == NULL)
            exit(GetLastError());
        HANDLE hMutex = CreateMutexA(NULL, 0, "mut ex");
        HANDLE* hEventStarted = new HANDLE[number_of_senders];

        CreateSenderProcesses(file_name, number_of_senders, hEventStarted);

        WaitForMultipleObjects(number_of_senders, hEventStarted, TRUE, INFINITE);
        ReleaseMutex(hMutex);

        MessageProcessor::ProcessMessages(file_name, inputSemaphore, outputSemaphore, hMutex, number_of_notes);

        CloseHandle(inputSemaphore);
        CloseHandle(outputSemaphore);
        for (int i = 0; i < number_of_senders; i++) {
            CloseHandle(hEventStarted[i]);
        }

        delete[] hEventStarted;
    }

private:
    static void CreateSenderProcesses(const std::string& file_name, int number_of_senders, HANDLE* hEventStarted) {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        for (int i = 0; i < number_of_senders; ++i) {
            std::string sender_cmd = "Sender.exe " + file_name;
            LPSTR lpwstrSenderProcessCommandLine = const_cast<LPSTR>(sender_cmd.c_str());

            ZeroMemory(&si, sizeof(STARTUPINFO));
            si.cb = sizeof(STARTUPINFO);
            if (!CreateProcessA(NULL, lpwstrSenderProcessCommandLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, reinterpret_cast<LPSTARTUPINFOA>(&si), &pi)) {
                std::cout << "Процесс Sender не запущен.\n";
                exit(GetLastError());
            }
            hEventStarted[i] = CreateEventA(NULL, FALSE, FALSE, "StartPocess");
            if (hEventStarted[i] == NULL)
                exit(GetLastError());
            CloseHandle(pi.hProcess);
        }
    }
};

int main() {
    setlocale(LC_ALL, "RU");
    App::Run();
    return 0;
}
