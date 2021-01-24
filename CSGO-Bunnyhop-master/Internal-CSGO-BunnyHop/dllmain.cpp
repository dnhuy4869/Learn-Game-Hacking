// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

struct gameOffsets
{
    DWORD lPlayer = 0xC5F89C;
    DWORD fJump = 0x50DF1B8;
    DWORD flags = 0x100;
    DWORD velocity = 0x110;
}offsets;

struct values
{
    DWORD localPlayer;
    DWORD gameModule;
    BYTE flag;
}val;

struct vector
{
    float x, y, z;
};

bool isPlayerMoving()
{
    vector playerVel = *(vector*)(val.localPlayer + offsets.velocity);
    int vel = playerVel.x + playerVel.y + playerVel.z;
    if (vel != 0)
        return true;
    else
        return false;
}

void main()
{
    val.gameModule = (DWORD)GetModuleHandle("client.dll");
    val.localPlayer = *(DWORD*)(val.gameModule + offsets.lPlayer);

    if (val.localPlayer == NULL)
        while (val.localPlayer == NULL)
            val.localPlayer = *(DWORD*)(val.gameModule + offsets.lPlayer);

    while (true)
    {
        val.localPlayer = *(DWORD*)(val.gameModule + offsets.lPlayer);
        val.flag = *(BYTE*)(val.localPlayer + offsets.flags);

        if (isPlayerMoving())
            if (GetAsyncKeyState(VK_SPACE) && val.flag & (1 << 0))
                *(DWORD*)(val.gameModule + offsets.fJump) = 6;
    }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

