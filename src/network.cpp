#include "inc/network.h"

void netInfo() {
    ULONG outBufLen = sizeof(IP_ADAPTER_INFO);
    PIP_ADAPTER_INFO pInfo = (IP_ADAPTER_INFO*) malloc(outBufLen);
    if(pInfo == NULL) {
        std::cout << "  Could not allocate memory correctly\n";
        return;
    }

    DWORD dwRetVal = GetAdaptersInfo(pInfo, &outBufLen);
    if(dwRetVal == ERROR_BUFFER_OVERFLOW) {
        free(pInfo);
        pInfo = (IP_ADAPTER_INFO*) malloc(outBufLen);

        if(pInfo == NULL) {
            std::cout << "  Could not allocate memory correctly\n";
            return;
        }

        dwRetVal = GetAdaptersInfo(pInfo, &outBufLen);
    }

    if(dwRetVal != ERROR_SUCCESS) {
        std::cout << "Network info had an error\n";
        return;
    }

    std::cout << "Network Information:\n";

    PIP_ADAPTER_INFO pAdapter = pInfo;
    int count = 0;
    while(pAdapter != NULL) {
        count++;
        std::cout << "  Network adapter " << count << ":\n";
        std::cout << "      Name: " << pAdapter->AdapterName << std::endl;
        std::cout << "      Description: " << pAdapter->Description << std::endl;
    
        pAdapter = pAdapter->Next;
    }

    if(pInfo != NULL) {
        free(pInfo);
    }
}