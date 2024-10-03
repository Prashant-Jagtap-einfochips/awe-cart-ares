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
 * @file BCF.h
 * 
 * @ingroup BCF
 *
 * @brief Implementation of the Class BCF       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_BCF__INCLUDED_)
#define BC_BCF__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include "BCF_Resource.h"
#include "CART_Cmd_Ret.h"
#include "CART.h"


/**
 * @ingroup BCF
 *
 * @brief This software module is responsible for:
 * 1. HU/User command access
 * 2. HU/User command parsing
 * 3. HU/User command translation to BAF command messages (ex. RTC)
 * 4. BAF command messages serialization
 * 5. Sending the serialized data to BAF
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */
typedef void BCF;

//typedef uint8_t BCF_RET_CODE_TYPE;

#define BCF_VERSION "00.00.AA"

/**
 * @ingroup BCF
 *
 * @brief Returns pointer to the BCF object associated with the CART_ID passed as
 * argument
 * 
 * @param bcf_id CART_ID of the BCF module
 */
BCF* BCF_create(CART_ID bcf_id);

/**
 * @ingroup BCF
 *
 * @brief Returns the resources required by the BCF object 
 *
 * @param btf Pointer to the BCF object
 */
BCF_Resource* BCF_resource(BCF* bcf);

/**
 * @ingroup BTF
 *
 * @brief This API initializes the BCF module.
 * Prerequisite: BCF_create()
 * 
 * @param bcf Pointer to the BCF object to be initialized.
 * 
 * @param ctx Pointer to application context.
 */
CART_STATUS BCF_init(BCF* bcf, void* ctx);

/**
 * @ingroup BCF
 *
 * @brief Usually HU command.
 * This API return success if command is valid. If valid the command will be
 * translated and sent to BAF, if required..
 * 
 * @param bcf Pointer to the BCF object
 * 
 * @param req Pointer to the request message
 * 
 * @param rsp Pointer to the response message
 */
CART_Cmd_Ret BCF_command(BCF* bcf, CART_Message* req, CART_Message* rsp);

/**
 * @ingroup BCF
 *
 * @brief This API handles the BAF response messages.
 * Based on the response message, BCF may invoke the BCF_Callback_notify()
 * 
 * @param bcf Pointer to the BCF object
 * 
 * @param response Pointer to the response message
 */
CART_Cmd_Ret BCF_receive(BCF* bcf, CART_Message* response);

/**
 * @ingroup BCF
 *
 * @brief This API translates HU/User command to BAF command() message(s), serializes and
 * sends the message to BAF.
 * In general this API is called on a different thread than the one that calls
 * BAF_command(), but it is not required.
 * This API can be invoked periodically, or it can be triggered by
 * BAF_Callback_post()
 * If BAF_Callback_post() calls BAF_exec(), then BAF_exec() will execute in the
 * same thread as BAF_command()
 * 
 * @param bcf Pointer to the BCF object
 * 
 * @param exec_id Integer value indicatinc the thread execution function to be
 * executed. Set to CART_EXEC_ID_DEFAULT if post() callback is not used.
 * 
 * @param ctx
 */
uint32_t BCF_exec(BCF* bcf, CART_EXEC_ID exec_id, void* ctx); 


#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_BCF__INCLUDED_)*/
