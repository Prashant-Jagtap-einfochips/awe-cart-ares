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
 * @file Process.h
 * 
 * @ingroup Process
 *
 * @brief Implementation of the Class Process       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Process__INCLUDED_)
#define BC_Process__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include "Process_Context.h"
#include "Connection.h"
#include "JModule_Func.h"

#include "Logger.h"
#include "Ports.h"


/**
 * @ingroup Process
 *
 * @brief This class contains data structures and API required for executing a JModule Process. A process may
 * operate on any number of input and/or output ports.
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */
typedef struct Process
{
	/**
	 * @ingroup Process
	 *
	 * @brief Pointer to the process execution function. The JModule Developer implements the Algorithm wrapper
	 * API, which will be assigned to this pointer to function, i.e. registered with the Process.
	 */
	JModule_Func_Process exec;

	/**
	 * @ingroup Process
	 *
	 * @brief For synchronous processes, specifies the calling interval, in samples for the current process.
	 */
	uint32_t call_interval;

	/**
	 * @ingroup Process
	 *
	 * @brief Pointer to the input Ports container
	 */
	Ports* ports_in;

	/**
	 * @ingroup Process
	 *
	 * @brief Pointer to the output Ports container
	 */
	Ports* ports_out;

	/**
	 * @ingroup Process
	 *
	 * @brief Pointer to a feature context that will be used by the process API to access JModule states or
	 * parameters.
	 */
	void* feature;
} Process;

/**
 * @ingroup Process
 *
 * @brief API that invokes the exec pointer to function. The API first checks the availability of the buffers
 * associated with the input/output Ports, and makes these buffers available after the exec pointer to
 * function execution.
 * 
 * @param this Pointer to the Process structure
 * 
 * @param context Pointer to the Process_Context of a particular JModule instance
 * 
 * @param thread_level The thread level of the Process
 * 
 * @param arg Context pointer
 */
void Process_exec(Process* this, Process_Context* context, uint32_t thread_level, void* arg);

/**
 * @ingroup Process
 *
 * @brief API that returns pointer to an input port specified by its index. The API returns NULL if the index
 * is out of range.
 * 
 * @param this Pointer to the Process structure
 * 
 * @param port_index Index of the input port to be returned
 */
Port* Process_get_port_in(Process* this, uint32_t port_index);

/**
 * @ingroup Process
 *
 * @brief API that returns pointer to an output port specified by its index. The API returns NULL if the
 * index is out of range.
 * 
 * @param this Pointer to the Process structure
 * 
 * @param port_index Index of the output port to be returned
 */
Port* Process_get_port_out(Process* this, uint32_t port_index); 



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Process__INCLUDED_)*/
