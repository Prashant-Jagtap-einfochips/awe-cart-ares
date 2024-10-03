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
 * @file JModule.h
 * 
 * @ingroup JModule
 *
 * @brief Implementation of the Class JModule       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_JModule__INCLUDED_)
#define BC_JModule__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include "JModule_Bitfield.h"
#include "JModule_Func.h"
#include "JModule_Id.h"
#include "Processes.h"


/**
 * @ingroup JModule
 *
 * @brief This is the top structure of a JModule component. The JModule component contains the
 * classes/structures required for defining a JModule in a JModule Library. Within structure
 * declarations, whenever possible, Variable Length Array (VLA) are used instead of Array of Pointers.
 * VLA has the advantage of faster access, but the restriction of being the last member in the
 * structure. VLA are allocated, most conveniently through C structure initializers.
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */
typedef struct JModule
{
	/**
	 * @ingroup JModule
	 *
	 * @brief Unique identifier listed in the JModule_Id enumeration.
	 */
	JModule_Id id;

	/**
	 * @ingroup JModule
	 *
	 * @brief Pointer to the JModule creation API. This API should assign APIs to the init, command,
	 * register_notify and the exec for each process pointers to functions. This API should be exposed to
	 * the JModule_Desc file.
	 */
	JModule_Func_Create create;

	/**
	 * @ingroup JModule
	 *
	 * @brief Pointer to the feature initialization API. This API should call algorithm specific initialization
	 * APIs and possibly initialize the state of the feature. The API has provisions for initializing
	 * multiple JModule instances.
	 */
	JModule_Func_Init init;

	/**
	 * @ingroup JModule
	 *
	 * @brief Pointer to the command sending API
	 */
	JModule_Func_Send command;

	/**
	 * @ingroup JModule
	 *
	 * @brief Pointer to the API that registers a notification function with the algorithm.
	 */
	JModule_Func_Register_Notify register_notify;

	/**
	 * @ingroup JModule
	 *
	 * @brief Pointer to the API that retrieves a notification from the JModule.
	 */
	JModule_Func_Notify notify;

	/**
	 * @ingroup JModule
	 *
	 * @brief Pointer to the API that checks if JModule notifications are present.
	 */
	JModule_Func_Poll poll;

	/**
	 * @ingroup JModule
	 *
	 * @brief Pointer to the feature's Processes module
	 */
	Processes* processes;

	/**
	 * @ingroup JModule
	 *
	 * @brief Size, in bytes, of the state structure. A memory buffer of this size will be allocated in the
	 * JModule_Context structure for storing local states.
	 */
	uint32_t state_size;

	/**
	 * @ingroup JModule
	 *
	 * @brief Size, in bytes, of the parameter structure.  A memory buffer of this size will be allocated in the
	 * JModule_Context structure for storing local parameters.
	 */
	uint32_t param_size;

	/**
	 * @ingroup JModule
	 *
	 * @brief base_thread, in bytes, of the parameter structure. base_call_rate task number
	 * JModule_Context structure for storing local parameters.
	 */
	uint32_t base_thread;

	/**
	 * @ingroup JModule
	 *
	 * @brief Pointer to the parent JFramework
	 */
	void* framework;

	/**
	 * @ingroup JModule
	 *
	 * @brief Pointer to the feature parameter structure. It is declared as void* so the type can be overloaded
	 * by the JModule Developer. Set to NULL if the feature has no parameters, but param_size can be set
	 * to a non-zero value, so local parameters can be stored in the JModule_Context
	 */
	void* param;

	/**
	 * @ingroup JModule
	 *
	 * @brief Pointer to the feature state structure. It is declared as void* so the type can be overloaded by
	 * the JModule Developer. Set to NULL if the feature has no states, but state_size can be set to a non-
	 * zero value, so local parameters can be stored in the JModule_Context
	 */
	void* state;
} JModule;

/**
 * @ingroup JModule
 *
 * @brief API for invoking feature's pointer to the create function.
 * 
 * @param this Pointer to the JModule for which the create pointer to function will be invoked
 */
void JModule_create(JModule* this); 



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_JModule__INCLUDED_)*/
