#include <Windows.h>
#include "MemMan.h"

MemMan MemClass;

struct offsets
{
    DWORD entityList = 0x4CD9FEC;
    DWORD isSpotted = 0x93D;
} offset;

struct variables
{
    DWORD gameModule;
} val;

int main()
{
    int procID = MemClass.getProcess("csgo.exe");
    val.gameModule = MemClass.getModule(procID, "client.dll");

    while (true)
    {
        for (short int i = 0; i < 64; i++)
        {
            DWORD entity = MemClass.readMem<DWORD>(val.gameModule + offset.entityList + i * 0x10);
            if (entity != NULL)
                MemClass.writeMem<bool>(entity + offset.isSpotted, true);
        }
        Sleep(1);
    }
    return 0;
}