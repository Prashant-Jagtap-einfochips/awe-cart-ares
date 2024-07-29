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
 * @brief Implementation of the Class BAF_Callback       
 *
 * @author: cb1037694                     
 ****************************************************/

#include "BAF_Callback.h"
#include "BAF_Callback_Register.h"

extern BAF_Callback_Register BAF_Callback_register;
 
/**
 * Framework notification for invoking/posting the thread with priority id
 * specified as parameter.
 * This API is called by the BAF library and needs to be implemented by the
 * library user.
 * The API executes in the context of the BAF_exec(0,�)
 * BAF_exec(0,�) needs to be executed at the base frame rate.
 * In case there are multiple execution threads in the BAF library, the BAF_exec(0,
 * �) runs a monotonic rate scheduler and determines if other threads need to be
 * unblocked. When the thread with id <b>n</b> needs to be unblocked, then the
 * BAF_library calls BAF_Callback_post(<b>n</b>,�)
 * The argument void* arg is reserved for future use.
 * There are two possible implementations of BAF_Callback_post(n,�):
 *  - In a single threaded platform, or functional testing the BAF_Callback_post
 * calls BAF_exec(priority, NULL)
 *  - In a multithreaded environment the BAF_Callback_post shoud unlock a thread
 * that executes BAF_exec(priority, NULL), by posting a platform specific
 * semaphore.
 */
void BAF_Callback_post(uint32_t exec_id, void* arg)
{
	if(BAF_Callback_register.post != NULL) {
		BAF_Callback_register.post(exec_id, arg);
	}
	else {
		//printf("BAF_Callback_post not registered");
	}
}

/**
 * API that returns a real-time clock counter, required for profiling.
 * This API is called by the BAF library and needs to be implemented by the
 * library user.
 * The BAF library has an internal logging system and uses this callback to
 * timestamp these messages.
 * The logging system tracks the execution of different modules inside BAF library,
 * and if the BAF_Callback_clock() is implemented, the logs can be used for MIPS
 * measurements
 * Ideally this API should return a CPU clock counter, if available. In case of
 * SHARC processors this api can return <b>(uint32_t)__builtin_emuclk()</b>. If
 * the CPU clock counter is not available, then this API can return 0, but MIPS
 * measurements will not be available.
 */
uint32_t BAF_Callback_clock()
{
	if(BAF_Callback_register.clock != NULL) {
		return BAF_Callback_register.clock();
	}
	else {
		return 0xdeadc;
		//printf("BAF_Callback_clock not registered");
	}
}

/**
 * System I/O callback with context prototype
 * This API behaves the same as BAF_Callback_sys_process, except that this API
 * passes the context provided as second argument to the BAF_exec call. For
 * example, this API can be used to pass the I/O pointers to this API through a
 * BAF_exec call.
 */
void BAF_Callback_sys_process_ctx(uint32_t proc_id, uint32_t frame_size, uint32_t num_chan_in, uint32_t num_chan_out, void* src, void* dst, void* arg)
{
	if(BAF_Callback_register.sys_process_ctx != NULL) {
		BAF_Callback_register.sys_process_ctx(proc_id, frame_size, num_chan_in, num_chan_out, src, dst, arg);
	}
	else {
		//printf("BAF_Callback_sys_process not registered");
	}
}

/**
 * System I/O callback prototype.
 * This API is called by the BAF library and implemented by the BAF library user.
 * BAF has allocated the input and output buffers. Pointers to these buffers are
 * provided by the through this API arguments.
 * Usually, this API gets called by BAF twice: first to request the input buffer,
 * and second to provide output buffer.
 * The proc_Id argument it is used to identify if BAF requests or provides a
 * buffer.
 * For the first API call (proc_Id == 0), the BAF user will have to transfer the
 * A/D input samples buffer (SPORT buffer, for example) to the buffer pointed by
 * the dst argument.  The size of the transfer is num_chan_out * frame_size
 * samples. At this point, samples deinterleaving, channel routing, data type
 * conversion can be performed in order to match the required BAF input buffer
 * formatting.
 * Similarily, for the second call (proc_Id == 1), the buffer pointed by the src
 * argument, will have to be transferred to the D/A samples buffer. The size of
 * the transfer is num_chan_in * frame_size samples. Again, interleaving, routing,
 * type conversions can be performed at this stage.
 * There can be cases when this API can be called once, or more than twice, like
 * signal a generator application, or when operating on samples at different
 * sampling rate.
 */
void BAF_Callback_sys_process(uint32_t proc_id, uint32_t frame_size, uint32_t num_chan_in, uint32_t num_chan_out, void* src, void* dst)
{
	if(BAF_Callback_register.sys_process != NULL) {
		BAF_Callback_register.sys_process(proc_id, frame_size, num_chan_in, num_chan_out, src, dst);
	}
	else {
		//printf("BAF_Callback_sys_process not registered");
	}
} 

#include <stdio.h>
#include <stdarg.h>
char BAF_printf_str[256];
int BAF_printf(char* fmt, ...)
{
   	va_list args;
   	int val;
	va_start( args, fmt );
	if(BAF_Callback_register.print != NULL) {
		vsprintf(BAF_printf_str, fmt, args);
		val = BAF_Callback_register.print(BAF_printf_str);
	}
	else {
		val = vprintf(fmt, args );
    }
	va_end(args);
	return val;
}
