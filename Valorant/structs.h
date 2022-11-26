#pragma once

typedef struct _COOL_RWPHYSICAL_REQUESTS
{
	int SourceProcessID;
	uint64_t SourceAddress;
	uint64_t ReturnAddress;
	size_t Size;

} COOL_RWPHYSICAL_REQUESTS, * PCOOL_RWPHYSICAL_REQUESTS;

typedef struct _COOL_MODULE_REQUESTS
{
	int TargetProcessID;
	uint64_t ReturnAddress;
	uint64_t GuardedRegion;

} COOL_MODULE_REQUESTS, * PCOOL_MODULE_REQUESTS;

typedef struct _COOL_GUARDEDREGION_REQUESTS
{
	DWORD FirstPointer;
	uint64_t GuardedRegion;

} COOL_GUARDEDREGION_REQUESTS, * PCOOL_GUARDEDREGION_REQUESTS;