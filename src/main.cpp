#include "inc/main.h"
#include "inc/cpu.h"
#include "inc/ram.h"
#include "inc/gpu.h"
#include "inc/disk.h"
#include "inc/os.h"
#include "inc/bios.h"
#include "inc/mb.h"
#include "inc/network.h"
#include "inc/display.h"

int main(int argc, char** argv) {
    std::string version = "Winfetch v0.0.3\n";

    if(argc > 1) {
        if(strcmp(argv[1], "--help") == 0) {
            std::cout << "Usage: winfetch [OPTION]\n";
            std::cout << "Options:\n";
            std::cout << "  --help\t\tDisplay this help message\n";
            std::cout << "  --version\t\tDisplay version information\n";
            std::cout << "  --compact\t\tDisplay information in a compact format\n";
            return 0;
        } else if(strcmp(argv[1], "--version") == 0) {
            std::cout << version;
            return 0;
        } else if(strcmp(argv[1], "--compact") == 0) {
            auto start = std::chrono::steady_clock::now();

            cpuCompactInfo();
            ramCompactInfo();
            gpuInfo();
            diskInfo();

            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::milli> elapsed = end - start;
            std::cout << std::endl << "Got information in " << elapsed.count() << "ms\n";

            return 0;
        } else {
            std::cout << "Invalid argument! Use --help for help.\n";
            return 1;
        }
    }


    auto start = std::chrono::steady_clock::now();

    cpuInfo();
    ramInfo();
    gpuInfo();
    diskInfo();
    osInfo();
    biosInfo();
    mbInfo();
    netInfo();
    displayInfo();

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << std::endl << "Got information in " << elapsed.count() << "ms\n";

    return 0;
}