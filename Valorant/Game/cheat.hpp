#pragma once
#include "sdk.hpp"
#include "ue4.h"
#include <iostream>

using namespace Globals;
using namespace Camera;
using namespace UE4;

GWorld* UWorld;
GGameInstance* UGameInstance;
GLocalPlayer* ULocalPlayer;
GPlayerController* APlayerController;
GPawn* APawn;
GPrivatePawn* APrivatePawn;
GULevel* ULevel;
GUSkeletalMeshComponent* USkeletalMeshComponent;

bool cached = false;
uintptr_t WorldPtr;

struct FLinearColor {
	float R; // 0x00(0x04)
	float G; // 0x04(0x04)
	float B; // 0x08(0x04)
	float A; // 0x0c(0x04)
};

auto CacheGame() -> void
{
	auto guardedregion = Cool.GetGuardedRegions(0x60);

	while (true)
	{
		std::vector<ValEntity> CachedList;

		WorldPtr = GetWorld(guardedregion);

		auto ULevelPtr = UWorld->ULevel(WorldPtr);
		auto UGameInstancePtr = UWorld->GameInstance(WorldPtr);

		auto ULocalPlayerPtr = UGameInstance->ULocalPlayer(UGameInstancePtr);
		auto APlayerControllerPtr = ULocalPlayer->APlayerController(ULocalPlayerPtr);

		PlayerCameraManager = APlayerController->APlayerCameraManager(APlayerControllerPtr);
		auto MyHUD = APlayerController->AHUD(APlayerControllerPtr);

		auto APawnPtr = APlayerController->APawn(APlayerControllerPtr);

		if (APawnPtr != 0)
		{
			MyUniqueID = APawn->UniqueID(APawnPtr);
			MyRelativeLocation = APawn->RelativeLocation(APawnPtr);
		}

		if (MyHUD != 0)
		{
			auto PlayerArray = ULevel->AActorArray(ULevelPtr);

			for (uint32_t i = 0; i < PlayerArray.Count; ++i)
			{
				auto Pawns = PlayerArray[i];
				if (Pawns != APawnPtr)
				{
					if (MyUniqueID == APawn->UniqueID(Pawns))
					{
						ValEntity Entities{ Pawns };
						CachedList.push_back(Entities);
					}
				}
			}

			ValList.clear();
			ValList = CachedList;
			Sleep(1000);
		}
	}
}

uintptr_t GuardedRegions = 0;

auto GetWorld_22(uintptr_t uworld_addr) -> uintptr_t
{
	uintptr_t uworld_offset;

	if (uworld_addr > 0x10000000000)
	{
		uworld_offset = uworld_addr - 0x10000000000;
	}
	else {
		uworld_offset = uworld_addr - 0x8000000000;
	}

	return GuardedRegions + uworld_offset;
}

auto CheatLoop() -> void
{
	for (ValEntity ValEntityList : ValList)
	{
	if (APawn->bIsDormant(ValEntityList.Actor))
		{
			if (Settings::Visuals::team)
			{
				GuardedRegions = Cool.GetGuardedRegions(offsets::FirstPointer);

				auto UworldPtr = Cool.Read<uintptr_t>(GuardedRegions + offsets::FirstPointer);

				auto Uworld = GetWorld_22(UworldPtr);

				auto GameState = Cool.Read<uintptr_t>(Uworld + offsets::GameState);

				auto PlayerArray = Cool.Read<TArray<uintptr_t>>(GameState + offsets::PlayerArray);

				if (PlayerArray.IsValid())
				{
					for (auto& PlayerStates : PlayerArray.Iteration())
					{
						auto Pawn = Cool.ReadGuardedWrapper<uintptr_t>(PlayerStates + offsets::SpawnedCharacter);

						auto MeshComponent = Cool.ReadGuardedWrapper<uintptr_t>(Pawn + offsets::MeshComponent);

						auto AttachChildren = Cool.Read<TArray<uintptr_t>>(MeshComponent + offsets::AttachChildren);

						if (AttachChildren.IsValid())
						{
							for (int i = 0; i < AttachChildren.Length(); i++)
							{
								auto Mesh = AttachChildren[i];
								if (!Mesh) continue;

								if (Cool.Read<int>(Mesh + offsets::OutlineMode) == 3)
									Cool.Write<int>(Mesh + offsets::OutlineMode, 1);

							}
						}
					}
				}
			}
			if (Settings::Visuals::glow)
			{
				Cool.Write(ValEntityList.Actor + 0x6b8, 100);
			}
			if (Settings::Visuals::chams)
			{
				// thanks for chams pinguu
				// Thread: https://www.unknowncheats.me/forum/valorant/515451-glowing-chams-external-internal.html

				auto outline_3p1 = Cool.Read<uintptr_t>(std::uintptr_t(ValEntityList.Actor) + 0x10b8);
				Cool.Write<int>(outline_3p1 + 0x2B1, 1);

				Cool.Write<FLinearColor>(Cool.GetProcessBaseAddress() + 0x8EFA150, {2.093f, 0.019f, 20.0f, 5.9f}); // ally
				Cool.Write<FLinearColor>(Cool.GetProcessBaseAddress() + 0x8EFA7E0, { 2.093f, 0.019f, 20.0f, 5.9f }); // enemy
			}
		}
	}
}
