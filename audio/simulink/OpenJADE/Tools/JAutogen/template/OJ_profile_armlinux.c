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
#include <stdio.h>
#include "OJ_profile.h"



#define PROFILING_LOCAL_FILE         "./profile_data.bin"


// MIPS parameter table, this should be calculated automatically in code generation.
const float BasicAudio_Target_mipspara[MAX_NUM_PROFILING] = {
    // step functions and feature processing functions
    0.00200000,
};


// Profiling Mask specifies the enable/disable of each profiling
bool BasicAudio_Target_profilemask[MAX_NUM_PROFILING];
// Hold profiling data for all Step functions and features
Profile_Context BasicAudio_Target_profiledata[MAX_NUM_PROFILING];


// Profiling Mask Pointer
bool* pProfileMask = BasicAudio_Target_profilemask;
// Profiling Context Pointer
Profile_Context* pProfileContext = BasicAudio_Target_profiledata;
// Profiling MIPS parameter table Pointer
const float* pProfileMipsPara = BasicAudio_Target_mipspara;
// Profiling maxinum of instances
const unsigned int max_num_profiling = MAX_NUM_PROFILING;


struct timeval te_begin, te_end;


/*************************************************************************************************/
/********************************* The APIs exposed to OpenJADE **********************************/
/*************************************************************************************************/
void OJ_profileInit(void)
{
    int i;


	// Here "true" in memset for test purpose
    memset(pProfileMask, true, sizeof(bool) * max_num_profiling);
    memset(pProfileContext, 0, sizeof(Profile_Context) * max_num_profiling);
    for (i = 0; i < max_num_profiling; i ++) {
        pProfileContext[i].tmcnt_min = 0x70000000;
        pProfileContext[i].cyccnt_min = 0x70000000;
    }
}


void OJ_profileEnable(unsigned int profile_target)
{
    pProfileMask[profile_target] = true;
}


void OJ_profileDisable(unsigned int profile_target)
{
    pProfileMask[profile_target] = false;
}


void OJ_profileReset(unsigned int profile_target)
{
    ;
}


void OJ_profileThreshold(float percentage)
{
    ;
}


void OJ_profileCycleOrMips(unsigned int cycle_or_mips)
{
    ;
}


void OJ_profileStart(unsigned int profile_target)
{
    if (pProfileMask[profile_target] == true) {
        gettimeofday(&(pProfileContext[profile_target].start_tmcounter), NULL);
        //pProfileContext[profile_target].start_cyccounter = qurt_profile_get_thread_pcycles();
    }
}

void OJ_profileStop(unsigned int profile_target)
{
    Profile_Context* puser = &pProfileContext[profile_target];
    struct timeval te_end;
    long diff;
    

    gettimeofday(&te_end, NULL);;
    diff = (te_end - puser->start_pktcounter);
    if (diff > 0) {
	    puser->tmcnt_min = (puser->tmcnt_min < diff) ? (puser->tmcnt_min) : (diff);
	    puser->tmcnt_max = (puser->tmcnt_max > diff) ? (puser->tmcnt_max) : (diff);
        puser->tmcnt_avg = (unsigned int)(((unsigned long long)puser->tmcnt_avg * (unsigned long long)puser->prof_counter + (unsigned long long)diff) / (unsigned long long)(puser->prof_counter + 1));
    }
    puser->prof_counter += 1;
}


void OJ_profileSaveToFile(void)
{
    int i;
    FILE *fp;

    fp = fopen(PROFILING_LOCAL_FILE, "wb");
    if (fp != NULL) {
        for (i = 0; i < max_num_profiling; i ++) {
            fwrite((void*)(&pProfileContext[i]), 1, sizeof(Profile_Context), fp);
        }
    }
    fclose(fp);
}


