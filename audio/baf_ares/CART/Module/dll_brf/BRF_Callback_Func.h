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
 * @file BRF_Callback_Func.h
 * 
 * @ingroup BRF_Callback_Func
 *
 * @brief Implementation of the Class BRF_Callback_Func       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_BRF_Callback_Func__INCLUDED_)
#define BC_BRF_Callback_Func__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "CART.h"
#include "BRF_Callback.h"

/**
 * @ingroup BRF_Callback_Func
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */

/**
 * @ingroup BRF_Callback_Func
 *
 * @brief @param exec_id
 * 
 * @param arg
 */
typedef void (*BRF_Callback_Func_Post)(CART_EXEC_ID exec_id, void* arg);

typedef CART_Cmd_Ret (*BRF_Callback_Func_Notify)(CART_Message* status);

typedef CART_Cmd_Ret (*BRF_Callback_Func_Send)(CART_Message* req);

typedef CART_Cmd_Ret (*BRF_Callback_Func_Read)(BRF_STORAGE_ID storage_id, uint32_t address, uint32_t size, uint8_t* buf, void* ctx); 

typedef uint32_t (*BRF_Callback_Func_Clock)();

typedef uint32_t (*BRF_Callback_Func_Print)(char*, ...);


#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_BRF_Callback_Func__INCLUDED_)*/
