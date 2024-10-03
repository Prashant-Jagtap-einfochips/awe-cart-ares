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
 * @file BCF_Callback.h
 * 
 * @ingroup BCF_Callback
 *
 * @brief Implementation of the Class BCF_Callback       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_BCF_Callback__INCLUDED_)
#define BC_BCF_Callback__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include "CART.h"


/**
 * @ingroup BCF_Callback
 *
 * @brief Defines BCF callbacks for BCF_exec() thread wakeup, sending messages to other
 * CART components, and sending command() notifications back to the application
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */

/**
 * @ingroup BCF_Callback
 *
 * @brief This callback is invoked when BCF is scheduling the execution of the thread, or
 * other scheduling item, that will call BCF_exec().
 * The thread calling BAF_exec() can be implemented as a periodic thread, in which
 * case this callback can be ignored (empty).
 * This callback executes from the BCF_command() context.
 * 
 * @param exec_id Integer value that should be passed to the exec() API as
 * argument.
 * 
 * @param ctx Context pointer 
 */
void BCF_Callback_post(CART_EXEC_ID exec_id, void* ctx);

/**
 * @ingroup BCF_Callback
 *
 * @brief This callback indicates to the Application that the BAF has completed the
 * execution of the BAF command(s) related to the HU/User BCF_command().
 * This callback can be used to notify the application that the BCF_command() has
 * been executed by BAF.
 * 
 * @param notify pointer to notify message to be sent to the Application
 * 
 * @param ack pointer to notify message to be sent to the Application
 */
CART_Cmd_Ret BCF_Callback_notify(CART_Message* notify);

/**
 * @ingroup BCF_Callback
 *
 * @brief This callback is invoked by BCF when it needs to send a BAF command message.
 * Use CART get_sender_id() API to get the CART_ID of the particular BAF instance,
 * that the command needs to be sent to. This callback executes from the
 * BCF_exec() context.
 * 
 * @param message Pointer to the message to be sent to BAF
 */
CART_Cmd_Ret BCF_Callback_send(CART_Message* message); 

/**
 * @ingroup BCF_Callback_send_ctx
 *
 * @brief This callback is invoked by BCF when it needs to send a BAF command message.
 * Use CART get_sender_id() API to get the CART_ID of the particular BAF instance,
 * that the command needs to be sent to. This callback executes from the
 * BCF_exec() context. Use ctx input pointer to pass environment context.
 * 
 * @param message Pointer to the message to be sent to BAF
 */
CART_Cmd_Ret BCF_Callback_send_ctx(CART_Message* message, void * ctx); 


uint32_t BCF_Callback_clock(); 

int BCF_printf(const char* fmt, ...);


#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_BCF_Callback__INCLUDED_)*/
