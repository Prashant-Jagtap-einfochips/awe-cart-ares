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
 * @file BTF_Callback_Func.h
 * 
 * @ingroup BTF_Callback_Func
 *
 * @brief Implementation of the Class BTF_Callback_Func       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_BTF_Callback_Func__INCLUDED_)
#define BC_BTF_Callback_Func__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "CART.h"

/**
 * @ingroup BTF_Callback_Func
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */

/**
 * @ingroup BTF_Callback_Func
 *
 * @brief @param exec_id
 * 
 * @param arg
 */
typedef void (*BTF_Callback_Func_Post)(CART_EXEC_ID exec_id, void* arg);

typedef CART_Cmd_Ret (*BTF_Callback_Func_Notify)(CART_Message* status);

typedef CART_Cmd_Ret (*BTF_Callback_Func_Send)(CART_Message* req);

typedef uint32_t (*BTF_Callback_Func_Clock)();


typedef uint32_t (*BTF_Callback_Func_Print)(char*, ...);


#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_BTF_Callback_Func__INCLUDED_)*/
