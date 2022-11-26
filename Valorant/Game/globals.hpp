#pragma once
#include <Windows.h>

HWND Entryhwnd = NULL;
int processid = 0;

namespace offsets
{
    DWORD
        Gameinstance = 0x1a0,
        Ulevel = 0x38,
        LocalPlayers = 0x40,
        PlayerController = 0x38,
        PlayerCameraManager = 0x478,
        MyHUD = 0x470,
        AcknowledgedPawn = 0x460,
        PlayerState = 0x3f0,
        TeamComponent = 0x628,
        TeamID = 0xf8,
        UniqueID = 0x38,
        FNameID = 0x18,
        AActorArray = 0xa0,
        RootComponent = 0x230,
        RelativeLocation = 0x164,
        MeshComponent = 0x430,
        DamageHandler = 0x9a8,
        bIsDormant = 0x100,
        Health = 0x1b0,
        ComponentToWorld = 0x250,
        BoneArray = 0x5C0,
        BoneArrayCache = BoneArray + 0x10,
        BoneCount = 0x5C8,
        FirstPointer = 0x60,
        GameState = 0x140,
        PlayerArray = 0x3e8,
        SpawnedCharacter = 0x8c8,
        AttachChildren = 0x110,
        OutlineMode = 0x330;
}

namespace Settings
{
    inline bool bMenu = true;

    namespace Visuals
    {
        inline bool team = false;
        inline bool glow = false;
        inline bool chams = false;
        inline float BoxWidth = 1.0f;
    }
}
