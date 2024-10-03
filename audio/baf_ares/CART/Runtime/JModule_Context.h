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
 * @file JModule_Context.h
 * 
 * @ingroup JModule_Context
 *
 * @brief Implementation of the Class JModule_Context       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_JModule_Context__INCLUDED_)
#define BC_JModule_Context__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include "JModule.h"
#include "Heap.h"
#include "JModule_Bitfield.h"

#include "Processes_Context.h"


/**
 * @ingroup JModule_Context
 *
 * @brief @ingroup JModule_Context
 * 
 * @brief @author cb1037694
 * 
 * @version 1.0
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */
typedef struct JModule_Context
{
	Module_Id id;
	Processes_Context* processes;
	void* state;
	void* param;
} JModule_Context;

/**
 * @ingroup JModule_Context
 *
 * @brief @ingroup JModule_Context
 * 
 * @brief Overlay JModule_Context structure over the instance buffer
 * 
 * @param this
 * 
 * @param feature
 * 
 * @param heap
 * 
 * @param context_id
 * 
 * @param this
 * 
 * @param feature
 * 
 * @param heap
 * 
 * @param context_id
 */
void * JModule_Context_create(JModule_Context* this, JModule* feature, Heap* heap, Context_Id context_id);

/**
 * @ingroup JModule_Context
 *
 * @brief @ingroup JModule_Context
 * 
 * @brief @param this
 * 
 * @param feature
 * 
 * @param this
 * 
 * @param feature
 */
uint32_t JModule_Context_alloc(JModule_Context* this, JModule* feature); 



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_JModule_Context__INCLUDED_)*/
