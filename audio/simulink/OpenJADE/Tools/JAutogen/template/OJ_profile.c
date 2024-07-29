/*
 * BOSE CORPORATION
 * COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 *    BOSE CORPORATION
 *    The Mountain
 *    Framingham, MA 01701-9168
 *
 * File: OJ_profile.c
 *
 */
#define __LONG_LONG_PROCESSOR_TIME__

#include "rtwtypes.h"
#include "ASDID.h"

#include <sys/platform.h>
#include <builtins.h>

#include "OJ_profile.h"








// MIPS parameter table, this should be calculated automatically in code generation.
const real32_T model_1_1_mipspara[MAX_NUM_PROFILING] = {
    // 10 step functions
    (1.0 / 0.0000555) / 1000000,
    (1.0 / 0.0006666) / 1000000,
    (1.0 / 0.0007222) / 1000000,
    (1.0 / 0.0014444) / 1000000,
    (1.0 / 0.0028888) / 1000000,
    (1.0 / 0.0057777) / 1000000,
    (1.0 / 0.0115555) / 1000000,
    (1.0 / 0.0462222) / 1000000,
    (1.0 / 0.1848888) / 1000000,
    (1.0 / 0.5546666) / 1000000,
    // SRCOut features
    (1.0 / 0.0006666) / 1000000,
    (1.0 / 0.0007222) / 1000000
};

// Counter table, this should be calculated automatically in code generation.
const uint32_t model_1_1_counterpara[MAX_NUM_PROFILING] = {
    // 10 step functions
    18001 * PROFILE_STTIME,
    1500 * PROFILE_STTIME,
    1384 * PROFILE_STTIME,
    692 * PROFILE_STTIME,
    346 * PROFILE_STTIME,
    173 * PROFILE_STTIME,
    86 * PROFILE_STTIME,
    21 * PROFILE_STTIME,
    5 * PROFILE_STTIME,
    2 * PROFILE_STTIME,
    // SRCOut features
    1500 * PROFILE_STTIME,
    1384 * PROFILE_STTIME
};

// Profiling Mask specifies the enable/disable of each profiling
bool model_1_1_profilemask[MAX_NUM_PROFILING];
// Hold profiling data for all Step functions and features
Profile_Context model_1_1_profiledata[MAX_NUM_PROFILING];


// Profiling Mask Pointer
bool* pProfileMask = model_1_1_profilemask;
// Profiling Context Pointer
Profile_Context* pProfileContext = model_1_1_profiledata;
// Profiling MIPS parameter table Pointer
const real32_T* pProfileMipsPara = model_1_1_mipspara;
// Profiling Counter table Pointer
const uint32_t* pProfileCounterPara = model_1_1_counterpara;
// Profiling maxinum of instances
const uint32_t max_num_profiling = MAX_NUM_PROFILING;


// Profiling Configuration
uint32_t profile_cycleOrmips;
// Profiling threshold (0 - 1)
real32_T profile_threshold;


/*************************************************************************************************/
/********************************* The APIs exposed to OpenJADE **********************************/
/*************************************************************************************************/
void OJ_profileInit(void)
{
    profile_cycleOrmips = PROFILE_IN_CYCLE;
    profile_threshold = PROFILE_THRESHOLD;
	// Here "true" in memset for test purpose
    memset(pProfileMask, true, sizeof(bool) * max_num_profiling);
    memset(pProfileContext, 0, sizeof(Profile_Context) * max_num_profiling);
}


void OJ_profileEnable(uint32_t profile_target)
{
    pProfileMask[profile_target] = true;
}


void OJ_profileDisable(uint32_t profile_target)
{
    pProfileMask[profile_target] = false;
}


void OJ_profileReset(uint32_t profile_target)
{
    Profile_Context* puser = &pProfileContext[profile_target];

    puser->peak_value = 0;
    puser->avg_value = 0;
    puser->prof_counter = 0;
    puser->over_counter = 0;
}


void OJ_profileThreshold(real32_T percentage)
{
    profile_threshold = percentage;
}


void OJ_profileCycleOrMips(uint32_t cycle_or_mips)
{
    profile_cycleOrmips = cycle_or_mips;
}

#ifndef PROFILE_STARTSTOP_INLINE
void OJ_profileStart(uint32_t profile_target)
{
    if (pProfileMask[profile_target] == true) {
        pProfileContext[profile_target].start_counter = READ_CLOCKCYCLE;
    }
}


void OJ_profileStop(uint32_t profile_target)
{
    real32_T tmp;
    uint64_t final_counter = READ_CLOCKCYCLE;
    Profile_Context* puser = &pProfileContext[profile_target];
    

    puser->cur_cycles += (final_counter - puser->start_counter);
    puser->start_counter = 0;
    tmp = (profile_cycleOrmips == PROFILE_IN_MIPS) ? (real32_T)(puser->cur_cycles * pProfileMipsPara[profile_target]) : (real32_T)(puser->cur_cycles);
    puser->avg_value = (tmp + puser->prof_counter * puser->avg_value) / (puser->prof_counter + 1);
    puser->peak_value = (puser->peak_value > tmp) ? (puser->peak_value) : (tmp);
    puser->prof_counter += 1;
    if (puser->prof_counter > pProfileCounterPara[profile_target]) {
        // Do statistical collection
        if (tmp - puser->avg_value >= puser->avg_value * PROFILE_THRESHOLD) {
            puser->over_counter += 1;

        }
    }
    puser->cur_cycles = 0; 
}
#endif


void ContextSwitchHook(void* current_task_ptr, void* ready_task_ptr)
{
    int i;
    Profile_Context* puser_profile = pProfileContext;


    for (i = 0; i < max_num_profiling; i ++) {
        if (puser_profile->start_counter > 0) {
            // The first time that the profiling task is preempted by another task
            if (puser_profile->taskptr == NULL) {
                // Copy the profiling task pointer
                puser_profile->taskptr = current_task_ptr;
                puser_profile->othertask_start_counter = READ_CLOCKCYCLE;
            }
            else if (puser_profile->taskptr == ready_task_ptr) {
                // Switch back to the profiling task
                puser_profile->cur_cycles += (puser_profile->othertask_start_counter - puser_profile->start_counter);
                puser_profile->start_counter = READ_CLOCKCYCLE;
                puser_profile->taskptr = NULL;
            }
        }
        ++ puser_profile;
    }
}
