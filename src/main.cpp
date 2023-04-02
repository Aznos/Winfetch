#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <intrin.h>
#include <iomanip>

void cpuInfo() {
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    int CPUInfo[4] = {-1};

    std::cout << "CPU Information:\n";

    //Architecture
    std::cout << "  Architecture: ";

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

    //CPU Name
    char cpuName[49];
    __cpuid(CPUInfo, 0x80000002);
    memcpy(cpuName, CPUInfo, sizeof(CPUInfo));
    __cpuid(CPUInfo, 0x80000003);
    memcpy(cpuName + 16, CPUInfo, sizeof(CPUInfo));
    __cpuid(CPUInfo, 0x80000004);
    memcpy(cpuName + 32, CPUInfo, sizeof(CPUInfo));
    cpuName[48] = '\0';

    std::cout << "  Name: " << cpuName << "\n";

    //Clock speed
    __cpuid(CPUInfo, 0x16);
    unsigned int baseClock = CPUInfo[0] & 0xFFFF;
    unsigned int maxClock = CPUInfo[1] & 0xFFFF;
    std::cout << "  Clock Speed: " << (baseClock / 1000.0) << "GHz (Max: " << (maxClock / 1000.0) << "GHz)\n";

    //Cores and threads
    DWORD bufferSize = 0;
    GetLogicalProcessorInformation(NULL, &bufferSize);
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(bufferSize);
    GetLogicalProcessorInformation(buffer, &bufferSize);

    int physicalCores = 0;
    for (DWORD i = 0; i < bufferSize / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); i++) {
        if (buffer[i].Relationship == RelationProcessorCore) {
            physicalCores++;
        }
    }

    std::cout << "  Cores: " << physicalCores << "\n";
    std::cout << "  Threads: " << sysinfo.dwNumberOfProcessors << "\n";
}

void osInfo() {
    std::cout << "OS Information:\n";

    //Computer name
    std::cout << "  Computer Name: ";
    std::wstring pcName;
    DWORD bufferSize = MAX_COMPUTERNAME_LENGTH + 1;
    pcName.resize(bufferSize);
    GetComputerNameExW(ComputerNamePhysicalDnsHostname, &pcName[0], &bufferSize);
    std::wcout << pcName.c_str() << "\n";

    //OS name
    HKEY hKey;
    DWORD dwType, dwSize;
    char szProductKey[255] = { 0 };
    DWORD dwMajorVersion = 0, dwMinorVersion = 0;

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        dwSize = sizeof(szProductKey);
        if (RegQueryValueEx(hKey, "ProductName", NULL, &dwType, (LPBYTE)szProductKey, &dwSize) == ERROR_SUCCESS) {
            std::cout << "  Version: " << szProductKey << std::endl;
        }
    }
}

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

int main() {
    std::cout << "Winfetch v0.0.1\n";

    cpuInfo();
    ramInfo();
    osInfo();

    return 0;
}