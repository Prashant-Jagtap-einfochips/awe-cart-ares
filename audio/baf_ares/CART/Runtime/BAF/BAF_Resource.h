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
 * @file BAF_Resource.h
 *
 * @ingroup BAF_Resource
 *
 * @brief Implementation of the Class BAF_Resource
 *
 * @author: cb1037694
 ****************************************************/

#if !defined(BC_BAF_Resource__INCLUDED_)
#define BC_BAF_Resource__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


/**
 * @ingroup BAF_Resource
 *
 * @brief Structure describing resources used by the framework. The create() API returns the memory_size of
 * the requested memory. The Customer Integrator should fill in the memory_pointer. The Customer
 * Integrator may provide a memory block for logging an update the structure's members,
 * memory_log_size and memory log_pointer. After updating the resource structure, the Customer
 * Integrator should call the BAF_init() API.
 *
 * @author cb1037694
 *
 * @version 1.0
 */
typedef struct BAF_Resource
{

	/**
	 * @ingroup BAF_Resource
	 *
	 * @brief Memory size required by the BAF heap. It is returned by the BAF_resource() API
	 */
	uint32_t memory_size;

	/**
	 * @ingroup BAF_Resource
	 *
	 * @brief Pointer to the memory used by the BAF heap. Customer Integrator should provide this pointer.
	 */
	void* memory_pointer;

	/**
	 * @ingroup BAF_Resource
	 *
	 * @brief Memory size used by the BAF logger heap. Customer Integrator should provide this value.
	 */
	uint32_t memory_log_size;

	/**
	 * @ingroup BAF_Resource
	 *
	 * @brief Pointer to the memory used by the BAF logger heap. Customer Integrator should provide this pointer.
	 */
	void* memory_log_pointer;

	/**
	 * @ingroup BAF_Resource
	 *
	 * @brief Memory size used by the BAF logger print log heap. Customer Integrator should provide this value.
    * This is a sub component of BAF logger heap.
	 */
	uint32_t print_log_size;

	/**
	 * @ingroup BAF_Resource
	 *
	 * @brief Memory size used by the BAF logger capture log heap. Customer Integrator should provide this value.
    * This is a sub component of BAF logger heap.
	 */
	uint32_t capture_log_size;

	/**
	 * @ingroup BAF_Resource
	 *
	 * @brief Memory size used by the BAF logger exec log heap. Customer Integrator should provide this value.
    * This is a sub component of BAF logger heap.
	 */
	uint32_t exec_log_size;

	/**
	 * @ingroup BAF_Resource
	 *
	 * @brief Number of threads in the current framework. The Customer Integrator can use this value to allocate
	 * threads/semaphores to accommodate the multithreading execution of the framework.
	 */
	uint32_t num_threads;
	/**
	 * @ingroup BAF_Resource
	 *
	 * @brief Number of thread level 0 ticks per per base thread
	 */
	uint32_t call_interval;

	/**
	 * @ingroup BAF_Resource
	 *
	 * @brief Thread level of the base rate. Base rate is usually the platform audio streaming interrupt rate
	 */
	uint32_t base_thread;

	/**
	 * @ingroup BAF_Resource
	 *
	 * @brief If set to 1 the thread level 0 is executed even if the base_thread is greater than zero
	 */
	uint32_t ext_scheduler;

	/**
	 * @ingroup BAF_Resource
	 *
	 * @brief address mask for jmodule sub-addressing
	 */
	uint32_t mask;

	/**
	 * @ingroup BAF_Resource
	 *
	 * @brief number of modules in the framework
	 */
	uint32_t num_modules;

	/**
	 * @ingroup BAF_Resource
	 *
	 * @brief number of connections in the framework
	 */
	uint32_t num_connections;

}  BAF_Resource;



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_BAF_Resource__INCLUDED_)*/
