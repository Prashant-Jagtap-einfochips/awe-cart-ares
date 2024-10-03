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
 * @file CART_MESSAGE_TYPE.h
 * 
 * @ingroup CART_MESSAGE_TYPE
 *
 * @brief Implementation of the Enumeration CART_MESSAGE_TYPE       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_CART_MESSAGE_TYPE__INCLUDED_)
#define BC_CART_MESSAGE_TYPE__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @ingroup CART_MESSAGE_TYPE
 *
 * @brief Message Identifiers for CART commands.
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */
typedef enum
{
	/**
	 * @ingroup CART_MESSAGE_TYPE
	 *
	 * @brief Request message. Usually set for the req argument of command() api and send()
	 * callback
	 */
	CART_MESSAGE_TYPE_REQ,

	/**
	 * @ingroup CART_MESSAGE_TYPE
	 *
	 * @brief Response message. Usually set for the rsp argument of command() and receive()
	 * APIs
	 */
	CART_MESSAGE_TYPE_RSP,

	/**
	 * @ingroup CART_MESSAGE_TYPE
	 *
	 * @brief Response message. Usually set for the notify argument of notify() callback
	 */
	CART_MESSAGE_TYPE_NOTIFY,

	/**
	 * @ingroup CART_MESSAGE_TYPE
	 *
	 * @brief Response message with error
	 */
	CART_MESSAGE_TYPE_ERROR,

	/**
	 * @ingroup CART_MESSAGE_TYPE
	 *
	 * @brief Request message that does not have a response. The rsp argument of command()
	 * API has this type if the req argument had it.
	 */
	CART_MESSAGE_TYPE_NORSP,

	/**
	 * @ingroup CART_MESSAGE_TYPE
	 *
	 * @brief Response message with no meningful content. It can be dropped
	 */
	CART_MESSAGE_TYPE_IGNORE
} CART_MESSAGE_TYPE;


#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_CART_MESSAGE_TYPE__INCLUDED_)*/
