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
 * @file JFramework.h
 * 
 * @ingroup JFramework
 *
 * @brief Implementation of the Class JFramework       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_JFramework__INCLUDED_)
#define BC_JFramework__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include "Framework_Param.h"
#include "Message.h"
#include "Message_Id.h"
#include "BAF_Callback.h"
#include "BAF_Func.h"
#include "Logger.h"
#include "Callback_Register.h"
#include "Execution.h"
#include "Connection.h"
#include "Buffer.h"
#include "Module.h"
#include "Logger.h"


/**
 * @ingroup JFramework
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef struct JFramework
{
	uint32_t size_module;
	uint32_t num_module;
	uint32_t size_thread;
	uint32_t num_thread;
	uint32_t call_interval[FW_NUM_THREAD];
	uint32_t thread_counter[FW_NUM_THREAD];
	uint32_t size_connection;
	uint32_t num_connection;
	uint32_t size_buffer;
	uint32_t num_buffer;
	Module* module;
	Execution* execution[FW_NUM_THREAD];
	Connection* connection;
	Buffer* buffer;
	BAF_Func_Notify notify;
	BAF_Callback_Register callback_register;

	/**
	 * @ingroup JFramework
	 *
	 * @brief The id of the default JModule for receiving single pass thru commands
	 */
	Module_Id default_module;
	uint8_t num_exec[FW_NUM_THREAD];
	uint8_t size_exec[FW_NUM_THREAD];
	uint8_t thread_state[FW_NUM_THREAD];
} JFramework;

/**
 * @ingroup JFramework
 *
 * @brief @param this
 * 
 * @param thread_level
 * 
 * @param arg
 */
void JFramework_thread_exec(JFramework* this, uint32_t thread_level, void* arg);

/**
 * @ingroup JFramework
 *
 * @brief @param this
 */
void JFramework_init(JFramework* this);

/**
 * @ingroup JFramework
 *
 * @brief @param this
 * 
 * @param buffer_id
 * 
 * @param payload
 */
int32_t JFramework_add_buffer(JFramework* this, Buffer_Id buffer_id, void* payload);

/**
 * @ingroup JFramework
 *
 * @brief @param this
 * 
 * @param module_id
 * 
 * @param feature
 * 
 * @param context
 */
int32_t JFramework_add_module(JFramework* this, Module_Id module_id, JModule* feature, JModule_Context* context);

/**
 * @ingroup JFramework
 *
 * @brief @param this
 * 
 * @param process
 * 
 * @param process_context
 * 
 * @param state
 * 
 * @param thread_level
 */
Error_Id JFramework_add_execution(JFramework* this, Process* process, Process_Context* process_context, void* state, uint32_t thread_level);

/**
 * @ingroup JFramework
 *
 * @brief @param this
 * 
 * @param buffer_id
 */
void* JFramework_get_buffer(JFramework* this, Buffer_Id buffer_id);

/**
 * @ingroup JFramework
 *
 * @brief @param this
 * 
 * @param req
 * 
 * @param res
 */
int32_t JFramework_command(JFramework* this, Message* req, Message* res);

/**
 * @ingroup JFramework
 *
 * @brief @param this
 * 
 * @param func
 */
void JFramework_register_notify(JFramework* this, BAF_Func_Notify func);

/**
 * @ingroup JFramework
 *
 * @brief @param this
 * 
 * @param msg
 */
void JFramework_notify(JFramework* this, Message* msg);

/**
 * @ingroup JFramework
 *
 * @brief @param this
 */
void JFramework_init_exec(JFramework* this);

/**
 * @ingroup JFramework
 *
 * @brief @param this
 * 
 * @param module_id
 */
Module* JFramework_get_module(JFramework* this, Module_Id module_id); 



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_JFramework__INCLUDED_)*/
