#include "inc/mb.h"

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