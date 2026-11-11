#pragma once
#include <windows.h>
#include <winternl.h>
#include <QVector>


typedef struct _MY_PROCESSOR_PERF_INFO {
    LARGE_INTEGER IdleTime;
    LARGE_INTEGER KernelTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER DpcTime;
    LARGE_INTEGER InterruptTime;
    ULONG InterruptCount;
} MY_PROCESSOR_PERF_INFO, *PMY_PROCESSOR_PERF_INFO;

typedef NTSTATUS(WINAPI* PNtQuerySystemInformation)(
    SYSTEM_INFORMATION_CLASS SystemInformationClass,
    PVOID SystemInformation,
    ULONG SystemInformationLength,
    PULONG ReturnLength
    );

class CPUInspectorWindows {
public:
    CPUInspectorWindows();
    double getCPUInfo();

    QVector<qreal> getPerCoreUsage();

private:
    uint64_t prevTotal = 0;
    uint64_t prevIdle = 0;

    PNtQuerySystemInformation pNtQuerySystemInformation = nullptr;
    int numCores = 0;

    QVector<MY_PROCESSOR_PERF_INFO> prevCoreInfo;

    bool QueryCoreInfo(QVector<MY_PROCESSOR_PERF_INFO>& info);
};
