/*
 * BOSE CORPORATION
 * COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 *    BOSE CORPORATION
 *    The Mountain
 *    Framingham, MA 01701-9168
 *
 * File: OJ_profile.h
 */


#ifndef OJ_PROFILE_H_
#define OJ_PROFILE_H_


#include "rtwtypes.h"

#define PROFILE_STARTSTOP_INLINE               (1)


#define STEP0_PROFILING                        (0)
#define STEP1_PROFILING                        (1)
#define STEP2_PROFILING                        (2)
#define STEP3_PROFILING                        (3)
#define STEP4_PROFILING                        (4)
#define STEP5_PROFILING                        (5)
#define STEP6_PROFILING                        (6)
#define STEP7_PROFILING                        (7)
#define STEP8_PROFILING                        (8)
#define STEP9_PROFILING                        (9)
#define SRCOUT_RATE1_PROFILING                 (10)
#define SRCOUT_RATE2_PROFILING                 (11)
#define MAX_NUM_PROFILING                      (12)




// Profile threshold
#define PROFILE_THRESHOLD            (0.1)
// Profile statiscal start time
#define PROFILE_STTIME               (5)
// Read DSP Clock
#define READ_CLOCKCYCLE              (__builtin_emuclk())


// Profile in Cycles or MIPS
#define PROFILE_IN_CYCLE             (1)
#define PROFILE_IN_MIPS              (2)


typedef struct {
    real32_T peak_value;
    real32_T avg_value;
    uint32_t over_counter;
    void* taskptr;
    uint64_t cur_cycles;
    uint64_t start_counter;
    uint64_t prof_counter;
    uint64_t othertask_start_counter;
    uint64_t othertask_accum_counter;
} Profile_Context;






extern const real32_T model_1_1_mipspara[MAX_NUM_PROFILING];
extern const uint32_t model_1_1_counterpara[MAX_NUM_PROFILING];


// Profiling Mask specifies the enable/disable of each profiling
extern bool model_1_1_profilemask[MAX_NUM_PROFILING];
// Hold profiling data for all Step functions and features
extern Profile_Context model_1_1_profiledata[MAX_NUM_PROFILING];


// Profiling Mask Pointer
extern bool* pProfileMask;
// Profiling Context Pointer
extern Profile_Context* pProfileContext;
// Profiling MIPS parameter table Pointer
extern const real32_T* pProfileMipsPara;
// Profiling Counter table Pointer
extern const uint32_t* pProfileCounterPara;


// Profiling Configuration
extern uint32_t profile_cycleOrmips;
// Profiling threshold (0 - 1)
extern real32_T profile_threshold;






void OJ_profileInit(void);
void OJ_profileEnable(uint32_t profile_target);
void OJ_profileDisable(uint32_t profile_target);
void OJ_profileReset(uint32_t profile_target);
void OJ_profileThreshold(real32_T percentage);
void OJ_profileCycleOrMips(uint32_t cycle_or_mips);


#ifdef PROFILE_STARTSTOP_INLINE
static inline void OJ_profileStart(uint32_t profile_target)
{
    if (pProfileMask[profile_target] == true) {
        pProfileContext[profile_target].start_counter = READ_CLOCKCYCLE;
    }
}


static inline void OJ_profileStop(uint32_t profile_target)
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
#else
void OJ_profileStart(uint32_t profile_target);
void OJ_profileStop(uint32_t profile_target);
#endif


void ContextSwitchHook(void* current_task_ptr, void* ready_task_ptr);


#endif


