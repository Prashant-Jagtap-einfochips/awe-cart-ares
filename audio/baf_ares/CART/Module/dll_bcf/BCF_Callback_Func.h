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
 * @file BCF_Callback_Func.h
 * 
 * @ingroup BCF_Callback_Func
 *
 * @brief Implementation of the Class BCF_Callback_Func       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_BCF_Callback_Func__INCLUDED_)
#define BC_BCF_Callback_Func__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "CART.h"

/**
 * @ingroup BCF_Callback_Func
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */

/**
 * @ingroup BCF_Callback_Func
 *
 * @brief @param exec_id
 * 
 * @param arg
 */
typedef void (*BCF_Callback_Func_Post)(CART_EXEC_ID exec_id, void* arg);

typedef CART_Cmd_Ret (*BCF_Callback_Func_Notify)(CART_Message* status);

typedef CART_Cmd_Ret (*BCF_Callback_Func_Send)(CART_Message* req);

typedef uint32_t (*BCF_Callback_Func_Clock)();


typedef uint32_t (*BCF_Callback_Func_Print)(char*, ...);


#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_BCF_Callback_Func__INCLUDED_)*/
