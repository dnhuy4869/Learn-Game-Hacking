// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

DWORD WINAPI HackThread(HMODULE hModule)
{
    AllocConsole();

    FILE* file;
    freopen_s(&file, "CONOUT$", "w", stdout);

    uintptr_t moduleBaseAddress = (uintptr_t)GetModuleHandle(L"ac_client.exe");

    bool bHealth = false, bAmmo = false, bRecoil = false;

    while (true)
    {
        if (GetAsyncKeyState(VK_END) & 1)
        {
            break;
        }

        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            bHealth = !bHealth;
        }

        if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        {
            bAmmo = !bAmmo;
        }

        if (GetAsyncKeyState(VK_NUMPAD3) & 1)
        {
            bRecoil = !bRecoil;

            if (bRecoil)
            {
                mem::Nop((BYTE*)(moduleBaseAddress + 0x63786), 10);
            }
            else
            {
                mem::Patch((BYTE*)(moduleBaseAddress + 0x63786), (BYTE*)"\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2", 10);
            }
        }

        uintptr_t* localPlayer = (uintptr_t*)(moduleBaseAddress + 0x10F4F4);

        if (localPlayer)
        {
            if (bHealth)
            {
                *(int*)(*localPlayer + 0xF8) = 1337;
            }

            if (bAmmo)
            {
                uintptr_t ammoAddress = FindDMAAddy(moduleBaseAddress + 0x10F4F4, { 0x374, 0x14, 0x0 });
                *(int*)ammoAddress = 1337;
            }
        }

        Sleep(10);
    }

    fclose(file);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);

    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

