/**
 * \file memusage.h
 * \brief This file contains cross platform memory usage declarations.
 ******************************************************************************/
#ifndef MEM_USAGE_H__
#define MEM_USAGE_H__

#include <cstdint>

/**
   \struct MemUsage
   \brief This struct holds process memory statistics
 */
struct MemUsage
{
    uint64_t max_resident_set_size; /** max physical memory (bytes) */
    uint64_t resident_set_size; /** current physical memory (bytes) */
    uint64_t num_page_reclaims; /** page reclaims (soft page faults) */
    uint64_t num_page_faults; /** page faults (hard page faults) */
    uint64_t num_swaps; /** num RAM swaps to hard disk */
    uint64_t max_size; /** max physical + virtual memory (bytes) */
    uint64_t size; /** current physical + virtual memory (bytes) */
};

/**
   \brief This function calls a platform-specific function to memory usage.
   \param[out] mem_usage A reference to a MemUsage to fill with memory info.
   \returns true for success; false for fail.
 */
bool GetMemUsage(MemUsage& mem_usage);

#endif
