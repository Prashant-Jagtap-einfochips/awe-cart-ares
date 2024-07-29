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
 * @brief Implementation of the Class JSched       
 *
 * @author: Constantin Bajenaru                     
 ****************************************************/

#include "JSched.h"
static uint32_t getnzidx(JExec* execs[]){
    uint32_t i; 
    for ( i = 0; i < JSCHED_MAX_NUM_EXEC; i++){
        if(execs[i]) break;
    } 
    return i; 
}

/**
 * @ingroup JSched
 *
 * @brief Scheduling kernel. It is invoked after an execution element is posted, and at the end of the
 * execution of the processing of an execution element.
 */

/**
 * Scheduling kernel. It is invoked after an execution element is posted, and at the end of the
 * execution of the processing of an execution element.
 */
static void JSched_kernel(JSched* this)
{
	uint32_t level, min_level;
	uint32_t gate;
	
	level = this->level;
	if ( level >= JSCHED_MAX_NUM_EXEC ){
		level = JSCHED_MAX_NUM_EXEC - 1;
	}
	
	gate = BAF_Callback_enter_cs();    
	
	min_level = getnzidx(this->execs);
	while (min_level < level) {
	    JExec *saved_exec = this->exec;
	    uint32_t saved_level = this->level;
	    JExec *exec = this->execs[min_level];
	    this->execs[min_level] = NULL;
	
	    this->level = exec->level;
	    this->exec = exec;
	
	    BAF_Callback_exit_cs(1); // enable unconditionally
	
	    (exec->func)(exec->baf, exec->level, exec->context);
	
	    BAF_Callback_enter_cs();
	    
	    this->level = saved_level;
	    this->exec = saved_exec;
	
	    min_level = getnzidx(this->execs);
	}
	BAF_Callback_exit_cs(gate);
	
}

/**
 * Initializes the scheduler
 */
void JSched_init(JSched* this)
{
	uint32_t i;
	for (i = 0; i < JSCHED_MAX_NUM_EXEC; i++) {
	    this->execs[i] = NULL;
	}
	this->level = JSCHED_MAX_NUM_EXEC;
	
}

/**
 * Post for processing an execution element
 */
void JSched_post(JSched* this, JExec* exec)
{
	uint32_t gate;
	
	if(exec->level >= JSCHED_MAX_NUM_EXEC){
	    return;
	}
	
	gate = BAF_Callback_enter_cs();
		
	if (this->execs[exec->level]) {
	    BAF_Callback_exit_cs(gate);
	    return;
	}
	this->execs[exec->level] = exec;
		
	BAF_Callback_exit_cs(gate);  
	
	JSched_kernel(this);
	
} 

