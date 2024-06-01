#include <windows.h>
#include <iostream>

// extern "C" __declspec(dllexport) void HookFunction(const LPCSTR message) {
//     MessageBox(NULL, "hallo there", "Window Message here", 0);
// }

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

DLL_EXPORT void HookFunction(const LPCSTR message)
{
    MessageBoxA(0, message, "Custom Window Message", MB_OK | MB_ICONINFORMATION);
}

#ifdef __cplusplus
}
#endif

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
