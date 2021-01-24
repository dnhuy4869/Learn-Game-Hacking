#include <Windows.h>
#include <iostream>
#include "MemMan.h"

MemMan MemClass;

struct gameOffsets
{
    DWORD lPlayer = 0xC5E87C;
    DWORD fJump = 0x50DE048;
    DWORD flags = 0x100;
}offsets;

struct values
{
    DWORD localPlayer;
    DWORD process;
    DWORD gameModule;
    BYTE flag;
}val;

int main()
{
    val.process = MemClass.getProcess("csgo.exe");
    val.gameModule = MemClass.getModule(val.process, "client.dll");

    val.localPlayer = MemClass.readMem<DWORD>(val.gameModule + offsets.lPlayer);

    if (val.localPlayer == NULL)
        while (val.localPlayer == NULL)
            val.localPlayer = MemClass.readMem<DWORD>(val.gameModule + offsets.lPlayer);

    while (true)
    {
        val.flag = MemClass.readMem<BYTE>(val.localPlayer + offsets.flags);

        if (GetAsyncKeyState(VK_SPACE) && val.flag & (1 << 0))
            MemClass.writeMem<DWORD>(val.gameModule + offsets.fJump, 6);

        Sleep(1);
    }
    return 0;
}