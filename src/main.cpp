#include <iostream>
#include <windows.h>
#include <tchar.h>

void cpuInfo() {
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);

    std::cout << "CPU Information:\n";
    std::cout << "  Processor Architecture: ";

    switch(sysinfo.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_AMD64:
            std::cout << "x64\n";
            break;
        case PROCESSOR_ARCHITECTURE_INTEL:
            std::cout << "x86\n";
            break;
        case PROCESSOR_ARCHITECTURE_ARM:
            std::cout << "ARM\n";
            break;
        default:
            std::cout << "Unknown\n";
            break;
    }
}

int main() {
    std::cout << "Winfetch v0.0.1:\n";

    cpuInfo();

    return 0;
}