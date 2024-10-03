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
 * @file BRF_Callback.h
 * 
 * @ingroup BRF_Callback
 *
 * @brief Implementation of the Interface BRF_Callback       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_BRF_Callback__INCLUDED_)
#define BC_BRF_Callback__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include "CART.h"

typedef enum {
  BRF_STORAGE_ID_DEFAULT
} BRF_STORAGE_ID;



/**
 * @ingroup BRF_Callback
 *
 * @brief Implements BRF thread wakeup and sending messages to BAF
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */

/**
 * @ingroup BRF_Callback
 *
 * @brief @param exec_id Integer value that should be passed to the exec() API as
 * argument.
 * 
 * @param ctx Context pointer 
 */
void BRF_Callback_post(CART_EXEC_ID exec_id, void* ctx);

/**
 * @ingroup BRF_Callback
 *
 * @brief This callback indicates to the Application that the BAF has completed the
 * execution of the BAF command(s) related to the HU/User BRF_command().
 * This callback can be used to notify the application that the BRF_command() has
 * been executed by BAF.
 * 
 * @param notify pointer to notify message to be sent to the Application
 * 
 * @param ack pointer to notify message to be sent to the Application
 */
CART_Cmd_Ret BRF_Callback_notify(CART_Message* notify);

/**
 * @ingroup BRF_Callback
 *
 * @brief This callback is invoked by BRF when it needs to send a BAF command message.
 * Use CART get_sender_id() API to get the CART_ID of the particular BAF instance,
 * that the command needs to be sent to. This callback executes from the
 * BRF_exec() context.
 * 
 * @param message Pointer to the message to be sent to BAF
 */
CART_Cmd_Ret BRF_Callback_send(CART_Message* message); 

/**
 * @ingroup BRF_Callback
 *
 * @brief This callback implements the NonVolatile media/database read.
 * It returns a pointer to the read buffer.
 * 
 * @param storage_id id to the storage media (for multiple database access)
 * 
 * @param address address/offset in bytes 
 * 
 * @param size size in bytes
 * 
 * @param buf pointer to read data
 * 
 * @param ctx  context pointer
 */
CART_Cmd_Ret BRF_Callback_read(BRF_STORAGE_ID storage_id, uint32_t address, uint32_t size, uint8_t* buf, void* ctx); 


/**
 * @ingroup BRF_Callback
 *
 * @brief This callback returns a clock to be used for logging/benchmarking.
 * Not required if benchmarking is not performed.
 */
uint32_t BRF_Callback_clock(); 

int BRF_printf(const char* fmt, ...);

#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_BRF_Callback__INCLUDED_)*/
