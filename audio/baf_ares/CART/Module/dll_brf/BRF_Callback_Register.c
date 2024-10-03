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
 * @brief Implementation of the Class BRF_Callback_Register       
 *
 * @author: cb1037694                     
 ****************************************************/

#include "BRF_Callback_Register.h"
BRF_Callback_Register BRF_Callback_register;
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
 
void CART_Callback_Register_post(BRF_Callback_Func_Post funcptr)
{
	BRF_Callback_register.post = funcptr;
}

void CART_Callback_Register_send(BRF_Callback_Func_Send funcptr)
{
	BRF_Callback_register.send = funcptr;
}

void CART_Callback_Register_notify(BRF_Callback_Func_Notify funcptr)
{
	BRF_Callback_register.notify = funcptr;
}

void CART_Callback_Register_read(BRF_Callback_Func_Read funcptr)
{
	BRF_Callback_register.read = funcptr;
}

void CART_Callback_Register_clock(BRF_Callback_Func_Clock funcptr)
{
	BRF_Callback_register.clock = funcptr;
}

void CART_Callback_Register_print(BRF_Callback_Func_Print print)
{
	BRF_Callback_register.print = print;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
