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
 * @file JExec.h
 * 
 * @ingroup JExec
 *
 * @brief Implementation of the Class JExec       
 *
 * @author: Constantin Bajenaru                     
 ****************************************************/

#if !defined(BC_JExec__INCLUDED_)
#define BC_JExec__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
typedef void(*JExec_Func)(void*, uint32_t, void*);


/**
 * @ingroup JExec
 *
 * @brief Execution element.
 * Always runs to completion.
 * It can be preempted by an execution element with a lower priority level value.
 * 
 * @author Constantin Bajenaru
 * 
 * @version 1.0
 */
typedef struct JExec
{
	/**
	 * @ingroup JExec
	 *
	 * @brief Pointer to execution element function
	 */
	JExec_Func func;

	/**
	 * @ingroup JExec
	 *
	 * @brief Pointer to BAF object
	 */
	void* baf;

	/**
	 * @ingroup JExec
	 *
	 * @brief Context pointer passed as an argument to the func pointer
	 */
	void* context;

	/**
	 * @ingroup JExec
	 *
	 * @brief Priority level, with zero the highest
	 */
	uint32_t level;
} JExec;

/**
 * @ingroup JExec
 *
 * @brief Initialize an execution element
 * 
 * @param func Pointer to execution element function
 * 
 * @param baf Pointer to BAF object
 * 
 * @param level Priority level, with zero the highest
 * 
 * @param context Context pointer passed as an argument to the func pointer
 */
uint32_t JExec_init(JExec* this, JExec_Func func, void* baf, uint32_t level, void* context); 



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_JExec__INCLUDED_)*/
