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
 * @brief Implementation of the Class JModule_Context       
 *
 * @author: cb1037694                     
 ****************************************************/

#include "JModule_Context.h"

/**
 * @ingroup JModule_Context
 *
 * @brief @ingroup JModule_Context
 * 
 * @brief @param this
 * 
 * @param feature
 * 
 * @param dry
 * 
 * @param heap
 * 
 * @param context_id
 * 
 * @param this
 * 
 * @param feature
 * 
 * @param dry
 * 
 * @param heap
 * 
 * @param context_id
 */
JModule_Context * JModule_Context_create_(JModule_Context* this, JModule* feature, bool dry, Heap* heap, Context_Id context_id);

void * JModule_Context_create(JModule_Context* this, JModule* feature, Heap* heap, Context_Id context_id)
{
	this = JModule_Context_create_( this, feature, false,  heap, context_id);
	return this;
}

uint32_t JModule_Context_alloc(JModule_Context* this, JModule* feature)
{
	Heap heap_dry = {.base=(void*)0x10, .top=(void*)0x10, .size=0x1000000};
	
	JModule_Context_create_( this, feature, true,  &heap_dry, 0);
	
	return ((intptr_t)heap_dry.top - (intptr_t)heap_dry.base);
}

JModule_Context * JModule_Context_create_(JModule_Context* this, JModule* feature, bool dry, Heap* heap, Context_Id context_id)
{
	void * alloc_ptr;
	uint32_t proc_idx;
	uint32_t num_process_items, num_port_items;
	Ports * ports;
	
	alloc_ptr = Heap_alloc( heap, sizeof(JModule_Context) ); //get heap current pointer
	if( !alloc_ptr ) {
		return alloc_ptr;
	}
	this = alloc_ptr;
	
	num_process_items = feature->processes->num_items;
	alloc_ptr = Heap_alloc(heap, sizeof(Processes_Context) + num_process_items * sizeof(Process_Context));
	if( !alloc_ptr ) {
		return alloc_ptr;
	}
	if(!dry) {
	    this->processes = alloc_ptr;
	    this->processes->num_items = num_process_items;
		for ( proc_idx = 0; proc_idx < num_process_items; proc_idx++){   
	        this->processes->item[proc_idx].id = JModule_Bitfield_process(proc_idx, feature->id, context_id); // to do context #;
		}
	}
	
	for ( proc_idx = 0; proc_idx < num_process_items; proc_idx++){   
		ports = feature->processes->process[proc_idx].ports_in;
		if(ports){
			num_port_items = ports->num_items;
			alloc_ptr = Heap_alloc(heap, sizeof(Ports_Context) + num_port_items * sizeof(Port_Context));
			if(!dry){
				this->processes->item[proc_idx].ports_in = alloc_ptr;
				this->processes->item[proc_idx].ports_in->num_items = num_port_items;
			}
		}
		ports = feature->processes->process[proc_idx].ports_out;
		if(ports){
			num_port_items = ports->num_items;
			alloc_ptr = Heap_alloc(heap, sizeof(Ports_Context) + num_port_items * sizeof(Port_Context));
			if(!dry){
				this->processes->item[proc_idx].ports_out = alloc_ptr;
				this->processes->item[proc_idx].ports_out->num_items = num_port_items;
			}
		}
	}
	alloc_ptr = Heap_alloc(heap, feature->state_size);
	if(!dry) {
	    this->state = alloc_ptr;
	}
	
	alloc_ptr = Heap_alloc(heap, feature->param_size);
	if(!dry) {
	    this->param = alloc_ptr;
	}
	
	return this;
} 

