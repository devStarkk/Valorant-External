#pragma once
#include "../ioctls.h"
#include "../structs.h"

class CoolDriver
{
public:
	HANDLE DriverHandle;
	int TargetProcessPid;
	uint64_t GuardedPointer;

	void Init(int targetPid)
	{
		DriverHandle = CreateFileW(L"\\\\.\\coolschool", GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_HIDDEN, NULL);
		if ((unsigned long long)DriverHandle == 0xFFFFFFFFFFFFFFFF)
		{
			printf("Driver not loaded.");
			Sleep(-1);
		}

		TargetProcessPid = targetPid;
	}

	void MmReadPhysicalMemory(uint64_t source, uint64_t destination, SIZE_T size)
	{
		if (source == 0 || destination == 0) return;

		COOL_RWPHYSICAL_REQUESTS MMCOPY;
		MMCOPY.SourceProcessID = TargetProcessPid;
		MMCOPY.SourceAddress = source;
		MMCOPY.ReturnAddress = destination;
		MMCOPY.Size = size;

		DeviceIoControl(DriverHandle, COOLCODE_READPHYSICAL, &MMCOPY, sizeof(MMCOPY), &MMCOPY, sizeof(MMCOPY), 0, 0);
	}

	void MmWritePhysicalMemory(uint64_t source, uint64_t destination, SIZE_T size)
	{
		if (source == 0 || destination == 0) return;

		COOL_RWPHYSICAL_REQUESTS MMCOPY;
		MMCOPY.SourceProcessID = TargetProcessPid;
		MMCOPY.SourceAddress = source;
		MMCOPY.ReturnAddress = destination;
		MMCOPY.Size = size;

		DeviceIoControl(DriverHandle, COOLCODE_WRITEPHYSICAL, &MMCOPY, sizeof(MMCOPY), &MMCOPY, sizeof(MMCOPY), 0, 0);
	}

	uint64_t GetProcessBaseAddress()
	{
		COOL_MODULE_REQUESTS GETBASEREQUEST;

		GETBASEREQUEST.TargetProcessID = TargetProcessPid;

		DeviceIoControl(DriverHandle, COOLCODE_REQUESTMODULEBASE, &GETBASEREQUEST, sizeof(GETBASEREQUEST), &GETBASEREQUEST, sizeof(GETBASEREQUEST), 0, 0);

		return GETBASEREQUEST.ReturnAddress;
	}

	uint64_t GetGuardedRegions(DWORD FirstPointer)
	{
		COOL_GUARDEDREGION_REQUESTS GETBASEREQUEST;

		GETBASEREQUEST.FirstPointer = FirstPointer;

		DeviceIoControl(DriverHandle, COOLCODE_REQUESTGUARDEDREGION, &GETBASEREQUEST, sizeof(GETBASEREQUEST), &GETBASEREQUEST, sizeof(GETBASEREQUEST), 0, 0);

		GuardedPointer = GETBASEREQUEST.GuardedRegion;

		return GETBASEREQUEST.GuardedRegion;
	}

	template<typename T>
	T Read(uint64_t address)
	{
		T Buffer;
		MmReadPhysicalMemory(address, (uint64_t)&Buffer, sizeof(T));
		return Buffer;
	}

	bool Check(uintptr_t Pointer) noexcept
	{
		static constexpr uintptr_t Base = 0xFFFFFFF000000000;
		uintptr_t Result = Pointer & Base;
		return Result == 0x8000000000 || Result == 0x10000000000;
	}

	template <typename T>
	T ReadGuarded(uintptr_t src, size_t size = sizeof(T))
	{
		T Buffer;
		MmReadPhysicalMemory(src, (uintptr_t)&Buffer, size);

		uintptr_t Val = GuardedPointer + (*(uintptr_t*)&Buffer & 0xFFFFFF);
		return *(T*)&Val;
	}

	template <typename T>
	T ReadGuardedWrapper(T src)
	{
		T buffer = Read<uintptr_t>(src);

		if (Check((uintptr_t)buffer))
		{
			return ReadGuarded<uintptr_t>(src);
		}

		return buffer;
	}

	template <typename T>
	void ReadArray(uint64_t address, T* array, size_t len)
	{
		MmReadPhysicalMemory(address, (uint64_t)array, sizeof(T) * len);
	}

	template<typename T>
	void Write(uint64_t address, T val)
	{
		MmWritePhysicalMemory(address, (uint64_t)&val, sizeof(T));
	}
};

CoolDriver Cool;
