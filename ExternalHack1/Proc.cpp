#include "Proc.h"
//Get ProcId
DWORD GetProcId(const wchar_t* procName)
{
	DWORD procId = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry32;
		procEntry32.dwSize = sizeof(procEntry32);


		if (Process32First(hSnap, &procEntry32)) {

			do {
				if (!_wcsicmp(procEntry32.szExeFile, procName))
				{
					procId = procEntry32.th32ProcessID;
					break;
				}


			} while (Process32Next(hSnap, &procEntry32));


		}

	} CloseHandle(hSnap);
	return procId;
}

//Get Module Base Address
uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
{
	uintptr_t modBaseAddress = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS | TH32CS_SNAPMODULE32, procId);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry32;
		modEntry32.dwSize = sizeof(modEntry32);
		if (Module32First(hSnap, &modEntry32))
		{

			do {
				if (!_wcsicmp(modEntry32.szModule, modName))
				{
					modBaseAddress = (uintptr_t)modEntry32.modBaseAddr;
					break;
				}


			} while (Module32Next(hSnap, &modEntry32));
		}

	}CloseHandle(hSnap);
	return modBaseAddress;
}


uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int>offsets)
{

	uintptr_t Addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); i++)
	{

		ReadProcessMemory(hProc, (BYTE*)Addr, &Addr, sizeof(Addr), 0);
		Addr += offsets[i];


	}
	return Addr;

}
