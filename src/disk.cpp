#include "inc/disk.h"

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