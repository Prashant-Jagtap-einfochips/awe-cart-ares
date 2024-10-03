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
 * @file CART_Cmd_Ret.h
 * 
 * @ingroup CART_Cmd_Ret
 *
 * @brief Implementation of the Class CART_Cmd_Ret       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_CART_Cmd_Ret__INCLUDED_)
#define BC_CART_Cmd_Ret__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "CART_STATUS.h"

/**
 * @ingroup CART_Cmd_Ret
 *
 * @brief command() API return typedef. This is a union reflecting the dual meaning of a
 * command API return.
 * If status > = 0 then the command() was processed successfully and the return
 * value is interpreted as response size.
 * If status < 0 then the command() has encountered an error.
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */
typedef union CART_Cmd_Ret 
{

	/**
	 * @ingroup CART_Cmd_Ret
	 *
	 * @brief size in bytes of the payload of the received message
	 */
	int32_t res_size;

	/**
	 * @ingroup CART_Cmd_Ret
	 *
	 * @brief error if < 0
	 */
	CART_STATUS status;
}  CART_Cmd_Ret;



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_CART_Cmd_Ret__INCLUDED_)*/
