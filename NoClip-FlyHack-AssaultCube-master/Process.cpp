#include "Process.h"

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

uintptr_t GetModuleBaseAddress(DWORD processId, const wchar_t* moduleName)
{
	uintptr_t moduleBaseAddress = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);

	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 moduleEntry32;
		moduleEntry32.dwSize = sizeof(MODULEENTRY32W);

		if (Module32First(hSnapshot, &moduleEntry32))
		{
			do
			{
				if (!_wcsicmp(moduleEntry32.szModule, moduleName))
				{
					moduleBaseAddress = (uintptr_t)moduleEntry32.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnapshot, &moduleEntry32));
		}
	}

	CloseHandle(hSnapshot);

	return moduleBaseAddress;
}

uintptr_t FindDMAAddy(HANDLE hProcess, uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t address = ptr;

	for (int i = 0; i < offsets.size(); ++i)
	{
		ReadProcessMemory(hProcess, (LPCVOID)address, &address, sizeof(uintptr_t), nullptr);
		address += offsets[i];
	}

	return address;
}