/*******************************************************************************
*  BOSE CORPORATION
*  COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
*  This program may not be reproduced, in whole or in part in any
*  form or any means whatsoever without the written permission of:
*     BOSE CORPORATION
*     The Mountain
*     Framingham, MA 01701-9168
*******************************************************************************/
/****************************************************
 * @file BAF_Callback.h
 *
 * @ingroup BAF_Callback
 *
 * @brief Implementation of the Interface BAF_Callback
 *
 * @author: cb1037694
 ****************************************************/

#if !defined(BC_BAF_Callback__INCLUDED_)
#define BC_BAF_Callback__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include "BAF_Message.h"


/**
 * @ingroup BAF_Callback
 *
 * @brief Callback function prototypes
 *
 * @author cb1037694
 *
 * @version 1.0
 */

/**
 * @ingroup BAF_Callback
 *
 * @brief Framework notification for invoking/posting the thread with priority id specified as parameter.
 * This API is called by the BAF library and needs to be implemented by the library user.
 * The API executes in the context of the BAF_exec(0,�)
 * BAF_exec(0,�) needs to be executed at the base frame rate.
 * In case there are multiple execution threads in the BAF library, the BAF_exec(0,�) runs a monotonic
 * rate scheduler and determines if other threads need to be unblocked. When the thread with id
 * <b>n</b> needs to be unblocked, then the BAF_library calls BAF_Callback_post(<b>n</b>,�)
 * The argument void* arg is reserved for future use.
 * There are two possible implementations of BAF_Callback_post(n,�):
 *  - In a single threaded platform, or functional testing the BAF_Callback_post calls
 * BAF_exec(priority, NULL)
 *  - In a multithreaded environment the BAF_Callback_post shoud unlock a thread that executes
 * BAF_exec(priority, NULL), by posting a platform specific semaphore.
 *
 * @param priority Priority or thread number to be unblocked
 *
 * @param arg
 */
void BAF_Callback_post(uint32_t priority, void* arg);
void BAF_Callback_post_mask(uint32_t exec_mask, void* arg);

/**
 * @ingroup BAF_Callback
 *
 * @brief API that returns a real-time clock counter, required for profiling.
 * This API is called by the BAF library and needs to be implemented by the library user.
 * The BAF library has an internal logging system, and uses this callback to timestamp these messages.
 *
 * The logging system tracks the execution of different modules inside BAF library, and if the
 * BAF_Callback_clock() is implemented, the logs can be used for MIPS measurements
 * Ideally this API should return a CPU clock counter, if available. In case of SHARC processors this
 * api can return <b>(uint32_t)__builtin_emuclk()</b>. If the CPU clock counter is not available, then
 * this API can return 0, but MIPS measurements will not be available.
 */
uint32_t BAF_Callback_clock();

/**
 * @ingroup BAF_Callback
 *
 * @param log pointer to log memory
 *
 * @param len available space in terms of uint32_t datum
 *
 * @param start_stop 0 for start, 1 for stop
 *
 * @return Count of uint32_t datum written
 *
 * This API is called by the BAF library and needs to be implemented by the library user.
 * The BAF library has an internal logging system which uses this callback to get
 * the array of uint32_t values and add them to the supplied log pointed by *stats.
 *
 */
uint32_t BAF_Callback_Stats(uint32_t *log, uint32_t len, uint32_t start_stop);

/**
 * @ingroup BAF_Callback
 *
 * @brief System I/O callback with context prototype
 * This API behaves the same as BAF_Callback_sys_process, except that this one passes the context
 * provided as the second argument of the BAF_exec call. For example, this API can be used to pass the
 * I/O pointers to this API through a BAF_exec call.
 *
 * @param proc_Id rocess ID. It is used to identify the I/O operation type, like A/D or D/A interface
 *
 * @param frame_size Number of samples per channel in a frame
 *
 * @param num_chan_in Number of input channels
 *
 * @param num_chan_out Number of output channels
 *
 * @param src Pointer to audio/data samples to be sent to the output device (like DAC, or other core)
 *
 * @param dst Pointer to audio/data samples to be received from the input device (like ADC or other
 * core)
 *
 * @param arg
 */
void BAF_Callback_sys_process_ctx(uint32_t proc_Id, uint32_t frame_size, uint32_t num_chan_in, uint32_t num_chan_out, void* src, void* dst, void* arg);

/**
 * @ingroup BAF_Callback
 *
 * @brief System I/O callback prototype
 * This API is called by the BAF library and implemented by the BAF library user.
 * BAF has allocates the input and output buffers. Pointers to these buffers are provided by the
 * through this API arguments.
 * Usually, this API gets called by BAF twice: first to request the input buffer, and second to
 * provide output buffer.
 * The proc_Id argument it is used to identify if BAF request or provides a buffer.
 * For the first API call (proc_Id == 0), the BAF user will have to transfer the A/D input samples
 * buffer (SPORT buffer, for example) to the buffer pointed by the dst argument.  The size of the
 * transfer is num_chan_out * frame_size samples. At this point, samples deinterleaving, channel
 * routing, data type conversion can be performed in order to match the required BAF input buffer
 * formatting.
 * Similarily, for the second call (proc_Id == 1), the buffer pointed by the src argument, will have
 * to be transferred to the D/A samples buffer. The size of the transfer is num_chan_in * frame_size
 * samples. Again, interleaving, routing, type conversions can be performed at this stage.
 * There can be cases when this API can be called once, or more than twice, like signal a generator
 * application, or when operating on samples at different sampling rate.
 *
 * @param proc_Id Process ID. It is used to identify the I/O operation type, like A/D or D/A interface
 *
 * @param frame_size Number of samples per channel in a frame
 *
 * @param num_chan_in Number of input channels
 *
 * @param num_chan_out Number of output channels
 *
 * @param src Pointer to audio/data samples to be sent to the output device (like DAC, or other core)
 *
 * @param dst Pointer to audio/data samples to be received from the input device (like ADC or other
 * core)
 */
void BAF_Callback_sys_process(uint32_t proc_Id, uint32_t frame_size, uint32_t num_chan_in, uint32_t num_chan_out, void* src, void* dst);

typedef enum
{
    BAF_CALLBACK_SYS_PROCESS_SYS_IN = 0,
    BAF_CALLBACK_SYS_PROCESS_SYS_OUT = 1,
    BAF_CALLBACK_SYS_PROCESS_SYS_IN0 = 0,
    BAF_CALLBACK_SYS_PROCESS_SYS_OUT0 = 1,
    BAF_CALLBACK_SYS_PROCESS_SYS_IN1 = 2,
    BAF_CALLBACK_SYS_PROCESS_SYS_OUT1 = 3,
    BAF_CALLBACK_SYS_PROCESS_SYS_IN2 = 4,
    BAF_CALLBACK_SYS_PROCESS_SYS_OUT2 = 5,
    BAF_CALLBACK_SYS_PROCESS_SYS_IN3 = 6,
    BAF_CALLBACK_SYS_PROCESS_SYS_OUT3 = 7
} BAF_CALLBACK_SYS_PROCESS;



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_BAF_Callback__INCLUDED_)*/
