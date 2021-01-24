#pragma once

#include <vector>
#include <Windows.h>
#include <TlHelp32.h>

DWORD GetProcessId(const wchar_t* processName);
uintptr_t GetModuleBaseAddress(DWORD processId, const wchar_t* moduleName);
uintptr_t FindDMAAddy(HANDLE hProcess, uintptr_t ptr, std::vector<unsigned int> offsets);
