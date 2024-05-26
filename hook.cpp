#include <windows.h>
#include <iostream>

extern "C" __declspec(dllexport) void HookFunction() {
    // MessageBox(NULL, "DLL Injected Successfully!", "Success", MB_OK);
    MessageBox(NULL, "Process attach!", "Woohoo", 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        HookFunction();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
