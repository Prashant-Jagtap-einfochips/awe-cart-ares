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
 * @file BTF.h
 * 
 * @ingroup BTF
 *
 * @brief Implementation of the Class BTF       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_BTF__INCLUDED_)
#define BC_BTF__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include "BTF_Resource.h"
#include "CART_Cmd_Ret.h"
#include "CART.h"


/**
 * @ingroup BTF
 *
 * @brief 1. Bose tuning tool communication
 * 2 .Customer tuning tool communication
 * 3. Tune commands parsing (ex. BDP, BCP)
 * 4. Tune commands to BAF/BTF command message translation, serialization
 * 5. Converts tune commands to BTF commands for BTF (for testing or HU tuning)
 * 6. Sending related messages to BAF
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */
typedef void BTF;

typedef uint8_t BTF_RET_CODE_TYPE;

#define BTF_VERSION "00.00.AA"

/**
 * @ingroup BTF
 *
 * @brief This API returns a pointer to the BTF object corresponding to the CART_ID
 * provided as argument.
 * 
 * @param btf_id CART_ID of the BTF module
 */
BTF* BTF_create(CART_ID btf_id);

/**
 * @ingroup BTF
 *
 * @brief Returns the resources required by the BTF object 
 *
 * @param btf Pointer to the BTF object
 */
BTF_Resource* BTF_resource(BTF* btf);

/**
 * @ingroup BTF
 *
 * @brief This API initializes the BTF module
 * 
 * @param btf Pointer to the BTF object
 * 
 * @param arg context pointer to be supplied
 */
CART_STATUS BTF_init(BTF* btf, void* arg);

/**
 * @ingroup BTF
 *
 * @brief This API is used to send tune commands to BTF (ex. BDP. BCP)
 * 
 * @param btf Pointer to the BTF object
 * 
 * @param req Pointer to the request message
 * 
 * @param res Pointer to the response message
 */
CART_Cmd_Ret BTF_command(BTF* btf, CART_Message* req, CART_Message* res);

/**
 * @ingroup BTF
 *
 * @brief This API is invoked by the BTF_receive() dispatcher to send BAF responses to
 * BTF (ex. TOP, TSP reads)
 * 
 * @param btf Pointer to the BTF object
 * 
 * @param res
 */
CART_Cmd_Ret BTF_receive(BTF* btf, CART_Message* res);

/**
 * @ingroup BTF
 *
 * @brief This API translates Tune commands to BAF command message(s), serializes and
 * sends the message to BAF
 * 
 * @param btf Pointer to the BTF object
 * 
 * @param exec_id Integer value indicatinc the thread execution function to be
 * executed. Set to CART_EXEC_ID_DEFAULT if post() callback is not used
 * 
 * @param ctx Context pointer
 */
uint32_t BTF_exec(BTF* btf, CART_EXEC_ID exec_id, void* ctx); 



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_BTF__INCLUDED_)*/
