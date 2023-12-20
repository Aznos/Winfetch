#include "inc/bios.h"

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