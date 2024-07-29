/*******************************************************************************
 * BOSE CORPORATION
 * COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:
 * BOSE CORPORATION
 * The Mountain
 * Framingham, MA 01701-9168
 ********************************************************************************
 * Revision: $Header$
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <cycle_count.h>

#include "TalariaAPI.h"
#include "ProfileConfig.h"

#include MODEL_HEADER_FILENAME

//-------------------------------------------------------------------------
//  LOCAL DATA STORAGE
//-------------------------------------------------------------------------
static boolean_T           modelStepFlags    [MODEL_SYNC_TASK_COUNT];

/*******************************************************************************
* MIPS test runner
*******************************************************************************/

int main(int argc, char *argv[])
{

    // Reset the system
    #ifndef COMPILE_WITHOUT_MODEL
    MODEL_FUNC(initialize)();
    #endif

    cycle_t start_counter;
    cycle_t cycles;
    cycle_t cycles_min[MODEL_SYNC_TASK_COUNT];
    cycle_t cycles_max[MODEL_SYNC_TASK_COUNT];
    int i, j;
    float samplePeriods[] = { MODEL_TASK_SAMPLE_PERIODS };
    float mips;
    int counter[MODEL_SYNC_TASK_COUNT] = {0};
    float mips_sum[MODEL_SYNC_TASK_COUNT] = {0};
    float mips_min[MODEL_SYNC_TASK_COUNT] = {0};
    float mips_max[MODEL_SYNC_TASK_COUNT] = {0};
    unsigned long cycles_sum[MODEL_SYNC_TASK_COUNT] = {0};

    for (j = 0; j < MODEL_PROFILE_RUN_COUNT; j++)
    {
        // Figure out what rates should run
        MODEL_RUN_FLAGS_UPDATE(&modelStepFlags[0]);

        modelStepFlags[0] = 1;

        for (i = 0; i < MODEL_SYNC_TASK_COUNT; i++)
        {
            if (modelStepFlags[i])
            {
                counter[i]++;
                START_CYCLE_COUNT(start_counter);
                #ifndef COMPILE_WITHOUT_MODEL
                MODEL_STEP(i);
                #endif
                STOP_CYCLE_COUNT(cycles, start_counter);

                mips = cycles/samplePeriods[i]/1e6;
                mips_sum[i] = mips_sum[i] + mips;
                cycles_sum[i] = cycles_sum[i] + cycles;

                // Output stats if running only once
                if (MODEL_PROFILE_RUN_COUNT == 1)
                {
                    printf("Step %d cycle count:", i);
                    PRINT_CYCLES(" ", cycles);
                    printf("Step %d mips: %f\n\n", i, mips);
                }

                // Compare and store the min and max cycles and mips
                if ((mips < mips_min[i]) || (mips_min[i] == 0))
                {
                    mips_min[i] = mips;
                }
                if ((mips > mips_max[i]) || (mips_max[i] == 0))
                {
                    mips_max[i] = mips;
                }
                if ((cycles < cycles_min[i]) || (cycles_min[i] == 0))
                {
                    cycles_min[i] = cycles;
                }
                if ((cycles > cycles_max[i]) || (cycles_max[i] == 0))
                {
                    cycles_max[i] = cycles;
                }
            }
        }
    }

    // Output stats if ran more than once
    if (MODEL_PROFILE_RUN_COUNT != 1)
    {
        printf("Results after profiling for %d frame(s).\n\n",MODEL_PROFILE_RUN_COUNT);

        for (i = 0; i < MODEL_SYNC_TASK_COUNT; i++)
        {
            printf("Step %d run count: %d\n", i, counter[i]);
            printf("Step %d average mips: %f\n", i, mips_sum[i]/counter[i]);
            printf("Step %d max mips: %f\n", i, mips_max[i]);
            printf("Step %d min mips: %f\n", i, mips_min[i]);
            printf("Step %d cycle count average: %d\n", i, cycles_sum[i]/counter[i]);
            printf("Step %d cycle count maximum: %d\n",i,cycles_max[i]);
            printf("Step %d cycle count minimum: %d\n\n",i,cycles_min[i]);
        }
    }

    return 0;
}

