#include <windows.h>
#include <iostream>
#include <string>
#include <tlhelp32.h>

// Function to get the process ID by process name
DWORD GetProcessIdByName(const char* processName) {
    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    if (Process32First(snapshot, &processEntry)) {
        do {
            if (strcmp(processEntry.szExeFile, processName) == 0) {
                CloseHandle(snapshot);
                return processEntry.th32ProcessID;
            }
        } while (Process32Next(snapshot, &processEntry));
    }

    CloseHandle(snapshot);
    return 0;
}

// Main function to inject the DLL
bool InjectDLL(DWORD processId, const char* dllPath) {

    std::cout << "Injecting DLL into process " << processId << "..." << std::endl;
    std::cout << "DLL Path " << dllPath << std::endl;

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (!hProcess) {
        std::cerr << "Failed to open target process!" << std::endl;
        return false;
    }
    
    void* pAlloc = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
    if (pAlloc == NULL) {
        std::cerr << "Failed to allocate memory in target process!" << std::endl;
        CloseHandle(hProcess);
        return false;
    }

    if (!WriteProcessMemory(hProcess, pAlloc, dllPath, strlen(dllPath) + 1, NULL)) {
        std::cerr << "Failed to write to process memory!" << std::endl;
        VirtualFreeEx(hProcess, pAlloc, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, pAlloc, 0, NULL);


    if (hThread == NULL) {
        std::cerr << "Failed to create remote thread!" << std::endl;
        VirtualFreeEx(hProcess, pAlloc, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    WaitForSingleObject(hThread, INFINITE);
    VirtualFreeEx(hProcess, pAlloc, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);
    return true;
}

// Definition of the funciton poninter
typedef void (*HookFunction)(const char*);

int main() {
    
    // Change this to the full path of your DLL
    const char* dllPath = "D:\\Code Engineering\\cpp advanced\\hook funcitons\\simple-hook-target\\hook.dll";

    HWND hwnd = FindWindowA(0, "GTA: Vice City");
    if (!hwnd)
    {
        std::cout << "Window not found!" << std::endl;
        return 0;
    }
    else 
    { 
        std::cout << "Window found!" << std::endl;

        DWORD pId;
        GetWindowThreadProcessId(hwnd, &pId);
        std::cout << "Process ID: " << pId << std::endl;      


        if (InjectDLL(pId, dllPath)) 
        {
            std::cout << "DLL injected successfully!" << std::endl;

            std::cout << "Enter the message: ";
            std::string message;
            std::getline(std::cin, message);

            std:: cout << message << std::endl;

            HookFunction _hookFunction = (HookFunction)GetProcAddress(LoadLibraryA(dllPath), "HookFunction");

            if (_hookFunction) {
                _hookFunction(message.c_str());
            } else {
                std::cerr << "Failed to get function address!" << std::endl;
            }

            // TODO: the dll function is being called already now we need to inject it into the target process and call the function from there



        } else 
        {
            std::cerr << "DLL injection failed!" << std::endl;
        }
    }

    return 0;
}
