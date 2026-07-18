/* ========================================================================== */
/*                                                                            */
/*   sample.cpp                                                               */
/*   (c) 2012 Author                                                          */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */
//x86_64-w64-mingw32-g++ sample1.cpp -o sample1.exe -mwindows
#include <windows.h>
#include <psapi.h>
#include <iostream>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Your application logic goes here
    
    // Memory tracking
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        // WorkingSetSize shows current memory used (in bytes)
        size_t memory_used = pmc.WorkingSetSize;
        
        std::string msg = "Current Memory Usage: " + std::to_string(memory_used / 1024) + " KB";
        MessageBoxA(NULL, msg.c_str(), "Memory Status", MB_OK);
    }

    return 0;
}
