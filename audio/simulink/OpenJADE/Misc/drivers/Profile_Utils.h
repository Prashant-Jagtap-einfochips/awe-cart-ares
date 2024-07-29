/*!
 * @file      Profile_Utils.h
 * @brief     Provides utilities to capture timing for profiling GXP accelerators
 */
#ifndef __PROFILE_UTILS_H__
#define __PROFILE_UTILS_H__

#include <sys/platform.h>
#include <builtins.h>
#include <stdint.h>

#include "Logger.h"

/**
 * @brief Registers and assigns timer to FIR accelerator task.
 *
 * @param [in]  device = accelerator device ID
 * @param [in]  task = accelerator task ID
 * @param [in]  pTask = pointer to accelerator task TCB list
 *
 * @return None
 *
 */
static inline void RegisterFirAccelTask( uint8_t device, uint8_t task, void *pTask )
{
   uint32_t thread_level = JTEST_FIR_ACCEL_THREAD_LEVEL+device;
   Logger_print(LOG_TYPE_PRINT, thread_level, "Id:%d t:%10u\n", task, (uint32_t)pTask);
}

/**
 * @brief Registers and assigns timer to IIR accelerator task.
 *
 * @param [in]  device = accelerator device ID
 * @param [in]  task = accelerator task ID
 * @param [in]  pTask = pointer to accelerator task TCB list
 *
 * @return None
 *
 */
static inline void RegisterIirAccelTask( uint8_t device, uint8_t task, void *pTask )
{
   uint32_t thread_level = JTEST_IIR_ACCEL_THREAD_LEVEL+device;
   Logger_print(LOG_TYPE_PRINT, thread_level, "Id:%d t:%10u\n", task, (uint32_t)pTask);
}

/**
 * @brief Starts FIR timer at start of event timing.
 *
 * @param [in]  device = accelerator device ID
 * @param [in]  task = accelerator task ID
 *
 * @return None
 *
 */
static inline void StartFirTimer( uint8_t device, uint8_t task )
{
   uint32_t thread_level = JTEST_FIR_ACCEL_THREAD_LEVEL+device;
   uint32_t emuclk = __builtin_emuclk();
   Logger_print(LOG_TYPE_EXEC, thread_level, "Bgn:%d t:%10u\n", task, emuclk);
}

/**
 * @brief Starts IIR timer at start of event timing.
 *
 * @param [in]  device = accelerator device ID
 * @param [in]  task = accelerator task ID
 *
 * @return None
 *
 */
static inline void StartIirTimer( uint8_t device, uint8_t task )
{
   uint32_t thread_level = JTEST_IIR_ACCEL_THREAD_LEVEL+device;
   uint32_t emuclk = __builtin_emuclk();
   Logger_print(LOG_TYPE_EXEC, thread_level, "Bgn:%d t:%10u\n", task, emuclk);
}

/**
 * @brief Stop FIR accelerator timer at end of event timing.
 *
 * @param [in]  device = accelerator device ID
 * @param [in]  task = accelerator task ID
 *
 * @return None
 *
 */
static inline void StopFirTimer( uint8_t device, uint8_t task )
{
   uint32_t thread_level = JTEST_FIR_ACCEL_THREAD_LEVEL+device;
   uint32_t emuclk = __builtin_emuclk();
   Logger_print(LOG_TYPE_EXEC, thread_level, "End:%d t:%10u\n", task, emuclk);
}

/**
 * @brief Stop IIR accelerator timer at end of event timing.
 *
 * @param [in]  device = accelerator device ID
 * @param [in]  task = accelerator task ID
 *
 * @return None
 *
 */
static inline void StopIirTimer( uint8_t device, uint8_t task )
{
   uint32_t thread_level = JTEST_IIR_ACCEL_THREAD_LEVEL+device;
   uint32_t emuclk = __builtin_emuclk();
   Logger_print(LOG_TYPE_EXEC, thread_level, "End:%d t:%10u\n", task, emuclk);
}

#endif /*__PROFILE_UTILS_H__*/

