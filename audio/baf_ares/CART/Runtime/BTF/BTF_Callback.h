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
 * @ingroup BTF_Callback
 *
 * @brief Implementation of the Interface BTF_Callback       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_BTF_Callback__INCLUDED_)
#define BC_BTF_Callback__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include "CART.h"


/**
 * @ingroup BTF_Callback
 *
 * @brief Implements BTF thread wakeup and sending messages to BAF
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */

/**
 * @ingroup BTF_Callback
 *
 * @brief @param exec_id Integer value that should be passed to the exec() API as
 * argument.
 * 
 * @param ctx Context pointer 
 */
void BTF_Callback_post(CART_EXEC_ID exec_id, void* ctx);

/**
 * @ingroup BTF_Callback
 *
 * @brief This callback indicates to the Application that the BAF has completed the
 * execution of the BAF command(s) related to the HU/User BTF_command().
 * This callback can be used to notify the application that the BTF_command() has
 * been executed by BAF.
 * 
 * @param notify pointer to notify message to be sent to the Application
 * 
 * @param ack pointer to notify message to be sent to the Application
 */
CART_Cmd_Ret BTF_Callback_notify(CART_Message* notify);

/**
 * @ingroup BTF_Callback
 *
 * @brief This callback is invoked by BTF when it needs to send a BAF command message.
 * Use CART get_sender_id() API to get the CART_ID of the particular BAF instance,
 * that the command needs to be sent to. This callback executes from the
 * BTF_exec() context.
 * 
 * @param message Pointer to the message to be sent to BAF
 */
CART_Cmd_Ret BTF_Callback_send(CART_Message* message); 

/**
 * @ingroup BTF_Callback
 *
 * @brief This callback is invoked by BTF when it needs to send a BAF command message.
 * Use CART get_sender_id() API to get the CART_ID of the particular BAF instance,
 * that the command needs to be sent to. This callback executes from the
 * BTF_exec() context. Use ctx input pointer to pass environment context.
 * 
 * @param message Pointer to the message to be sent to BAF
 */
CART_Cmd_Ret BTF_Callback_send_ctx(CART_Message* message, void* ctx); 

uint32_t BTF_Callback_clock(); 

int BTF_printf(const char* fmt, ...);

#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_BTF_Callback__INCLUDED_)*/
