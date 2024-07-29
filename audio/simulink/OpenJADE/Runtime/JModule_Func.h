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
 * @file JModule_Func.h
 * 
 * @ingroup JModule_Func
 *
 * @brief Implementation of the Class JModule_Func       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_JModule_Func__INCLUDED_)
#define BC_JModule_Func__INCLUDED_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @ingroup JModule_Func
 *
 * @brief Type definitions for pointer to functions used by structures in the JModule component.
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */
typedef struct JModule_Func
{
	void* func;
} JModule_Func;

/**
 * @ingroup JModule_Func
 *
 * @brief Pointer to feature creation API
 */
typedef void (*JModule_Func_Create)();

/**
 * @ingroup JModule_Func
 *
 * @brief Pointer to JModule initialization API
 */
typedef void (*JModule_Func_Init)();

/**
 * @ingroup JModule_Func
 *
 * @brief Pointer to Process execution API
 */
typedef void (*JModule_Func_Process)();

/**
 * @ingroup JModule_Func
 *
 * @brief Pointer to notification callback
 */
typedef void (*JModule_Func_Notify)();

/**
 * @ingroup JModule_Func
 *
 * @brief Pointer to command sender API
 */
typedef uint32_t (*JModule_Func_Command)();

/**
 * @ingroup JModule_Func
 *
 * @brief Pointer to command sender API
 */
typedef uint32_t (*JModule_Func_Send)();

/**
 * @ingroup JModule_Func
 *
 * @brief Pointer to notification registration API
 */
typedef void (*JModule_Func_Register_Notify)();

/**
 * @ingroup JModule_Func
 *
 * @brief Pointer to notification polling API
 */
typedef uint32_t (*JModule_Func_Poll)();

/**
 * @ingroup JModule_Func
 *
 * @brief Pointer to payload API
 */
typedef void (*JModule_Func_Payload)(); 



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_JModule_Func__INCLUDED_)*/
