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
 * @file BRF_Callback_Register.h
 * 
 * @ingroup BRF_Callback_Register
 *
 * @brief Implementation of the Class BRF_Callback_Register       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_BRF_Callback_Register__INCLUDED_)
#define BC_BRF_Callback_Register__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include "BRF_Callback_Func.h"


/**
 * @ingroup BRF_Callback_Register
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef struct BRF_Callback_Register
{
	BRF_Callback_Func_Post post;
	BRF_Callback_Func_Clock clock;
	BRF_Callback_Func_Notify notify;
	BRF_Callback_Func_Read read;
	BRF_Callback_Func_Send send;
	BRF_Callback_Func_Print print;
} BRF_Callback_Register;

/**
 * @ingroup CART_Callback_Register
 *
 * @brief @param funcptr
 */
void CART_Callback_Register_post(BRF_Callback_Func_Post funcptr);

/**
 * @ingroup BRF_Callback_Register
 *
 * @brief @param funcptr
 */
void CART_Callback_Register_notify(BRF_Callback_Func_Notify funcptr);

/**
 * @ingroup BRF_Callback_Register
 *
 * @brief @param funcptr
 */
void CART_Callback_Register_send(BRF_Callback_Func_Send funcptr);

/**
 * @ingroup BRF_Callback_Register
 *
 * @brief @param funcptr
 */
void CART_Callback_Register_read(BRF_Callback_Func_Read funcptr);

/**
 * @ingroup BRF_Callback_Register
 *
 * @brief @param funcptr
 */
void CART_Callback_Register_clock(BRF_Callback_Func_Clock funcptr);

/**
 * @ingroup BRF_Callback_Register
 *
 * @brief @param funcptr
 */
void CART_Callback_Register_print(BRF_Callback_Func_Print funcptr); 



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_BRF_Callback_Register__INCLUDED_)*/
