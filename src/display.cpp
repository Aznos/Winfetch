#include "inc/display.h"

void displayInfo() {
    std::cout << "Display Information:\n";

    HRESULT hr = S_OK;
    IDXGIFactory1* pFactory = nullptr;
    hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)(&pFactory));
    if(FAILED(hr)) {
        std::cout << "Failed to create DXGIFactory\n";
        return;
    }

    std::vector<IDXGIAdapter1*> adapters;
    IDXGIAdapter1* pAdapter = nullptr;
    for(UINT i = 0; pFactory->EnumAdapters1(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; i++) {
        adapters.push_back(pAdapter);
    }

    for(UINT i = 0; i < adapters.size(); i++) {
        DXGI_ADAPTER_DESC1 desc;
        adapters[i]->GetDesc1(&desc);

        std::cout << "  Adapter " << i << ":\n";
        std::wcout << L"    Description: " << desc.Description << std::endl;
        std::cout << "      Vendor ID: 0x" << std::hex << desc.VendorId << std::dec << std::endl; 
        std::cout << "      Device ID: 0x" << std::hex << desc.DeviceId << std::dec << std::endl; 
        std::cout << "      Subsystem ID: 0x" << std::hex << desc.SubSysId << std::dec << std::endl; 
        std::cout << "      Dedicated Video Memory: " << desc.DedicatedVideoMemory / (1024 * 1024) << "MB\n";
        std::cout << "      Shared System Memory: " << desc.SharedSystemMemory / (1024 * 1024) << "MB\n";
        std::cout << "      Adapter LUID: " << desc.AdapterLuid.LowPart << "-" << desc.AdapterLuid.HighPart << std::endl;
    }

    if(adapters.empty()) {
        std::cout << "  No adapters found\n";
    }

    if(pFactory) {
        pFactory->Release();
    }
}