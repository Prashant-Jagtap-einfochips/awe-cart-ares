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
 * @brief Implementation of the Class Heap       
 *
 * @author: cb1037694                     
 ****************************************************/

#include "Heap.h"

void Heap_init(Heap* this, void* base, uint32_t size)
{
	this->base = base;
	this->top = base;
	this->size = size;
}

void * Heap_alloc(Heap* this, uint32_t size)
{
	void * alloc_ptr = this->top;
	this->top = (void*)((intptr_t)this->top + size);
	if ( this->top >= (void*)( (intptr_t)this->base + this->size )) {
		Error_print(NULL, 0, ERROR_OUT_OF_MEMORY, "Out of memory %d\n", size, 0);
		return NULL; 
	}
	return  alloc_ptr;
}

void * Heap_alloc_align(Heap* this, uint32_t size, uint32_t align)
{
	if(align & (align-1)) {
		Error_print(NULL, 0, ERROR_BAD_ALIGNMENT, "Alignment must be power of two %d\n", align, 0);
		return NULL; 
	}
	void * alloc_ptr =  (void*)(((intptr_t)this->top + (align-1)) & ~(align-1));
	this->top =  (void*)((intptr_t)alloc_ptr + size);
	if ( this->top >= (void*)((intptr_t)this->base + this->size )) {
		Error_print(NULL, 0, ERROR_OUT_OF_MEMORY, "Out of memory %d\n", size, 0);
		return NULL;
	}
	return  alloc_ptr;
} 

