#include "win_shell_util.h"

#include <Windows.h>

int start_as_admin(const std::wstring& exe, const std::wstring& params)
{
    return 0;
}

int restart_as_admin(bool waitForExit)
{
    TCHAR exe[MAX_PATH];
    ZeroMemory(exe, MAX_PATH);
    GetModuleFileName(NULL, exe, MAX_PATH);
    LPWSTR cmdline = GetCommandLine();

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    if (!CreateProcess(NULL, cmdline, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        printf("CreateProcess failed (%d)", GetLastError());
        return 1;
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
}