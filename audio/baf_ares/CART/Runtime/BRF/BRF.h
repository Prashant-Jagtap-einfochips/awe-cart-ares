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
 * @file BRF.h
 * 
 * @ingroup BRF
 *
 * @brief Implementation of the Class BRF       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_BRF__INCLUDED_)
#define BC_BRF__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include "BRF_Resource.h"
#include "CART_Cmd_Ret.h"
#include "CART.h"


/**
 * @ingroup BRF
 *
 * @brief 1. Room mode command access
 * 2. NonVolatile media/database access
 * 3. Database parsing
 * 4. Database records to BCF commands translation and serialization(ex. TOP)
 * 5. Sending serialized data to BAMF
 * 6. Send appropriate BAF commands to prepare/finalize database transfer to BAF(ex. mute/unmutedeactivate/activate)
 * 7. Send completion status
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */
typedef void BRF;

typedef uint8_t BRF_RET_CODE_TYPE;

#define BRF_VERSION "00.00.AA"

/**
 * @ingroup BRF
 *
 * @brief This API returns a pointer to the BRF object corresponding to the CART_ID
 * provided as argument.
 * 
 * @param brf_id CART_ID of the BRF module
 */
BRF* BRF_create(CART_ID brf_id);

/**
 * @ingroup BRF
 *
 * @brief Returns the resources required by the BRF object
 *
 * @param brf Pointer to the BRF object
 */
BRF_Resource* BRF_resource(BRF* btf);

/**
 * @ingroup BTF
 *
 * @brief This API initializes the BRF module
 * 
 * @param brf Pointer to the BRF object
 * 
 * @param arg context pointer to be supplied
 */
CART_STATUS BRF_init(BRF* brf, void* arg);

/**
 * @ingroup BRF
 *
 * @brief This API is used to send tune commands to BRF (ex. BDP. BCP)
 * 
 * @param brf Pointer to the BRF object
 * 
 * @param req Pointer to the request message
 * 
 * @param res Pointer to the response message
 */
CART_Cmd_Ret BRF_command(BRF* brf, CART_Message* req, CART_Message* res);

/**
 * @ingroup BRF
 *
 * @brief This API is invoked by the BRF_receive() dispatcher to send BAF responses to
 * BRF (ex. TOP, TSP reads)
 * 
 * @param brf Pointer to the BRF object
 * 
 * @param res
 */
CART_Cmd_Ret BRF_receive(BRF* brf, CART_Message* res);

/**
 * @ingroup BRF
 *
 * @brief This API translates Tune commands to BAF command message(s), serializes and
 * sends the message to BAF
 * 
 * @param brf Pointer to the BRF object
 * 
 * @param exec_id Integer value indicatinc the thread execution function to be
 * executed. Set to CART_EXEC_ID_DEFAULT if post() callback is not used
 * 
 * @param ctx Context pointer
 */
uint32_t BRF_exec(BRF* brf, CART_EXEC_ID exec_id, void* ctx); 



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_BRF__INCLUDED_)*/
