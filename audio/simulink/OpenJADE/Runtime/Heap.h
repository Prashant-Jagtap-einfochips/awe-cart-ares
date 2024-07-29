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
 * @file Heap.h
 * 
 * @ingroup Heap
 *
 * @brief Implementation of the Class Heap       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Heap__INCLUDED_)
#define BC_Heap__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include "Error.h"


/**
 * @ingroup Heap
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef struct Heap
{
	uint32_t size;
	void* top;
	void* base;
} Heap;

/**
 * @ingroup Heap
 *
 * @brief @param this
 * 
 * @param base
 * 
 * @param size
 */
void Heap_init(Heap* this, void* base, uint32_t size);

/**
 * @ingroup Heap
 *
 * @brief @param this
 * 
 * @param size
 */
void * Heap_alloc(Heap* this, uint32_t size);

/**
 * @ingroup Heap
 *
 * @brief @param this
 * 
 * @param size
 * 
 * @param align
 */
void * Heap_alloc_align(Heap* this, uint32_t size, uint32_t align); 



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Heap__INCLUDED_)*/
