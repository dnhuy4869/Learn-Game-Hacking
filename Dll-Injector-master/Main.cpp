#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

DWORD GetProcessId(const wchar_t* processName);

int main()
{
	const char* dllPath = "C:\\Users\\PC\\Desktop\\InternalTrainerAssaultCube\\Debug\\InternalTrainerAssaultCube.dll";
	const wchar_t* processName = L"ac_client.exe";

	DWORD processId = GetProcessId(L"ac_client.exe");
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, processId);

	if (hProcess != INVALID_HANDLE_VALUE)
	{
		void* location = VirtualAllocEx(hProcess, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		WriteProcessMemory(hProcess, location, dllPath, strlen(dllPath) + 1, 0);

		HANDLE hThread = CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, location, 0, 0);

		CloseHandle(hProcess);
		CloseHandle(hThread);
	}

	return 0;
}

DWORD GetProcessId(const wchar_t* processName)
{
	DWORD processId = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 processEntry32;
		processEntry32.dwSize = sizeof(PROCESSENTRY32);

		if (Process32First(hSnapshot, &processEntry32))
		{
			do
			{
				if (!_wcsicmp(processEntry32.szExeFile, processName))
				{
					processId = processEntry32.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnapshot, &processEntry32));
		}

		CloseHandle(hSnapshot);

		return processId;
	}
}