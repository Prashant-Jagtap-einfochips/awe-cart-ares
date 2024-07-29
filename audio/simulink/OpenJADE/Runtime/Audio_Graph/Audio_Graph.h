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
 * @file Audio_Graph.h
 * 
 * @ingroup Audio_Graph
 *
 * @brief Implementation of the Class Audio_Graph       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Audio_Graph__INCLUDED_)
#define BC_Audio_Graph__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include "Port_List.h"
#include "Module_List.h"
#include "Connection_List.h"
#include "Execution_List.h"


/**
 * @ingroup Audio_Graph
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef struct Audio_Graph
{
	Module_List* modules;
	Connection_List* connections;
	Port_List* ports;
	uint32_t num_exec;
	uint32_t base_thread;
	uint32_t ext_scheduler;
	Execution_List* execution[
#ifdef OJ_NUM_EXECS	
		OJ_NUM_EXECS
#endif
	];
}  Audio_Graph;



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Audio_Graph__INCLUDED_)*/
