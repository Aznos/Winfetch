#include "inc/os.h"

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

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    int count = 0;

    if(Process32First(snapshot, &entry)) {
        do {
            count++;
        } while(Process32Next(snapshot, &entry));
    }

    CloseHandle(snapshot);

    std::cout << "  Processes running: " << count << std::endl;
}