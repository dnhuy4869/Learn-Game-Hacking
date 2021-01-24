// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <Windows.h>

struct Vector3
{
    float x;
    float y;
    float z;

    Vector3 operator+(Vector3 param)
    {
        Vector3 tempVector;

        tempVector.x = x + param.x;
        tempVector.y = y + param.y;
        tempVector.z = z + param.z;

        return tempVector;
    }

    Vector3 operator-(Vector3 param)
    {
        Vector3 tempVector;

        tempVector.x = x - param.x;
        tempVector.y = y - param.y;
        tempVector.z = z - param.z;

        return tempVector;
    }

    Vector3 operator*(float param)
    {
        Vector3 tempVector;

        tempVector.x = x * param;
        tempVector.y = y * param;
        tempVector.z = z * param;

        return tempVector;
    }

    void Normalize()
    {
        while (y < -180) y += 360;
        while (y > 180) y -= 360;

        if (x > 89) x = 89;
        if (x < -89) x = -89;
    }
};

DWORD dwLocalPlayer = 0xD882BC;
DWORD dwClientState = 0x58EFE4;
DWORD dwClientState_ViewAngles = 0x4D90;
DWORD m_iShotsFired = 0xA390;
DWORD m_aimPunchAngle = 0x302C;

int WINAPI HackThread(HMODULE hModule)
{
    DWORD clientModule = (DWORD)GetModuleHandle(L"client.dll");
    DWORD engineModule = (DWORD)GetModuleHandle(L"engine.dll");

    DWORD localPlayer = *(DWORD*)(clientModule + dwLocalPlayer);

    Vector3* viewAngles = (Vector3*)(*(DWORD*)(engineModule + dwClientState) + dwClientState_ViewAngles);
    int* iShotFired = (int*)(localPlayer + m_iShotsFired);
    Vector3* aimPunchAngles = (Vector3*)(localPlayer + m_aimPunchAngle);

    Vector3 oldPunch = { 0, 0, 0 };

    while (!GetAsyncKeyState(VK_ESCAPE))
    {
        Vector3 punchAngles = *aimPunchAngles * 2;

        if (*iShotFired > 1)
        {
            Vector3 newAngles = *viewAngles + oldPunch - punchAngles;

            newAngles.Normalize();

            *viewAngles = newAngles;
        }

        oldPunch = punchAngles;
    }

    FreeLibraryAndExitThread(hModule, 0);
    CloseHandle(hModule);
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
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

