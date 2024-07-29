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
 * @file BAF_Callback_Register.h
 * 
 * @brief Implementation of the Class BAF_Callback_Register       
 *
 * @author: cb1037694                     
 ****************************************************/

#include "BAF_Callback_Register.h"
BAF_Callback_Register BAF_Callback_register;
 
void BAF_Callback_Register_post(BAF_Callback_Func_Post funcptr)
{
	BAF_Callback_register.post = funcptr;
}

void BAF_Callback_Register_clock(BAF_Callback_Func_Clock funcptr)
{
	BAF_Callback_register.clock = funcptr;
}

void BAF_Callback_Register_sys_process(BAF_Callback_Func_Sys_Process funcptr)
{
	BAF_Callback_register.sys_process = funcptr;
}

void BAF_Callback_Register_sys_process_ctx(BAF_Callback_Func_Sys_Process_Ctx funcptr)
{
	BAF_Callback_register.sys_process_ctx = funcptr;
} 

void BAF_Callback_Register_print(BAF_Callback_Func_Print print)
{
	BAF_Callback_register.print = print;
}

