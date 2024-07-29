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
 * @file BAF_Callback_Func.h
 * 
 * @ingroup BAF_Callback_Func
 *
 * @brief Implementation of the Class BAF_Callback_Func       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_BAF_Callback_Func__INCLUDED_)
#define BC_BAF_Callback_Func__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


/**
 * @ingroup BAF_Callback_Func
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */

/**
 * @ingroup BAF_Callback_Func
 *
 * @brief @param exec_id
 * 
 * @param arg
 */
typedef void (*BAF_Callback_Func_Post)(uint32_t exec_id, void* arg);

typedef uint32_t (*BAF_Callback_Func_Clock)();

/**
 * @ingroup BAF_Callback_Func
 *
 * @brief @param proc_id
 * 
 * @param frame_size
 * 
 * @param num_chan_in
 * 
 * @param num_chan_out
 * 
 * @param src
 * 
 * @param dst
 * 
 * @param arg
 */
typedef void (*BAF_Callback_Func_Sys_Process_Ctx)(uint32_t proc_id, uint32_t frame_size, uint32_t num_chan_in, uint32_t num_chan_out, void* src, void* dst, void* arg);

/**
 * @ingroup BAF_Callback_Func
 *
 * @brief @param proc_id
 * 
 * @param frame_size
 * 
 * @param num_chan_in
 * 
 * @param num_chan_out
 * 
 * @param src
 * 
 * @param dst
 */
typedef void (*BAF_Callback_Func_Sys_Process)(uint32_t proc_id, uint32_t frame_size, uint32_t num_chan_in, uint32_t num_chan_out, void* src, void* dst); 

typedef uint32_t (*BAF_Callback_Func_Print)(char*, ...);


#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_BAF_Callback_Func__INCLUDED_)*/
