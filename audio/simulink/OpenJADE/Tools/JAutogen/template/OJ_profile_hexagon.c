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

#include "OJ_profile.h"


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
        pProfileContext[i].pktcnt_min = 0x70000000;
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
        pProfileContext[profile_target].start_pktcounter = qurt_thread_pktcount_get(qurt_thread_get_id());
        pProfileContext[profile_target].start_cyccounter = qurt_profile_get_thread_pcycles();
    }
}

void OJ_profileStop(unsigned int profile_target)
{
    Profile_Context* puser = &pProfileContext[profile_target];
    unsigned int end_pkt, end_cyc, diff;
    

    end_pkt = qurt_thread_pktcount_get(qurt_thread_get_id());
    end_cyc = qurt_profile_get_thread_pcycles();

    diff = (end_pkt - puser->start_pktcounter);
	puser->pktcnt_min = (puser->pktcnt_min < diff) ? (puser->pktcnt_min) : (diff);
	puser->pktcnt_max = (puser->pktcnt_max > diff) ? (puser->pktcnt_max) : (diff);
	//puser->pktcnt_avg = (unsigned int)(((float)puser->pktcnt_avg * (float)puser->prof_counter + (float)diff) / (float)(puser->prof_counter + 1));
    puser->pktcnt_avg = (unsigned int)(((unsigned long long)puser->pktcnt_avg * (unsigned long long)puser->prof_counter + (unsigned long long)diff) / (unsigned long long)(puser->prof_counter + 1));
    

    diff = (end_cyc - puser->start_cyccounter);
	puser->cyccnt_min = (puser->cyccnt_min < diff) ? (puser->cyccnt_min) : (diff);
	puser->cyccnt_max = (puser->cyccnt_max > diff) ? (puser->cyccnt_max) : (diff);
	//puser->cyccnt_avg = (unsigned int)(((float)puser->cyccnt_avg * (float)puser->prof_counter + (float)diff) / (float)(puser->prof_counter + 1));
	puser->cyccnt_avg = (unsigned int)(((unsigned long long)puser->cyccnt_avg * (unsigned long long)puser->prof_counter + (unsigned long long)diff) / (unsigned long long)(puser->prof_counter + 1));
	puser->prof_counter += 1;
}

#ifdef SAVE_PROFILING_TO_LOCAL
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
#endif

