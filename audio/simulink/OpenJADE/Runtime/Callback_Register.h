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
 * @ingroup BAF_Callback_Register
 *
 * @brief Implementation of the Class BAF_Callback_Register       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_BAF_Callback_Register__INCLUDED_)
#define BC_BAF_Callback_Register__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include "BAF.h"


/**
 * @ingroup BAF_Callback_Register
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef struct BAF_Callback_Register
{
	BAF_Callback_Func_Post post;
	BAF_Callback_Func_Sys_Process_Ctx sys_process_ctx;
} BAF_Callback_Register;



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_BAF_Callback_Register__INCLUDED_)*/
