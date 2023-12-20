#include "inc/gpu.h"

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

void gpuCompactInfo() {
    IDirect3D9* d3d = Direct3DCreate9(D3D_SDK_VERSION);

    if(!d3d) {
        std::cout << "No graphics card found\n";
        return;
    }

    D3DADAPTER_IDENTIFIER9 adapter;

    if(d3d->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &adapter) == D3D_OK) {
        std::cout << "GPU: " << adapter.Description << std::endl;
    }
}