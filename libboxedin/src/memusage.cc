/*******************************************************************************
 * File: memusage.cc
 *
 * Description:
 * Cross platform memory usage
 ******************************************************************************/
#include <memusage.h>

#if defined(__linux__)

#include <sys/time.h>
#include <sys/resource.h>


bool GetMemUsage(MemUsage& mem_usage)
{
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0)
    {
        mem_usage.max_resident_set_size = (uint64_t)usage.ru_maxrss * 1000;
        mem_usage.resident_set_size = 0; // unsupported
        mem_usage.num_page_reclaims = usage.ru_minflt;
        mem_usage.num_page_faults = usage.ru_majflt;
        mem_usage.num_swaps = usage.ru_nswap;
        mem_usage.max_size = 0; // unsupported
        mem_usage.size = 0; // unsupported

        return true;
    }

    return false;
}

#elif defined(_WIN32)

#include <windows.h>
#include <psapi.h>

bool GetMemUsage(MemUsage& mem_usage)
{
    DWORD processID;
    HANDLE hProcess;
    PROCESS_MEMORY_COUNTERS pmc;

    processID = GetCurrentProcessId();

    hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION |
                                    PROCESS_VM_READ,
                                    FALSE, processID );
    if (NULL == hProcess)
        return false;

    if ( GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) )
    {
        mem_usage.max_resident_set_size = pmc.PeakWorkingSetSize;
        mem_usage.resident_set_size = pmc.WorkingSetSize;
        mem_usage.num_page_reclaims = 0; // unsupported
        mem_usage.num_page_faults = pmc.PageFaultCount;
        mem_usage.num_swaps = 0; // unsupported
        mem_usage.max_size = pmc.PeakPagefileUsage;
        mem_usage.size = pmc.PagefileUsage;
    }

    CloseHandle( hProcess );

    return true;
}


#else

# error "GetMemUsage() has not been ported to your OS."

#endif
