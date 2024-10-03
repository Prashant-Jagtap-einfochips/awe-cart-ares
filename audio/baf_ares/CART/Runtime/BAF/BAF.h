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
 * @file BAF.h
 *
 * @ingroup BAF
 *
 * @brief Implementation of the Class BAF
 *
 * @author: cb1037694
 ****************************************************/

#if !defined(BC_BAF__INCLUDED_)
#define BC_BAF__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "BAF_Version.h"
#include "BAF_Resource.h"
#include "BAF_Message.h"
#include "BAF_Func.h"
#include "BAF_Callback.h"



/**
 * @ingroup BAF
 *
 * @brief This class provides the Bose Audio Framework (BAF) abstract definition and implements the BAF APIs.
 * This class statically instantiates the current BAF object. The implementation will not be visible
 * to the Customer Integrator
 *
 * @author cb1037694
 *
 * @version 1.0
 */
typedef void  BAF;

/**
 * @ingroup BAF
 *
 * @brief Returns the resources (memory_size) required by the BAF object input parameter
 *
 * @param baf Pointer to the BAF object
 */
BAF_Resource* BAF_resource(BAF* baf);

/**
 * @ingroup BAF
 *
 * @brief Returns a pointer to the current BAF object
 *
 * @param baf Pointer to the BAF object. It should be NULL for this API call
 */
BAF* BAF_create(BAF* baf);

/**
 * @ingroup BAF
 *
 * @brief Initializes the BAF object. After ths API is called the real-time execution of the framework can
 * start. As pre-requisite, the resource structure returned by the BAF_resource() API needs to be
 * properly updated.
 *
 * @param baf Pointer to the BAF object
 */
int32_t BAF_init(BAF* baf);

/**
 * @ingroup BAF
 *
 * @brief Executes the framework thread of the BAF object associated with the thread_level. This API is
 * usually invoked periodically at a rate specific to the thread_level and the current BAF object
 * specifications.
 *
 * @param baf Pointer to the BAF object
 *
 * @param thread_level Thread level to be executed
 *
 * @param arg context pointer passed to BAF_Callback_sys_process_ctx() and BAF_Callback_post()
 * 
 * @return zero if no overrun occured, overrun bitmap otherwise
 */
uint32_t BAF_exec(BAF* baf, uint32_t thread_level, void* arg);

/**
 * @ingroup BAF
 *
 * @brief API for sending commands to the BAF object
 *
 * @param baf Pointer to the BAF object
 *
 * @param req Request message
 *
 * @param rsp Response Message
 * 
 * @return rsp length if positive, zero if no response, negative in case of error
 */
int32_t BAF_command(BAF* baf, BAF_Message* req, BAF_Message* rsp);

/**
 * @ingroup BAF
 *
 * @brief API for registering notification callback with the BAF object.
 *
 * @param baf Pointer to the BAF object
 *
 * @param notify Notification callbak to be registered
 */
void BAF_register_notify(BAF* baf, BAF_Func_Notify notify);

/**
 * @ingroup BAF
 *
 * @brief Invokes OpenJADE scheduler
 *
 * @param baf Pointer to the BAF object
 *
 * @return zero if no overrun occured, overrun bitmap otherwise
 */
uint32_t BAF_schedule(BAF* baf, void* arg);

/**
 * @ingroup BAF
 *
 * @brief Returns BAF Runtime version as string "nn.nn.mm"
 *    "n" -- are number characters in the range 0 to 9
 *    "m" -- are number characters in the range 0 to 9 for release drops
 *    "m" -- are alphabet characters in the range A to Z for alpha drops
 *
 * @param baf Pointer to the BAF object. It should be NULL for this API call
 */
const char *BAF_version(BAF* baf);

/**
 * @ingroup BAF_Callback_Func
 *
 * @brief  Post callback typedef
 * 
 * @param exec_id
 * 
 * @param arg
 */
typedef void (*BAF_Callback_Func_Post)(uint32_t exec_id, void* arg);

/**
 * @ingroup BAF_Callback_Func
 *
 * @brief Sys callback prototype
 * 
 * @param proc_id
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
 * @ingroup BAF_Register
 *
 * @brief Post callback registering prototype
 *
 * @param baf Pointer to the BAF object
 *
 * @param funcptr Post callback to be registered
 */
void BAF_Register_post(BAF* baf, BAF_Callback_Func_Post funcptr);

/**
 * @ingroup BAF_Register
 *
 * @brief SYS process callback registering prototype
 *
 * @param baf Pointer to the BAF object
 *
 * @param funcptr sys callback to be registered
 */
void BAF_Register_sys_process_ctx(BAF* baf, BAF_Callback_Func_Sys_Process_Ctx funcptr);



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_BAF__INCLUDED_)*/
