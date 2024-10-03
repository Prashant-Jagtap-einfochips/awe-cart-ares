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
 * @file Framework_Builder.h
 * 
 * @brief Implementation of the Class Framework_Builder       
 *
 * @author: cb1037694                     
 ****************************************************/

#include "Framework_Builder.h"

JFramework* Framework_Builder_get_framework(Framework_Builder* this)
{
	return this->framework;
}

void Framework_Builder_init(Framework_Builder* this, JFramework* framework, Heap* heap, Audio_Graph* graph, JModule_List* features)
{
	uint32_t i;
		
	this->framework = framework;
	this->audio_graph = graph;
	this->features = features;
	this->heap = heap;
	
	for ( i = 0; i < features->num_items ; i++){
		features->item[i]->create(features->item[i]);
	}
	
	framework->num_module = 0;
	framework->size_module = graph->modules->num_items;
	framework->module = Heap_alloc( heap, framework->size_module * sizeof(Module) );
	
	framework->num_thread = graph->num_exec;
	framework->size_thread = graph->num_exec;
	for ( i = 0; i < framework->size_thread; i++ ) {
		framework->num_exec[i] = 0;
		framework->size_exec[i] = graph->execution[i]->num_items;
		framework->execution[i] = Heap_alloc ( heap, framework->size_exec[i] * sizeof(Execution) );
	}
	framework->num_connection = 0;
	framework->size_connection = graph->connections->num_items;
	framework->connection = Heap_alloc ( heap, framework->size_connection * sizeof (Connection) );
	
	framework->num_buffer = 0;
	framework->size_buffer = graph->ports->num_items;
	framework->buffer = Heap_alloc ( heap, framework->size_buffer * sizeof (Buffer) ); // todo: how many unique buffers
}

Error_Id Framework_Builder_build_framework(Framework_Builder* this)
{
	uint32_t i, j;
	Audio_Graph * graph = this->audio_graph;
	Connection * connection;
	Error_Id error_id;
	
	// Add features
	for (i = 0; i < graph->modules->num_items; i++){
		Framework_Builder_add_module(this, this->audio_graph->modules->item[i]);
	}
	
	// Add processes
	for (j = 0; j < graph->num_exec; j++ ){
		for (i = 0; i < graph->execution[j]->num_items; i++){
			error_id = Framework_Builder_add_execution(this, graph->execution[j]->item[i], j);
			if ( error_id != SUCCESS ) {
				return error_id;
			}
		}
		if ( graph->execution[j]->num_items != 0 ) {
			if ( this->framework->call_interval[j] == 0 /* inherit */){
				return ERROR_BAD_CALL_INTERVAL;
			}
		}
		this->framework->thread_counter[j] = this->framework->call_interval[j] - this->framework->call_interval[0];
	}
	
	// Add connections
	for (i = 0; i < graph->connections->num_items; i++){
		connection = Framework_Builder_add_connection(this, &graph->connections->item[i]);
		if ( !connection ) {
			return ERROR_BAD_CONNECTION;
		}
	}
	
	// Add buffers
	for (i = 0; i < graph->ports->num_items; i++){
		error_id = Framework_Builder_add_buffer(this, graph->ports->item[i].port_id, graph->ports->item[i].buffer_id );
		if ( error_id != SUCCESS ) {
			return error_id;
		}
	}
	return SUCCESS; // success
}

int32_t Framework_Builder_resource(Framework_Builder* this, Resource* resource, Audio_Graph* graph, JModule_List* features)
{
	JModule * feature;


	this = (this == NULL) ? (NULL) : (this);	
	resource->num_threads = graph->num_exec;
	resource->memory_size = 0;
	resource->num_modules = graph->modules->num_items;
	resource->memory_size += graph->modules->num_items * sizeof(Module);
	
	uint32_t exec_idx;
	for ( exec_idx = 0; exec_idx < graph->num_exec; exec_idx++ ) {
		resource->memory_size += graph->execution[exec_idx]->num_items * sizeof(Execution);
	}
	resource->num_connections = graph->connections->num_items;
	resource->memory_size += graph->connections->num_items * sizeof (Connection);
	resource->memory_size += graph->ports->num_items * sizeof (Buffer);
	
	resource->memory_size += graph->modules->num_items * sizeof(JModule_Context);
	
	// Add buffers
	uint32_t crt_port_idx;
	for ( crt_port_idx = 0; crt_port_idx < graph->ports->num_items; crt_port_idx++){
		
		Buffer_Id buffer_id = graph->ports->item[crt_port_idx].buffer_id;
		uint32_t port_idx;
		for ( port_idx = 0; port_idx < crt_port_idx; port_idx++ ){
			if ( buffer_id ==  graph->ports->item[port_idx].buffer_id ){
				break;
			}
		}
		if ( port_idx != crt_port_idx ){
			// TBD: Fix overallocation in case ports are not inherited
			//continue; // buffer already allocated
		}
	
		Port_Id port_id = graph->ports->item[crt_port_idx].port_id;
		uint32_t port_index = JModule_Bitfield_get_port(port_id);
		uint32_t process_index = JModule_Bitfield_get_process(port_id);
		bool     direction_in = JModule_Bitfield_get_direction(port_id) == PORT_DIRECTION_IN ? true : false;
		JModule_Id jmodule_id = JModule_Bitfield_get_feature(port_id);
		JModule * feature = JModule_List_get_feature(features, jmodule_id);
		if ( !feature ) {
			Error_print (NULL, 0, ERROR_JMODULE_NOT_FOUND, "JModule %d not found port:%d\n", jmodule_id, port_id);
			return -1;
		}
	
		Process * process = &feature->processes->process[process_index];
		
		Port * port = direction_in ? 
							Process_get_port_in( process, port_index ) : 
							Process_get_port_out( process, port_index);
	
#if defined(BAF_CONNECTION)
		uint32_t buffer_size = Port_block_size(port)*2;
#else
		uint32_t buffer_size = Port_block_size(port);
#endif
		resource->memory_size += buffer_size;
	
	}
	
	uint32_t feature_idx;
	for (feature_idx = 0; feature_idx < graph->modules->num_items; feature_idx++){
		feature = JModule_List_get_feature(features, (JModule_Id)JModule_Bitfield_get_feature( graph->modules->item[feature_idx] ));
		if ( !feature ){
			Error_print (NULL, 0, ERROR_JMODULE_NOT_FOUND, "JModule %d at index %d not found\n", 
				JModule_Bitfield_get_feature( graph->modules->item[feature_idx]), feature_idx);
			return -1;
		}
		resource->memory_size += JModule_Context_alloc(NULL, feature );
	}
	return 0;
}

Error_Id Framework_Builder_add_execution(Framework_Builder* this, Process_Id process_id, uint32_t thread_level)
{
	uint32_t module_id =  JModule_Bitfield_get_module(process_id);
	uint32_t process_idx =  JModule_Bitfield_get_process(process_id);
	Error_Id error_id;
	
	Module * module = JFramework_get_module( this->framework, module_id);
	if ( !module ) {
		return ERROR_JMODULE_NOT_FOUND;
	}
	Process* process = Module_get_process( module, process_idx);
	process->feature = module->feature;
	Process_Context* process_context = Module_get_process_context( module, process_idx);
	
	error_id = JFramework_add_execution(this->framework, process,  process_context, module->context->state, thread_level );
	
	return error_id;
}

Error_Id Framework_Builder_add_module(Framework_Builder* this, Module_Id module_id)
{
	JModule * feature = JModule_List_get_feature(this->features, (JModule_Id)JModule_Bitfield_get_feature(module_id));
	if(feature == NULL) {
		// error not found
		Error_print (NULL, 0, ERROR_JMODULE_NOT_FOUND, "JModule %d(%06x) not found\n", JModule_Bitfield_get_feature(module_id), module_id);
		return ERROR_JMODULE_NOT_FOUND;
	}
	
	JModule_Context * context = JModule_Context_create( NULL, feature, this->heap, JModule_Bitfield_get_context(module_id) );
	
	JFramework_add_module ( this->framework, module_id, feature, context );
	
	return SUCCESS;
}

/**
 * Search for buffer in the framework
 * If not found, create and add buffer to the framework
 * Attach buffer to the port
 */
Error_Id Framework_Builder_add_buffer(Framework_Builder* this, Port_Id port_id, Buffer_Id buffer_id)
{
	uint32_t i;
	uint32_t process_index = JModule_Bitfield_get_process(port_id);
	uint32_t port_index = JModule_Bitfield_get_port(port_id);
	bool     direction_in = JModule_Bitfield_get_direction(port_id) == PORT_DIRECTION_IN ? true : false;
	
	Module * module = JFramework_get_module(this->framework, JModule_Bitfield_get_module(port_id));
	if ( !module ) {
		return ERROR_JMODULE_NOT_FOUND;
	}
	Process * process = Module_get_process( module, process_index );
	if ( !process ) {
		return ERROR_INDEX_OUT_OF_RANGE;
	}

	Process_Context * process_ctx = Module_get_process_context( module, process_index );
	
	Port * port = direction_in ? 
						Process_get_port_in( process, port_index ) : 
						Process_get_port_out( process, port_index);
	Port_Context * port_ctx = direction_in ? 
									Process_Context_get_port_in ( process_ctx, port_index) : 
									Process_Context_get_port_out( process_ctx, port_index);
	
	
	void * payload = JFramework_get_buffer(this->framework, buffer_id);
	uint32_t buff_size = Port_block_size(port) ;
	if ( payload == NULL ) { // allocate payload
		if(!port_ctx->connection){
			Error_print (NULL, 0, WARNING_PORT_NOT_CONNECTED, "Warning! Port %08x not connected. No buffer allocation\n", 0, 0);
		}
	
#if defined(BAF_CONNECTION)
		payload = Heap_alloc (this->heap, buff_size * 2);
#else
		payload = Heap_alloc (this->heap, buff_size);
#endif	
		if( payload == NULL ){
			return ERROR_OUT_OF_MEMORY;
		}
	
		JFramework_add_buffer( this->framework, buffer_id, payload );
	}
	
	if ( direction_in ){
		process_ctx->ports_in->item[ port_index ].payload = payload;
	}
	else {
		process_ctx->ports_out->item[ port_index ].payload = payload;
	}
#if defined(BAF_CONNECTION)
	if ( direction_in ){
		process_ctx->ports_in->item[ port_index ].fifo[0] = payload;
		process_ctx->ports_in->item[ port_index ].fifo[1] = payload+buff_size;
	}
	else {
		process_ctx->ports_out->item[ port_index ].fifo[0] = payload;
		process_ctx->ports_out->item[ port_index ].fifo[1] = payload+buff_size;
	}
#endif
	return SUCCESS;
}

Connection* Framework_Builder_add_connection(Framework_Builder* this, Connection_Item* connection_item)
{
	JFramework * framework = this->framework;
	Connection * connection = &framework->connection [ framework->num_connection ];
	Module_Id module_id_src =  JModule_Bitfield_get_module(connection_item->src);
	Module_Id module_id_dst =  JModule_Bitfield_get_module(connection_item->dst);
	Module * module_src = JFramework_get_module ( this->framework, module_id_src);
	if ( !module_src ) {
		return NULL;
	}
	Module * module_dst = JFramework_get_module ( this->framework, module_id_dst);
	if ( !module_dst ) {
		return NULL;
	}
	Process_Id proc_id_src = JModule_Bitfield_get_process(connection_item->src);
	Process_Id proc_id_dst = JModule_Bitfield_get_process(connection_item->dst);
	Port_Id port_id_src = JModule_Bitfield_get_port(connection_item->src);
	Port_Id port_id_dst = JModule_Bitfield_get_port(connection_item->dst);
	Process* proc_src = Module_get_process( module_src, proc_id_src );
	Process* proc_dst = Module_get_process( module_dst, proc_id_dst );
	Port* port_src = Process_get_port_out(proc_src, port_id_src);
	Port* port_dst = Process_get_port_in(proc_dst, port_id_dst);
	if ( Ports_compare_merge (port_src, port_dst, proc_src->ports_out, proc_dst->ports_out) != PORT_SUCCESS ){
		return NULL;
	}
	if( !Module_set_port_out_connection(module_src, proc_id_src, port_id_src, connection )) {
		return NULL;
	}
	if ( !Module_set_port_in_connection(module_dst, proc_id_dst, port_id_dst, connection )) {
		return NULL;
	}
	
	framework->num_connection++;
	
#if defined(BAF_CONNECTION)
	// Set connection properties
	Connection_create(connection, 1, 1);
#endif
	return connection;
} 

JModule* JModule_List_get_feature(JModule_List* feature_list, JModule_Id feature_id)
{
    uint32_t i;

    for ( i = 0; i < feature_list->num_items; i++ ) {
        if ( feature_list->item[i]->id == feature_id ) {
            return feature_list->item[i];
        }
    }
    return NULL;
}

