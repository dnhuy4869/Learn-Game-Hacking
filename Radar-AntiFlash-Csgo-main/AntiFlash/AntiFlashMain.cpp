#include <Windows.h>
#include <iostream>
#include "MemMan.h"

MemMan MemClass;

struct offsets
{
    DWORD localPlayer = 0xC648AC;
    DWORD flashDuration = 0xA32C;
} offset;

struct variables
{
    DWORD localPlayer;
    DWORD gameModule;
} val;

int main()
{
    int flashDur = 0;
    int procID = MemClass.getProcess("csgo.exe");
    val.gameModule = MemClass.getModule(procID, "client.dll");
    val.localPlayer = MemClass.readMem<DWORD>(val.gameModule + offset.localPlayer);

    if (val.localPlayer == NULL)
        while (val.localPlayer == NULL)
            val.localPlayer = MemClass.readMem<DWORD>(val.gameModule + offset.localPlayer);

    while (true)
    {
        flashDur = MemClass.readMem<int>(val.localPlayer + offset.flashDuration);
        if (flashDur > 0)
            MemClass.writeMem<int>(val.localPlayer + offset.flashDuration, 0);

        Sleep(1);
    }

    return 0;
}