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

int main() {
    std::cout << "Winfetch v0.0.3\n";

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