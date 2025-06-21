/* Credits : Guided Hacking
   I do not claim anything , I write this code just for learning purpose 
   to use create your own cheat table */


#include <iostream>
#include <vector>
#include <windows.h>
#include "Proc.h"

int main()
{
    //Get process Id for target process
    DWORD procId = GetProcId(L"ac_client.exe");

    //Get module base address for the process
    uintptr_t ModuleBase = GetModuleBaseAddress(procId, L"ac_client.exe");

    //Get handle to the process

    HANDLE hProcess = 0;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procId);

    //Resolve base address of the pointer chain
    // pointerBaseAddrese = ModuleBase + relative offsets;
    uintptr_t DynamicPtrBaseAddr = ModuleBase + 0x17E0A8;
    std::cout << "DynamicPtrBaseAddress = " << "0x" << std::hex << DynamicPtrBaseAddr << std::endl;


    //Resolve our Ammo pointer chain
    std::vector<unsigned int>ammoOffsets = { 0x364 ,0x14 , 0x0 };
    uintptr_t ammoAddr = FindDMAAddy(hProcess, DynamicPtrBaseAddr, ammoOffsets);
    std::cout << "AmmoAddress = " << "0x" << std::hex << ammoAddr << std::endl;


    //Read Ammo value
    int ammoValue = 0;
    ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
    std::cout << "Current Ammo =" << std::dec << ammoValue << std::endl;

    //Write ammo value
    int NewAmmoValue = 2077;
    WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &NewAmmoValue, sizeof(NewAmmoValue), nullptr);

    //Read out Again;
    ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
    std::cout << "New Ammo =" << std::dec << NewAmmoValue << std::endl;

    getchar();
    return 0;
}
