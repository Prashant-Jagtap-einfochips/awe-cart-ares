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
 * @brief Implementation of the Class BTF_Callback_Register       
 *
 * @author: cb1037694                     
 ****************************************************/

#include "BTF_Callback_Register.h"
BTF_Callback_Register BTF_Callback_register;
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
 
void CART_Callback_Register_post(BTF_Callback_Func_Post funcptr)
{
	BTF_Callback_register.post = funcptr;
}

void CART_Callback_Register_send(BTF_Callback_Func_Send funcptr)
{
	BTF_Callback_register.send = funcptr;
}

void CART_Callback_Register_notify(BTF_Callback_Func_Notify funcptr)
{
	BTF_Callback_register.notify = funcptr;
}

void CART_Callback_Register_clock(BTF_Callback_Func_Clock funcptr)
{
	BTF_Callback_register.clock = funcptr;
}

void CART_Callback_Register_print(BTF_Callback_Func_Print print)
{
	BTF_Callback_register.print = print;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
