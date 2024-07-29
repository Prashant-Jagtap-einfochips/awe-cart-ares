/*****************************************************************************************
 *
 * BOSE CORPORATION
 * COPYRIGHT 2014 BOSE CORPORATION ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:

 *     BOSE CORPORATION
 *     The Mountain
 *     Framingham, MA 01701-9168

*****************************************************************************************/

/**
 *  @file pooliirAccelerator.c
 *  @brief Connects model IIR code generation with hardware accelerator manager
 *
 *  @note Optimization settings for speed should include setting compiler
 *        flags -Ov100 and -ipa (if possible) for the top level build. NDEBUG 
 *        should also be defined to turn off asserts. Also, it is important
 *        to note that this module was designed to support the usage of cached
 *        accelerator task memory and DMA output buffers if enabled in the IIR
 *        accelerator driver.
 * 
 *  @ingroup dsp
 *
 */

/***  Include Files ***********************************************************/
#include <stddef.h>
#include <string.h>
#include "pooliirAccelerator.h"
#include "GxpTranslateAddr.h"
#include "assert.h"


/***  External Variable References  *******************************************/


/***  Module Macros  **********************************************************/

// Verifies requested memory size does not exceed actual memory size
#define VERIFY_ITEM_SIZE( reqMemSize, memSize )  assert( (memSize) >= (reqMemSize) )

/* Number of coefficients per Biquad, includes 2 IIR states (Dk) per BIQUAD.
 * Coefficient ordering in memory: Ak0, Ak1, Bk1, Ak2, Bk2, Dk2, Dk1  
 *      where Ak are numerator and Bk are denominator coefficients
 */   
#define NUM_BIQUAD_COEFFS   7


/***  Module Types  ***********************************************************/

// Defines parameters used to create task.  Data residing in work memory
// persists for the life of the accelerator task.  
typedef struct
{
    iirAccelWorkMemory_t *pWork;            // Pointer to work memory
    const unsigned int *iirNumStages;       // Pointer to number of biquads
                                            // for each channel filter
    const float **iirCoeffBuff;             // Pointer to IIR coefficients
    unsigned int taskMemorySize;            // Task memory size

} iirTaskParams_t;


/***  Module Variables  *******************************************************/


/***  Local Function Declarations  ********************************************/

static bool create_iir_accel_task( iirTaskParams_t *params );
static inline void iir_accel_queue_task( iirAccelWorkMemory_t *pWork  );
static void setup_variable_work_memory( iirAccelWorkMemory_t *pWork, unsigned int *taskMemSize );


/***  Module Functions  *******************************************************/

/**
 *  iir_accelerator_hw_init()
 *
 *  @brief One-time initialization of hardware used by IIR accelerator
 *
 *  @param[out]    reserved  Artifact of code generation, not intended for use here
 *  @param[in]     deviceId  Device ID of accelerator to use
 *  @param[in]     coreId    DSP Core ID accelerator device assigned to
 *
 *  @warning It is not recommended to use reserved memory. When passed in, this will actually
 *           point to the first element of workMemory. The reason for this has to do with
 *           making sure Simulink does not optimize out the call to this function. So it is
 *           not recommended to use, as it's value may change outside of these functions.
 */
void iir_accelerator_hw_init( unsigned int *reserved, 
                              unsigned int deviceId,
                              unsigned int coreId )
{
    static bool iirAccelDevHwInitialized[GXP_IIR_NUM_DEVS] = { false, false, false, false, 
                                                               false, false, false, false };
    // Validate input parameters
    assert( deviceId < GXP_IIR_NUM_DEVS );
    assert( (coreId == ADI_CORE_SHARC0) || (coreId == ADI_CORE_SHARC1) );

    if ( iirAccelDevHwInitialized[deviceId] == false )
    {
        // Initialize the GXP IIR HW Accelerator device. Note, this must be called
        // prior to calling IIR accelerator device configuration APIs.
        gxpHamResult_t gxpHamResult = gxpHamIirInitAccelDevice( (gxpHamIirDevice_t) deviceId,
                                                                (gxpHamCoreId_t) coreId );
        assert( gxpHamResult == GXPHAM_SUCCESS );  

        // Perform the IIR accelerator HW device initialization only once for the model
        // when the device block instance is first initialized 
        iirAccelDevHwInitialized[deviceId] = true;
    }

} /* iir_accelerator_hw_init() */


/**
 *  iir_accelerator_init()
 *
 *  @brief Initializes data used by IIR accelerator and creates task list
 *
 *  @param[out]    reserved       Artifact of code generation, not intended for use here
 *  @param[in]     input          Input audio buffer pointer
 *  @param[in]     frameSize      Number of samples per frame
 *  @param[in]     numChannels    Number of channels
 *  @param[in]     numStages      Pointer to array of number of filter SOSs
 *  @param[in]     coeffs         Coefficient buffer pointer for all SOSs
 *  @param[in]     deviceId       Device ID of accelerator to use
 *  @param[in,out] workMemory     Pointer to memory buffer allocated by model, used by this
 *                                accelerator wrapper
 *  @param[in]     workMemorySize Size of work memory allocated by model
 *
 *  @warning It is not recommended to use reserved memory. When passed in, this will actually
 *           point to the first element of workMemory. The reason for this has to do with
 *           making sure Simulink does not optimize out the call to this function. So it is
 *           not recommended to use, as it's value may change outside of these functions.
 */
void iir_accelerator_init( float *reserved,
                           const float *input,
                           unsigned int frameSize,
                           unsigned int numChannels,
                           const unsigned int *numStages,
                           const float *coeffs,
                           unsigned int deviceId,
                           float *workMemory,
                           unsigned int workMemorySize )
{
    // Get pointer to the work memory
    // Note that the work memory will always be initialized to zero 
    // by the model initialize function prior to calling this function.
    iirAccelWorkMemory_t *pWork = (iirAccelWorkMemory_t *) workMemory;

    // Verify work memory is large enough
    unsigned int requiredWorkMemSize = sizeof(*pWork) + 
                                       IIR_VARIABLE_DATA_SECTION_SIZE( numChannels, frameSize );
    requiredWorkMemSize /= sizeof(workMemory[0]);
    assert( workMemorySize >= requiredWorkMemSize );

    // IIR accelerator filter coefficient buffer of pointers to Simulink generated
    // coefficient buffers.
    // Buffer located within variable data memory section.
    const float *iirCoeffBuff[numChannels];

    // Initialize the work memory and get pointers to variable data section
    // members
    pWork->frameSize = frameSize;
    pWork->numChannels = numChannels; 
    
    // Set the device ID of the IIR Accelerator to use for this task
    pWork->iirAccelDevId = (gxpHamIirDevice_t) deviceId;

    // Setup the variable data work memory space and get task memory size
    unsigned int taskMemorySize;
    setup_variable_work_memory( pWork, &taskMemorySize );

    // Current audio input pointer for accelerator optimization
    pWork->pAudioInput = input;

    // ------------------------------------------------------------------------
    // Generate data required to create the IIR accelerator task
    // ------------------------------------------------------------------------
    for( int i = 0, j = 0; i < numChannels; i++ )
    {
        iirCoeffBuff[i] = &coeffs[j];
        j += NUM_BIQUAD_COEFFS * numStages[i];
    }

    // Create the IIR accelerator task
    iirTaskParams_t params =
    {
        pWork,              // Pointer to work memory
        numStages,          // Pointer to number of biquads
                            // for each channel filter
        iirCoeffBuff,       // Pointer to IIR coefficients
        taskMemorySize      // Task memory size
    };
    bool result = create_iir_accel_task( &params );

    // Set flag indicating initialization has completed successfully. 
    if ( result == true )
    {
        pWork->initComplete = true;
    }

} /* iir_accelerator_init() */


/**
 *  iir_accelerator_process()
 *
 *  @brief Run-time processing of IIR accelerator, retrieving accelerator outputs
 *         from previous frame and initiating accelerator processing of current
 *         frame.
 *
 *  @param[out]    output       Output audio buffer pointer
 *  @param[in]     input        Input audio buffer pointer
 *  @param[in,out] workMemory   Pointer to memory buffer allocated by model, used by this
 *                              accelerator wrapper
 */
void iir_accelerator_process( float *output,
                              const float *input,
                              float *workMemory )
{
    // Grab data objects from work memory
    iirAccelWorkMemory_t *pWork = (iirAccelWorkMemory_t *) workMemory;
    
    // Exit if accelerator initialization has not completed or RMDL
    // process has started
    if ( pWork->initComplete == false )
    {
        return;
    }

    // Copy IIR accelerator output buffers from previous frame to Simulink model output
    // buffers for further processing
    const int frameSize = pWork->frameSize;
    const int numChannels = pWork->numChannels;
    float *dest = output;
    float *src = pWork->iirCircOutputDmaBuffs;
    const int count = numChannels * frameSize; 
    for ( int i = 0; i < count; i++ )
    {
        dest[i] = src[i];
    }

    // Update audio input pointer, TCB input base and index in the event the input
    // buffer has changed
    if ( expected_false(pWork->pAudioInput != input) )
    {   
        pWork->pAudioInput = input;

        gxpHamIirTcb_t *pList = pWork->iirTaskMemory->pChannelList;
        const float *pInput = input;
        for ( int i = 0; i < numChannels; i++ )
        {
            // Get input buffer address            
            int index = i * frameSize;
            unsigned int inputAddr = (unsigned int) &pInput[index];
            
            void *inputAddrTranslated = (void *)TranslateAddr( inputAddr );
            pList[i].pInputBase    = inputAddrTranslated;                  // Input Base            
            pList[i].pInputIndex   = inputAddrTranslated;                  // Input Index            
        }
    }

    // Queue up the next IIR accelerator processing task
    iir_accel_queue_task( pWork );

} /* iir_accelerator_process() */


/**
 *  iir_accelerator_cancel()
 *
 *  @brief IIR accelerator cancel of further audio frame step processing and of
 *         any accelerator DMA transfer currently in progress.
 *
 *  @param[in,out] workMemory   Pointer to memory buffer allocated by model and used
 *                              by IIR Accelerator
 * 
 *  @returns Status of 0 for success and 1 for failure
 */
unsigned int iir_accelerator_cancel( float *workMemory )
{
    // Grab data objects from work memory
    iirAccelWorkMemory_t *pWork = (iirAccelWorkMemory_t *)workMemory;

    // Exit if RMDL process has already started and cleared the initialization
    // complete flag to avoid the overhead of cancelling the fast copy at each
    // step
    if ( pWork->initComplete == false )
    {
        return 0;
    }

    // Set the initialization flag to false to stop further step processing
    // on audio frame boundaries to prepare for the RMDL process
    pWork->initComplete = false;

    // Cancel any IIR accelerator task processing currently in progress
    gxpHamResult_t eResult = gxpHamIirCancelTask( pWork->hIirTaskHandle );
    assert( eResult == GXPHAM_SUCCESS );

    return (unsigned int) (eResult);

} /* iir_accelerator_cancel() */


/**
 *  create_iir_accel_task()
 *
 *  @brief Creates the IIR accelerator task from data defining model instance
 *
 *  @param[in]   params    Parameter data required to create task
 *
 */
static bool create_iir_accel_task( iirTaskParams_t *params )
{
    // Initialize IIR task ACM channel information.  Only the last channel of the task's
    // callback has interrupts enabled.
    iirAccelWorkMemory_t *pWork = params->pWork;
    gxpHamIirTcb_t *ChannelTcb = pWork->iirTaskMemory->pChannelList;
    
    // Setup variables for loop optimization
    const float* pInput = pWork->pAudioInput;
    float* pOutput = pWork->iirCircOutputDmaBuffs;
    const unsigned int* numStages = params->iirNumStages;
    const float** pCoeff = params->iirCoeffBuff;
    const int frameSize = pWork->frameSize;
    const int numChannels = pWork->numChannels;

    IirAccelSecondaryGlobalCtl_t IirSgCtlParams = 
    {
        ADI_IIR_FLOAT_ROUNDING_MODE_IEEE_ROUND_TO_NEAREST_EVEN,     // Rounding Mode
        false,                                                      // 32 bit floating point
        true                                                        // Save Biquad state
    };
    IirAccelChannelCtl_t IirChCtlParams =  
    {
        frameSize,      // Window Size
        0,              // Number of Biquads (64 max)
        0,              // Priority
        false,          // Generate Trigger on Completion 
        false,          // Wait for Trigger to Start 
        0               // Callback Enable 
    };

    uint32_t IirSgCtl = gHamCreateAccelIirSgCtlRegValue( &IirSgCtlParams );

    for( int i = 0; i < numChannels; i++ )
    {
        uint32_t IirCtl2;

        // Set channel filter number of biquads and callback enable parameters.  Callback is only
        // called on task completion. 
        IirChCtlParams.biquads = numStages[i];                              // Number of Biquads
        IirChCtlParams.callbackEnable = (i == (numChannels - 1));           // Callback enabled only
                                                                            // on last channel 
        IirCtl2 = gHamCreateAccelIirCtl2RegValue( &IirChCtlParams );

        // Note: GXPHAM links TCB blocks (pNextTCB field) together to create task 
        // Note: IIR Software control register 1 (nIirSoftwareControl1 field) is reserved 
        //       for driver usage 

        // Rounding Mode, Fixed Point Enable and Formatting 
        ChannelTcb[i].nIirSecondaryControl1 = IirSgCtl;

        // Coefficient, Output, and Input Buffers 
        ChannelTcb[i].nCoeffCount    = NUM_BIQUAD_COEFFS * numStages[i];    // Coefficient Count
        ChannelTcb[i].nCoeffModify   = 1;                                   // Coefficient Modify  
        ChannelTcb[i].pCoeffIndex    = (void *)&pCoeff[i][0];               // Coefficient Index  
        ChannelTcb[i].pOutputBase    = (void *)&pOutput[i * frameSize];     // Output Base  
        ChannelTcb[i].nOutputCount   = frameSize;                           // Output Count  
        ChannelTcb[i].nOutputModify  = 1;                                   // Output Modify  
        ChannelTcb[i].pOutputIndex   = (void *)&pOutput[i * frameSize];     // Output Index  
        ChannelTcb[i].pInputBase     = (void *)&pInput[i * frameSize];      // Input Base  
        ChannelTcb[i].nInputCount    = frameSize;                           // Input Count  
        ChannelTcb[i].nInputModify   = 1;                                   // Input Modify  
        ChannelTcb[i].pInputIndex    = (void *)&pInput[i * frameSize];      // Input Index         

        // Window Size, Tap Length, Priority, Sampling Type, Sampling Ratio, Generate Trigger, 
        // Wait for Trigger, and Callback Enable 
        ChannelTcb[i].nIirControl2 = IirCtl2;
    }

    // Create Accelerator Task 
    gxpHamIirAccelTask_t taskParams = 
    {
                                   // Pointer to task memory with unlinked TCB channel list 
        (gxpHamIirTask_t*) pWork->iirTaskMemory, 
        numChannels,               // Number TCB channels in task list 
        params->taskMemorySize,    // Overall task memory size 
        &(pWork->hIirTaskHandle)   // Location to store handle of created task 
    };

    gxpHamResult_t result = gxpHamIirCreateAccelTask( pWork->iirAccelDevId, &taskParams );
    assert( result == GXPHAM_SUCCESS );

    return (result == GXPHAM_SUCCESS);

} /* create_iir_accel_task() */


/**
 *  iir_accel_queue_task()
 *
 *  @brief Queues IIR accelerator task for execution
 *
 *  @param[in,out] pWork   Pointer to work memory buffer allocated by model and
 *                         containing data defining this model instance
 *
 */
static inline void iir_accel_queue_task( iirAccelWorkMemory_t *pWork  )
{
    gxpHamResult_t result = GXPHAM_SUCCESS;

    // Queue up the task to the IIR accelerator and start processing
    result = gxpHamIirQueueAccelTask( pWork->hIirTaskHandle );
    assert( result == GXPHAM_SUCCESS );

} /* iir_accel_queue_task() */


/**
 *  setup_variable_work_memory()
 *
 *  @brief Allocates data within work memory variable data space for accelerator 
 *
 *  @param[in,out] pWork            Pointer to work memory buffer allocated by model and
 *                                  containing data defining this model instance
 *  @param[out]    taskMemorySize   Pointer to location to store size of task memory allocated 
 *
 */
static void setup_variable_work_memory( iirAccelWorkMemory_t *pWork, unsigned int *taskMemSize )
{
    assert( pWork != NULL );
    assert( taskMemSize != NULL );

    unsigned int numCh = pWork->numChannels;
    unsigned int varMemSize = IIR_VARIABLE_DATA_SECTION_SIZE( numCh, pWork->frameSize );
    
    char *pVarBase = pWork->data;
    char *pVarMem = pVarBase;
    unsigned int size;

    // IIR driver ensures task memory size aligns to cache line boundary
    size = (sizeof(*pWork->iirTaskMemory) + numCh * sizeof(gxpHamIirTcb_t));
    VERIFY_ITEM_SIZE( (pVarMem - pVarBase + size), varMemSize );
    pWork->iirTaskMemory = (gxpHamIirTask_t*) pVarMem; 
    pVarMem += size; 
    *taskMemSize = size;

    // Make sure to account for including any padding needed to align DMA 
    // buffer size to cache line boundary
    unsigned int rawSize = (numCh * pWork->frameSize * sizeof(pWork->iirCircOutputDmaBuffs[0]));
    size = ADI_CACHE_ROUND_UP_SIZE( rawSize, char );
    VERIFY_ITEM_SIZE( (pVarMem - pVarBase + size), varMemSize );
    pWork->iirCircOutputDmaBuffs = (float*) pVarMem; 
    pVarMem += size; 

} /* setup_variable_work_memory() */
