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
 * @file Process_Context.h
 * 
 * @ingroup Process_Context
 *
 * @brief Implementation of the Class Process_Context       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Process_Context__INCLUDED_)
#define BC_Process_Context__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include "Process_Mode.h"

#include "Ports_Context.h"


/**
 * @ingroup Process_Context
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef struct Process_Context
{
	uint32_t id;
	Ports_Context* ports_in;
	Ports_Context* ports_out;
	void* module;
	uint32_t mode;
	uint8_t dbg_port;
	uint8_t dbg_channel;
} Process_Context;

/**
 * @ingroup Process_Context
 *
 * @brief @param this
 * 
 * @param port_index
 */
Port_Context* Process_Context_get_port_in(Process_Context* this, uint32_t port_index);

/**
 * @ingroup Process_Context
 *
 * @brief @param this
 * 
 * @param port_index
 */
Port_Context* Process_Context_get_port_out(Process_Context* this, uint32_t port_index); 



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Process_Context__INCLUDED_)*/
