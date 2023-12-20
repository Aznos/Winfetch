#include "inc/cpu.h"

SYSTEM_INFO sysinfo;
void cpuInfo() {
    GetSystemInfo(&sysinfo);
    int CPUInfo[4] = {-1};
    HKEY hKey;
    DWORD dwType, dwSize;

    std::cout << "CPU Information:\n";

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

    char cpuName[49];
    __cpuid(CPUInfo, 0x80000002);
    memcpy(cpuName, CPUInfo, sizeof(CPUInfo));
    __cpuid(CPUInfo, 0x80000003);
    memcpy(cpuName + 16, CPUInfo, sizeof(CPUInfo));
    __cpuid(CPUInfo, 0x80000004);
    memcpy(cpuName + 32, CPUInfo, sizeof(CPUInfo));
    cpuName[48] = '\0';

    std::cout << "  Name: " << cpuName << "\n";

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD* dwMHzbuff = new DWORD;
        if (RegQueryValueEx(hKey, "~MHz", NULL, &dwType, (LPBYTE)dwMHzbuff, &dwSize) == ERROR_SUCCESS) {
            DWORD dwMHz = *dwMHzbuff;
            std::cout << "  Clock Speed: " << std::fixed << std::setprecision(2) << (dwMHz / 1000.0) << "GHz\n";
            delete dwMHzbuff;
        }
    }

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

void cpuCompactInfo() {
    GetSystemInfo(&sysinfo);
    int CPUInfo[4] = {-1};
    HKEY hKey;
    DWORD dwType, dwSize;

    char cpuName[49];
    __cpuid(CPUInfo, 0x80000002);
    memcpy(cpuName, CPUInfo, sizeof(CPUInfo));
    __cpuid(CPUInfo, 0x80000003);
    memcpy(cpuName + 16, CPUInfo, sizeof(CPUInfo));
    __cpuid(CPUInfo, 0x80000004);
    memcpy(cpuName + 32, CPUInfo, sizeof(CPUInfo));
    cpuName[48] = '\0';

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

    std::cout << "CPU: " << cpuName << physicalCores << "c/" << sysinfo.dwNumberOfProcessors << "t\n";
}