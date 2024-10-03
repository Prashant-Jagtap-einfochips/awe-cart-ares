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
 * @file BTF_Callback.h
 * 
 * @brief Implementation of the Class BTF_Callback       
 *
 * @author: cb1037694                     
 ****************************************************/

#include "BTF_Callback.h"
#include "BTF_Callback_Register.h"

extern BTF_Callback_Register BTF_Callback_register;
 
/**
 * Framework notification for invoking/posting the thread with priority id
 * specified as parameter.
 * This API is called by the BCF library and needs to be implemented by the
 * library user.
 * The API executes in the context of the BTF_exec(0,�)
 * BTF_exec(0,�) needs to be executed at the base frame rate.
 * In case there are multiple execution threads in the BCF library, the BTF_exec(0,
 * �) runs a monotonic rate scheduler and determines if other threads need to be
 * unblocked. When the thread with id <b>n</b> needs to be unblocked, then the
 * BTF_library calls BTF_Callback_post(<b>n</b>,�)
 * The argument void* arg is reserved for future use.
 * There are two possible implementations of BTF_Callback_post(n,�):
 *  - In a single threaded platform, or functional testing the BTF_Callback_post
 * calls BTF_exec(priority, NULL)
 *  - In a multithreaded environment the BTF_Callback_post shoud unlock a thread
 * that executes BTF_exec(priority, NULL), by posting a platform specific
 * semaphore.
 */
void BTF_Callback_post(CART_EXEC_ID exec_id, void* arg)
{
	if(BTF_Callback_register.post != NULL) {
		BTF_Callback_register.post(exec_id, arg);
	}
	else {
		//printf("BTF_Callback_post not registered");
	}
}

CART_Cmd_Ret BTF_Callback_send(CART_Message* req)
{
	CART_Cmd_Ret ret = {CART_STATUS_NO_ACTION};
	if(BTF_Callback_register.send != NULL) {
		ret = BTF_Callback_register.send(req);
	}
	else {
		//printf("BTF_Callback_post not registered");
	}
	return ret;
}

CART_Cmd_Ret BTF_Callback_send_ctx(CART_Message* req, void* ctx)
{
	CART_Cmd_Ret ret = {CART_STATUS_NO_ACTION};
	if(BTF_Callback_register.send != NULL) {
		ret = BTF_Callback_register.send(req);
	}
	else {
		//printf("BTF_Callback_post not registered");
	}
	return ret;
}

CART_Cmd_Ret BTF_Callback_notify(CART_Message* notify)
{
	CART_Cmd_Ret ret = {CART_STATUS_NO_ACTION};
	if(BTF_Callback_register.notify != NULL) {
		ret = BTF_Callback_register.notify(notify);
	}
	else {
		//printf("BTF_Callback_post not registered");
	}
	return ret;
}

/**
 * API that returns a real-time clock counter, required for profiling.
 * This API is called by the BCF library and needs to be implemented by the
 * library user.
 * The BCF library has an internal logging system and uses this callback to
 * timestamp these messages.
 * The logging system tracks the execution of different modules inside BCF library,
 * and if the BTF_Callback_clock() is implemented, the logs can be used for MIPS
 * measurements
 * Ideally this API should return a CPU clock counter, if available. In case of
 * SHARC processors this api can return <b>(uint32_t)__builtin_emuclk()</b>. If
 * the CPU clock counter is not available, then this API can return 0, but MIPS
 * measurements will not be available.
 */
uint32_t BTF_Callback_clock()
{
	if(BTF_Callback_register.clock != NULL) {
		return BTF_Callback_register.clock();
	}
	else {
		return 0xdeadc10c;
		//printf("BCF_Callback_clock not registered");
	}
}

#include <stdio.h>
#include <stdarg.h>
char BTF_printf_str[1024];
int BTF_printf(const char* fmt, ...)
{
   	va_list args;
   	int val;
	va_start( args, fmt );
	if(BTF_Callback_register.print != NULL) {
		vsprintf(BTF_printf_str, fmt, args);
		val = BTF_Callback_register.print(BTF_printf_str);
	}
	else {
		val = vprintf(fmt, args );
    }
	va_end(args);
	return val;
}
