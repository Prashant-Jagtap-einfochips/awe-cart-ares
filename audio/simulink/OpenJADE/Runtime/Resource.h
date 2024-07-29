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
 * @file Resource.h
 *
 * @ingroup Resource
 *
 * @brief Implementation of the Class Resource
 *
 * @author: cb1037694
 ****************************************************/

#if !defined(BC_Resource__INCLUDED_)
#define BC_Resource__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


/**
 * @ingroup Resource
 *
 * @brief @author cb1037694
 *
 * @version 1.0
 */
typedef struct Resource
{
	uint32_t memory_size;
	void* memory_pointer;
	uint32_t memory_log_size;
	void* memory_log_pointer;
	uint32_t print_log_size;
	uint32_t capture_log_size;
	uint32_t exec_log_size;
	uint32_t num_threads;
	uint32_t call_interval;
	uint32_t base_thread;
	uint32_t ext_scheduler;
	uint32_t mask;
	uint32_t num_modules;
	uint32_t num_connections;
}  Resource;



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Resource__INCLUDED_)*/
