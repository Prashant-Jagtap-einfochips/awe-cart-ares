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
 * @file BTF_Callback_Register.h
 * 
 * @ingroup BTF_Callback_Register
 *
 * @brief Implementation of the Class BTF_Callback_Register       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_BTF_Callback_Register__INCLUDED_)
#define BC_BTF_Callback_Register__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include "BTF_Callback_Func.h"


/**
 * @ingroup BTF_Callback_Register
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef struct BTF_Callback_Register
{
	BTF_Callback_Func_Post post;
	BTF_Callback_Func_Clock clock;
	BTF_Callback_Func_Notify notify;
	BTF_Callback_Func_Send send;
	BTF_Callback_Func_Print print;
} BTF_Callback_Register;

/**
 * @ingroup BTF_Callback_Register
 *
 * @brief @param funcptr
 */
void CART_Callback_Register_post(BTF_Callback_Func_Post funcptr);

/**
 * @ingroup BTF_Callback_Register
 *
 * @brief @param funcptr
 */
void CART_Callback_Register_notify(BTF_Callback_Func_Notify funcptr);

/**
 * @ingroup BTF_Callback_Register
 *
 * @brief @param funcptr
 */
void CART_Callback_Register_send(BTF_Callback_Func_Send funcptr);

/**
 * @ingroup BTF_Callback_Register
 *
 * @brief @param funcptr
 */
void CART_Callback_Register_clock(BTF_Callback_Func_Clock funcptr);

/**
 * @ingroup BTF_Callback_Register
 *
 * @brief @param funcptr
 */
void CART_Callback_Register_print(BTF_Callback_Func_Print funcptr); 



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_BTF_Callback_Register__INCLUDED_)*/
