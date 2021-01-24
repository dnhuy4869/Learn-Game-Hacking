#include "Process.h"
#include "Memory.h"
#include <iostream>

int main()
{
	#pragma region Part_1
	/*DWORD processId = GetProcessId(L"ac_client.exe");
	uintptr_t moduleBaseAddress = GetModuleBaseAddress(processId, L"ac_client.exe");
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, processId);

	uintptr_t baseAddress = moduleBaseAddress + 0x10F4F4;
	std::vector<unsigned int> ammoOffsets = { 0x374, 0x14, 0x0 };
	uintptr_t ammoAddress = FindDMAAddy(hProcess, baseAddress, ammoOffsets);

	int ammoNewValue = 300;

	WriteProcessMemory(hProcess, (LPVOID)ammoAddress, &ammoNewValue, sizeof(int), nullptr);*/
	#pragma endregion

	HANDLE hProcess = 0;

	uintptr_t moduleBaseAddress = 0, localPlayerPtr = 0, healthAddress = 0;
	bool bHealth = false, bAmmo = false, bRecoil = false;

	const int newValue = 1337;

	DWORD procId = GetProcessId(L"ac_client.exe");

	if (procId)
	{
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
		moduleBaseAddress = GetModuleBaseAddress(procId, L"ac_client.exe");
		localPlayerPtr = moduleBaseAddress + 0x10F4F4;
		healthAddress = FindDMAAddy(hProcess, localPlayerPtr, { 0xF8 });
	}
	else
	{
		std::cout << "Cannot find the game, press any key to exit" << std::endl;
		return 0;
	}

	DWORD dwExit = 0;

	while (GetExitCodeProcess(hProcess, &dwExit) && dwExit == STILL_ACTIVE)
	{
		if (GetAsyncKeyState(VK_NUMPAD1) & 1)
		{
			bHealth = !bHealth;
		}

		if (GetAsyncKeyState(VK_NUMPAD2) & 1)
		{
			bAmmo = !bAmmo;

			if (bAmmo)
			{
				mem::PatchEx((BYTE*)(moduleBaseAddress + 0x637e9), (BYTE*)"\xFF\x06", 2, hProcess);
			}
			else
			{
				mem::PatchEx((BYTE*)(moduleBaseAddress + 0x637e9), (BYTE*)"\xFF\x0E", 2, hProcess);
			}
		}

		if (GetAsyncKeyState(VK_NUMPAD3) & 1)
		{
			bRecoil = !bRecoil;

			if (bRecoil)
			{
				mem::NopEx((BYTE*)(moduleBaseAddress + 0x63786), 10, hProcess);
			}
			else
			{
				mem::PatchEx((BYTE*)(moduleBaseAddress + 0x63786), (BYTE*)"\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2", 10, hProcess);
			}
		}

		if (GetAsyncKeyState(VK_NUMPAD4) & 1)
		{
			return 0;
		}

		if (bHealth)
		{
			mem::PatchEx((BYTE*)healthAddress, (BYTE*)&newValue, sizeof(newValue), hProcess);
		}

		Sleep(10);
	}
}