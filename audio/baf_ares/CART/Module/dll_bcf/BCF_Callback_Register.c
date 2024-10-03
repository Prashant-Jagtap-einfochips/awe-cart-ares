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
 * @brief Implementation of the Class BCF_Callback_Register       
 *
 * @author: cb1037694                     
 ****************************************************/

#include "BCF_Callback_Register.h"
BCF_Callback_Register BCF_Callback_register;
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
 
void CART_Callback_Register_post(BCF_Callback_Func_Post funcptr)
{
	BCF_Callback_register.post = funcptr;
}

void CART_Callback_Register_send(BCF_Callback_Func_Send funcptr)
{
	BCF_Callback_register.send = funcptr;
}

void CART_Callback_Register_notify(BCF_Callback_Func_Notify funcptr)
{
	BCF_Callback_register.notify = funcptr;
}

void CART_Callback_Register_clock(BCF_Callback_Func_Clock funcptr)
{
	BCF_Callback_register.clock = funcptr;
}

void CART_Callback_Register_print(BCF_Callback_Func_Print print)
{
	BCF_Callback_register.print = print;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
