#include <windows.h>
#include <stdio.h>

typedef enum _THREADINFOCLASS {
    ThreadBreakOnTermination = 18
} THREADINFOCLASS;

typedef NTSTATUS (*NtSetInformationThreadFunc)(
    __in HANDLE ThreadHandle,
    __in THREADINFOCLASS ThreadInformationClass,
    __in_bcount(ThreadInformationLength) PVOID ThreadInformation,
    __in ULONG ThreadInformationLength
);

BOOL EnableDebugPrivilege();


int main()
{
    if (!EnableDebugPrivilege())
    {
        printf("[!] Could not enable SeDebugPrivilige\n");
        return -1;
    }

    NtSetInformationThreadFunc NtSetInformationThread = (NtSetInformationThreadFunc) GetProcAddress(
            GetModuleHandle("ntdll.dll"), 
            "NtSetInformationThread"
        );

    BOOL Enable = TRUE;
    NTSTATUS Status;

    Status = NtSetInformationThread(
        GetCurrentThread(),
        ThreadBreakOnTermination,
        &Enable,
        sizeof(Enable)
    );

    if (Status < 0)
    {
        printf("[!] Could not set ThreadBreakOnTermination: 0x%08x\n", Status);
        return -1;
    }

    printf("Ha! try to kill me now.");

    Sleep(INFINITE);

    return 0;
}


BOOL EnableDebugPrivilege()
{
    HANDLE hToken = NULL;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken))
    {
        printf("[!] OpenProcessToken failed. 0x%08x\n", GetLastError());
        return FALSE;
    }

    LUID DebugPrivilege;

    if (!LookupPrivilegeValueA(NULL, SE_DEBUG_NAME, &DebugPrivilege))
    {
        printf("[!] LookupPrivilegeValueA failed. 0x%08x\n", GetLastError());
        CloseHandle(hToken);
        return FALSE;
    }

    TOKEN_PRIVILEGES tp;
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = DebugPrivilege;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
    {
        printf("[!] AdjustTokenPrivileges failed. 0x%08x\n", GetLastError());
        CloseHandle(hToken);
        return FALSE;
    }

    CloseHandle(hToken);
    return TRUE;
}