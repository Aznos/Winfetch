#include "inc/ram.h"

void ramInfo() {
    std::cout << "RAM Information:\n";

    MEMORYSTATUSEX memoryStatus;
    memoryStatus.dwLength = sizeof(memoryStatus);
    GlobalMemoryStatusEx(&memoryStatus);

    DWORDLONG totalram = memoryStatus.ullTotalPhys;
    DWORDLONG usedram = memoryStatus.ullTotalPhys - memoryStatus.ullAvailPhys;
    double percentused = (double)usedram / (double)totalram * 100.0;
    
    std::cout << "  Total RAM: " << totalram / (1024 * 1024) << "MB\n";
    std::cout << "  Used RAM: " << std::fixed << std::setprecision(2) << usedram / (1024 * 1024) << "MB (" << percentused << "%)\n";
    std::cout << "  Available RAM: " << memoryStatus.ullAvailPhys / (1024 * 1024) << "MB\n";
}

void ramCompactInfo() {
    MEMORYSTATUSEX memoryStatus;
    memoryStatus.dwLength = sizeof(memoryStatus);
    GlobalMemoryStatusEx(&memoryStatus);

    DWORDLONG totalram = memoryStatus.ullTotalPhys;
    DWORDLONG usedram = memoryStatus.ullTotalPhys - memoryStatus.ullAvailPhys;
    double percentused = (double)usedram / (double)totalram * 100.0;

    std::cout << "RAM: " << totalram / (1024 * 1024) << "MB Total, " << std::fixed << std::setprecision(2) << usedram / (1024 * 1024) << "MB Used (" << percentused << "%)" << std::endl;
}