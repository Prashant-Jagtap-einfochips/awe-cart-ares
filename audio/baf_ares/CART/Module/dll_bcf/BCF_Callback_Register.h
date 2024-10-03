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
 * @file BCF_Callback_Register.h
 * 
 * @ingroup BCF_Callback_Register
 *
 * @brief Implementation of the Class BCF_Callback_Register       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_BCF_Callback_Register__INCLUDED_)
#define BC_BCF_Callback_Register__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include "BCF_Callback_Func.h"


/**
 * @ingroup BCF_Callback_Register
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef struct BCF_Callback_Register
{
	BCF_Callback_Func_Post post;
	BCF_Callback_Func_Clock clock;
	BCF_Callback_Func_Notify notify;
	BCF_Callback_Func_Send send;
	BCF_Callback_Func_Print print;
} BCF_Callback_Register;

/**
 * @ingroup BCF_Callback_Register
 *
 * @brief @param funcptr
 */
void CART_Callback_Register_post(BCF_Callback_Func_Post funcptr);

/**
 * @ingroup BCF_Callback_Register
 *
 * @brief @param funcptr
 */
void CART_Callback_Register_notify(BCF_Callback_Func_Notify funcptr);

/**
 * @ingroup BCF_Callback_Register
 *
 * @brief @param funcptr
 */
void CART_Callback_Register_send(BCF_Callback_Func_Send funcptr);

/**
 * @ingroup BCF_Callback_Register
 *
 * @brief @param funcptr
 */
void CART_Callback_Register_clock(BCF_Callback_Func_Clock funcptr);

/**
 * @ingroup BCF_Callback_Register
 *
 * @brief @param funcptr
 */
void CART_Callback_Register_print(BCF_Callback_Func_Print funcptr); 



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_BCF_Callback_Register__INCLUDED_)*/
