#include "CPUInspectorWindows.h"
#include <windows.h>
#include <unordered_map>
#include <string>
#include <QString>


// std::cout << "Architecture: " << si.wProcessorArchitecture << "\n";
// std::cout << "Number of processors: " << si.dwNumberOfProcessors << "\n";
// std::cout << "Processor type: " << si.dwProcessorType << "\n";
// std::cout << "Page size: " << si.dwPageSize << "\n";
// std::cout << "Processor level: " << si.wProcessorLevel << "\n";
// std::cout << "Processor revision: " << si.wProcessorRevision << "\n";

CPUInspectorWindows::CPUInspectorWindows() {
    std::unordered_map<std::string, int> commonInfo;
}


int CPUInspectorWindows::getCPUInfo(){
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);
    QString revision = QString::number(systemInfo.wProcessorLevel);
    return 0;
}
