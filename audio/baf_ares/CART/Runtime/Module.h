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
 * @file Module.h
 * 
 * @ingroup Module
 *
 * @brief Implementation of the Class Module       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Module__INCLUDED_)
#define BC_Module__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include "Error.h"

#include "JModule.h"
//#include "JModule_Lib\JModule_Id.h"
#include "JModule_Context.h"


/**
 * @ingroup Module
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef struct Module
{
	Module_Id id;
	JModule* feature;
	JModule_Context* context;
} Module;

/**
 * @ingroup Module
 *
 * @brief @param this
 * 
 * @param process_index
 */
Process* Module_get_process(Module* this, uint32_t process_index);

/**
 * @ingroup Module
 *
 * @brief @param this
 * 
 * @param process_index
 */
Process_Context* Module_get_process_context(Module* this, uint32_t process_index);

/**
 * @ingroup Module
 *
 * @brief @param this
 * 
 * @param process_index
 * 
 * @param port_index
 * 
 * @param connection
 */
Connection* Module_set_port_in_connection(Module* this, uint32_t process_index, uint32_t port_index, Connection* connection);

/**
 * @ingroup Module
 *
 * @brief @param this
 * 
 * @param process_index
 * 
 * @param port_index
 * 
 * @param connection
 */
Connection* Module_set_port_out_connection(Module* this, uint32_t process_index, uint32_t port_index, Connection* connection); 



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Module__INCLUDED_)*/
