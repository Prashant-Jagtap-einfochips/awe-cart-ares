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
 * @file JSched.h
 * 
 * @ingroup JSched
 *
 * @brief Implementation of the Class JSched       
 *
 * @author: Constantin Bajenaru                     
 ****************************************************/

#if !defined(BC_JSched__INCLUDED_)
#define BC_JSched__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "Framework_Param.h"

#define JSCHED_MAX_NUM_EXEC (FW_NUM_THREAD + 1)
extern  uint32_t BAF_Callback_enter_cs(void);
extern  void     BAF_Callback_exit_cs(uint32_t gate);

#include "JExec.h"


/**
 * @ingroup JSched
 *
 * @brief Execution elements scheduler.
 * 
 * @author Constantin Bajenaru
 * 
 * @version 1.0
 */
typedef struct JSched
{
	/**
	 * @ingroup JSched
	 *
	 * @brief Currently scheduled priority level
	 */
	uint32_t level;

	/**
	 * @ingroup JSched
	 *
	 * @brief Current execution element
	 */
	JExec* exec;

	/**
	 * @ingroup JSched
	 *
	 * @brief Array of execution elements in ready state. The element with lower index has higher priority.
	 */
	JExec* execs[JSCHED_MAX_NUM_EXEC];
} JSched;

/**
 * @ingroup JSched
 *
 * @brief Initializes the scheduler
 */
void JSched_init(JSched* this);

/**
 * @ingroup JSched
 *
 * @brief Post for processing an execution element
 * 
 * @param exec Pointer to execution element to be posted
 */
void JSched_post(JSched* this, JExec* exec); 



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_JSched__INCLUDED_)*/
