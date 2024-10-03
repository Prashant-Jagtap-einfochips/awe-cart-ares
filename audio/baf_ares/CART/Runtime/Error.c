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
 * @file Error.h
 * 
 * @brief Implementation of the Class Error       
 *
 * @author: cb1037694                     
 ****************************************************/

#include "Error.h"

#ifdef BAF_PRINTF_SUPPORTED
extern int BAF_printf(const char*,...);
#else
#define BAF_printf(_x,...) //x
#endif

void Error_print(Error* this, uint32_t sender_id, Error_Id error_id, const char* msg, uint32_t arg1, uint32_t arg2)
{
	if(this) {
		this->sender_id = sender_id;
		this->id = error_id;
		this->msg = msg;
		this->arg1 = arg1;
		this->arg2 = arg2;
	}
	BAF_printf("Module:%08lX Error:%08X\n", sender_id, error_id);
	BAF_printf(msg, arg1, arg2);
	//  Add support for an optional error hook call
} 

