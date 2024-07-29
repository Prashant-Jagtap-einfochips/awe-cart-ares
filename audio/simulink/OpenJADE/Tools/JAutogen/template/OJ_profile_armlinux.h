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


#include <stdbool.h>
#include <sys/time.h>


#define STEP0_PROFILING                                             (0)
#define MAX_NUM_PROFILING                                           (1)







typedef struct {
    unsigned int tmcnt_min;
    unsigned int tmcnt_avg;
    unsigned int tmcnt_max;
    unsigned int cyccnt_min;
    unsigned int cyccnt_avg;
    unsigned int cyccnt_max;
    struct timeval start_tmcounter;
    unsigned int start_cyccounter;
    unsigned int prof_counter;
} Profile_Context;


extern const float BasicAudio_Target_mipspara[MAX_NUM_PROFILING];
extern const unsigned int BasicAudio_Target_counterpara[MAX_NUM_PROFILING];


// Profiling Mask specifies the enable/disable of each profiling
extern bool BasicAudio_Target_profilemask[MAX_NUM_PROFILING];
// Hold profiling data for all Step functions and features
extern Profile_Context BasicAudio_Target_profiledata[MAX_NUM_PROFILING];
// Profiling Mask Pointer
extern bool* pProfileMask;
// Profiling Context Pointer
extern Profile_Context* pProfileContext;
// Profiling MIPS parameter table Pointer
extern const float* pProfileMipsPara;
// Profiling Counter table Pointer
//extern const unsigned int* pProfileCounterPara;







void OJ_profileInit(void);
void OJ_profileEnable(unsigned int profile_target);
void OJ_profileDisable(unsigned int profile_target);
void OJ_profileReset(unsigned int profile_target);
void OJ_profileThreshold(float percentage);
void OJ_profileCycleOrMips(unsigned int cycle_or_mips);

void OJ_profileSaveToFile(void);





#endif

