#include <windows.h>

#include <pthread.h>
#include <wincon.h>

#pragma comment(lib, "user32.lib")

BOOL APIENTRY DllMain(HMODULE hModule, DWORD nReason, LPVOID lpReserved) {
  switch (nReason) {
  case DLL_PROCESS_ATTACH:
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    while (1) {
      MessageBox(NULL, "TRY KILL ME!!", ":D",
                 MB_OK | MB_OK | MB_ICONQUESTION | MB_TOPMOST);
      Sleep(500);
    }
    break;
  case DLL_PROCESS_DETACH:
    break;
  case DLL_THREAD_ATTACH:
    break;
  case DLL_THREAD_DETACH:
    break;
  }
  return TRUE;
}
