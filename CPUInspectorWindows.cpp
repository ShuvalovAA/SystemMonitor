#include "CPUInspectorWindows.h"

uint64_t _FiletimeToInt(const FILETIME& ft) {
    ULARGE_INTEGER uli;
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;
    return uli.QuadPart;
}

CPUInspectorWindows::CPUInspectorWindows() {
    HMODULE hNtDll = GetModuleHandleW(L"ntdll.dll");
    if (hNtDll) {
        pNtQuerySystemInformation = (PNtQuerySystemInformation)GetProcAddress(hNtDll, "NtQuerySystemInformation");
    }

    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    numCores = sysInfo.dwNumberOfProcessors;

    prevCoreInfo.resize(numCores);
    QueryCoreInfo(prevCoreInfo);
}

// ВОТ ОНА - ФУНКЦИЯ, КОТОРУЮ НЕ МОГ НАЙТИ КОМПИЛЯТОР:
double CPUInspectorWindows::getCPUInfo() {
    FILETIME idleTime, kernelTime, userTime;
    if (!GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
        return 0.0;
    }

    uint64_t idle = _FiletimeToInt(idleTime);
    uint64_t total = _FiletimeToInt(kernelTime) + _FiletimeToInt(userTime);

    if (prevTotal == 0) {
        prevTotal = total;
        prevIdle = idle;
        return 0.0;
    }

    uint64_t idleDiff = idle - prevIdle;
    uint64_t totalDiff = total - prevTotal;
    prevTotal = total;
    prevIdle = idle;

    if (totalDiff == 0) {
        return 0.0;
    }

    return 100.0 * (totalDiff - idleDiff) / totalDiff;
}

QVector<qreal> CPUInspectorWindows::getPerCoreUsage() {
    QVector<MY_PROCESSOR_PERF_INFO> currentInfo(numCores);
    QVector<qreal> usage(numCores, 0.0);

    if (!QueryCoreInfo(currentInfo)) {
        return usage;
    }

    for (int i = 0; i < numCores; ++i) {
        uint64_t idleDiff = currentInfo[i].IdleTime.QuadPart - prevCoreInfo[i].IdleTime.QuadPart;
        uint64_t totalDiff = (currentInfo[i].KernelTime.QuadPart + currentInfo[i].UserTime.QuadPart) -
                             (prevCoreInfo[i].KernelTime.QuadPart + prevCoreInfo[i].UserTime.QuadPart);

        if (totalDiff > 0) {
            usage[i] = 100.0 * (totalDiff - idleDiff) / totalDiff;
        }
    }
    prevCoreInfo = currentInfo;
    return usage;
}

bool CPUInspectorWindows::QueryCoreInfo(QVector<MY_PROCESSOR_PERF_INFO>& info) {
    if (!pNtQuerySystemInformation) return false;

    ULONG size = 0;
    pNtQuerySystemInformation((SYSTEM_INFORMATION_CLASS)8, nullptr, 0, &size);
    if (size == 0) return false;

    info.resize(size / sizeof(MY_PROCESSOR_PERF_INFO));

    NTSTATUS status = pNtQuerySystemInformation(
        (SYSTEM_INFORMATION_CLASS)8,
        info.data(),
        size,
        &size
        );
    return status >= 0;
}
