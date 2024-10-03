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
 * @file CART_STATUS.h
 * 
 * @ingroup CART_STATUS
 *
 * @brief Implementation of the Enumeration CART_STATUS       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_CART_STATUS__INCLUDED_)
#define BC_CART_STATUS__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @ingroup CART_STATUS
 *
 * @brief Return status for some CART APIs, like init()
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */
typedef enum
{
	/**
	 * @ingroup CART_STATUS
	 *
	 * @brief Success
	 */
	CART_STATUS_SUCCESS = 0,

	/**
	 * @ingroup CART_STATUS
	 *
	 * @brief Error
	 */
	CART_STATUS_ERROR = -127,

	/**
	 * @ingroup CART_STATUS
	 *
	 * @brief Error
	 */
	CART_STATUS_INVALID,

	/**
	 * @ingroup CART_STATUS
	 *
	 * @brief Error
	 */
	CART_STATUS_TIMEOUT,

	/**
	 * @ingroup CART_STATUS
	 *
	 * @brief Error
	 */
	CART_STATUS_OUTOFRANGE,
	CART_STATUS_QUEUE_FULL,
    /**
     * @ingroup CART_STATUS
     *
     * @brief Error
     */
    CART_STATUS_DISCONNECTED,
    BCF_RET_SUCCESS=CART_STATUS_SUCCESS,
    BCF_RET_ERROR=CART_STATUS_ERROR,
    BCF_RET_BUSY,
    BCF_RET_SOURCE_OOR,
    BCF_RET_MAX_INPUT_EXCEEDED,
    BCF_RET_MAX_OUTPUT_EXCEEDED,
    BCF_RET_TIMEOUT,
    CART_STATUS_INDX_OOR,
    BCF_RET_INDX_OOR = CART_STATUS_INDX_OOR,
    BCF_RET_MIC_LEVEL_FAILED,
    BCF_RET_LOCK_FAIL,
    BCF_RET_INVALID,
    BCF_RET_NO_ACTION,
    CART_STATUS_NO_ACTION = BCF_RET_NO_ACTION,
    BCF_RET_NO_CONNECT,
} CART_STATUS;

typedef enum
{
   RET_SUCCESS = 1,
   RET_ERROR,
   RET_BUSY,
   RET_SOURCE_OOR,
   RET_MAX_INPUT_EXCEEDED,
   RET_MAX_OUTPUT_EXCEEDED,
   RET_TIMEOUT,
   RET_INDX_OOR,            /* BAMF Index Out Of Range Error */
   RET_MIC_LEVEL_FAILED,    /* Mic Level Failed */
   RET_LOCK_FAIL,
   RET_INVALID,
   RET_NO_ACTION,            /* No action was required. ie - all channels already muted. */
   RET_NO_CONNECT,
} RET_CODE_TYPE;

#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_CART_STATUS__INCLUDED_)*/
