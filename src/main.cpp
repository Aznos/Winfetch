#include <iostream>
#include <windows.h>
#include <intrin.h>
#include <iomanip>
#include <vector>
#include <chrono>
#include <d3d9.h>

SYSTEM_INFO sysinfo;

void cpuInfo() {
    GetSystemInfo(&sysinfo);
    int CPUInfo[4] = {-1};

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

    __cpuid(CPUInfo, 0x16);
    unsigned int baseClock = CPUInfo[0] & 0xFFFF;
    unsigned int maxClock = CPUInfo[1] & 0xFFFF;
    std::cout << "  Clock Speed: " << (baseClock / 1000.0) << "GHz (Max: " << (maxClock / 1000.0) << "GHz)\n";

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

    std::cout << "  Computer Name: ";
    std::wstring pcName;
    DWORD bufferSize = MAX_COMPUTERNAME_LENGTH + 1;
    pcName.resize(bufferSize);
    GetComputerNameExW(ComputerNamePhysicalDnsHostname, &pcName[0], &bufferSize);
    std::wcout << pcName.c_str() << "\n";

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

    ULONGLONG uptime = GetTickCount64();

    int days = uptime / (24 * 60 * 60 * 1000);
    int hours = (uptime % (24 * 60 * 60 * 1000)) / (60 * 60 * 1000);
    int mins = (uptime % (60 * 60 * 1000)) / (60 * 1000);
    int secs = (uptime % (60 * 1000)) / 1000;

    std::cout << "  Uptime: ";
    if(days > 0) {
        std::cout << days << " days, ";
    }

    std::cout << hours << " hours, " << mins << " minutes, " << secs << " seconds\n";
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

void gpuInfo() {
    std::cout << "GPU Information:\n";

    IDirect3D9* d3d = Direct3DCreate9(D3D_SDK_VERSION);

    if(!d3d) {
        std::cout << "  No graphics card found\n";
        return;
    }

    D3DADAPTER_IDENTIFIER9 adapter;

    if(d3d->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &adapter) == D3D_OK) {
        std::cout << "  Name: " << adapter.Description << std::endl;
        std::cout << "  Vendor: " << adapter.VendorId << std::endl;
        std::cout << "  Driver Version: " << adapter.DriverVersion.HighPart << "." << adapter.DriverVersion.LowPart << std::endl;
    }
}

void diskInfo() {
    std::cout << "Disk Information:\n";

    DWORD drives = GetLogicalDrives();
    std::vector<char> buffer(MAX_PATH);

    for(char letter = 'A'; letter <= 'Z'; letter++) {
        if(drives & 1) {
            std::string root = "";
            root += letter;
            root += ":\\";
            UINT type = GetDriveType(root.c_str());
            if(type == DRIVE_FIXED || type == DRIVE_REMOVABLE || type == DRIVE_REMOTE) {
                ULARGE_INTEGER freeSpace, totalSpace, totalFreeSpace;
                if(GetDiskFreeSpaceEx(root.c_str(), &freeSpace, &totalSpace, &totalFreeSpace)) {
                    std::cout << "  Drive " << letter << ":\n";
                    std::cout << "      Type: ";
                    switch(type) {
                        case DRIVE_FIXED:
                            std::cout << "Fixed\n";
                            break;
                        case DRIVE_REMOVABLE:
                            std::cout << "Removable\n";
                            break;
                        case DRIVE_REMOTE:
                            std::cout << "Remote\n";
                            break;
                        default:
                            std::cout << "Unknown\n";
                            break;
                    }
                    std::cout << "      File System: ";
                    if(GetVolumeInformation(root.c_str(), &buffer[0], MAX_PATH, NULL, NULL, NULL, NULL, 0)) {
                        std::cout << &buffer[0] << "\n";
                    } else {
                        std::cout << "Unknown\n";
                    }
                    std::cout << "      Total Space: " << totalSpace.QuadPart / (1024 * 1024) << "MB\n";
                    std::cout << "      Free Space: " << freeSpace.QuadPart / (1024 * 1024) << "MB\n";
                }
            }
        }
        drives >>= 1;
    }
}

void biosInfo() {
    std::cout << "BIOS Information:\n";

    HKEY hKey;
    DWORD dwType, dwSize;
    char buffer[255] = {0};

    if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\BIOS", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        dwSize = sizeof(buffer);
        if(RegQueryValueEx(hKey, "BIOSVendor", NULL, &dwType, (LPBYTE)buffer, &dwSize) == ERROR_SUCCESS) {
            std::cout << "  Vendor: " << buffer << std::endl;
        }
        if(RegQueryValueEx(hKey, "BIOSVersion", NULL, &dwType, (LPBYTE)buffer, &dwSize) == ERROR_SUCCESS) {
            std::cout << "  Version: " << buffer << std::endl;
        }
    }
}

void mbInfo() {
    std::cout << "Motherboard Information:\n";

    HKEY hKey;
    DWORD dwType, dwSize;
    char buffer[255] = {0};

    if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\BIOS", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        dwSize = sizeof(buffer);
        if(RegQueryValueEx(hKey, "BaseBoardProduct", NULL, &dwType, (LPBYTE)buffer, &dwSize) == ERROR_SUCCESS) {
            std::cout << "  Name: " << buffer << std::endl;
        }
        if(RegQueryValueEx(hKey, "BaseBoardManufacturer", NULL, &dwType, (LPBYTE)buffer, &dwSize) == ERROR_SUCCESS) {
            std::cout << "  Manufacturer: " << buffer << std::endl;
        }
    }
}

int main() {
    std::cout << "Winfetch v0.0.1\n";

    auto start = std::chrono::steady_clock::now();

    cpuInfo();
    ramInfo();
    gpuInfo();
    diskInfo();
    osInfo();
    biosInfo();
    mbInfo();

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << std::endl << "Got information in " << elapsed.count() << "ms\n";

    return 0;
}