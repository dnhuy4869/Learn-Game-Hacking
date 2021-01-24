#include <iostream>
#include "Process.h"

int main()
{
	HANDLE hProcess = 0;

	uintptr_t moduleBaseAddress = 0, localPlayerAddress = 0, spectateAddress = 0, invisibleAddress = 0;
	bool bHack = false;
	BYTE spectateOn = 5;
	BYTE spectateOff = 0;
	BYTE invisibleOn = 11;
	BYTE invisibleOff = 0;


	DWORD procId = GetProcessId(L"ac_client.exe");

	if (procId)
	{
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);
		moduleBaseAddress = GetModuleBaseAddress(procId, L"ac_client.exe");
		localPlayerAddress = moduleBaseAddress + 0x10F4F4;
	}
	else
	{
		std::cout << "Cannot find the game, press any key to exit" << std::endl;
		return 0;
	}

	while (true)
	{
		if (GetAsyncKeyState(VK_INSERT) & 1)
		{
			bHack = !bHack;

			ReadProcessMemory(hProcess, (BYTE*)moduleBaseAddress, &localPlayerAddress, sizeof(localPlayerAddress), nullptr);
			spectateAddress = localPlayerAddress + 0x338;
			invisibleAddress = localPlayerAddress + 0x82;

			if (bHack)
			{
				WriteProcessMemory(hProcess, (BYTE*)moduleBaseAddress, &spectateOn, sizeof(spectateOn), nullptr);
				WriteProcessMemory(hProcess, (BYTE*)moduleBaseAddress, &invisibleOn, sizeof(invisibleOn), nullptr);
			}
			else
			{
				WriteProcessMemory(hProcess, (BYTE*)moduleBaseAddress, &spectateOff, sizeof(spectateOff), nullptr);
				WriteProcessMemory(hProcess, (BYTE*)moduleBaseAddress, &invisibleOff, sizeof(invisibleOff), nullptr);
			}
		}
	}
}

