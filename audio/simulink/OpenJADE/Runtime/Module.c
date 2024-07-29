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
 * @brief Implementation of the Class Module       
 *
 * @author: cb1037694                     
 ****************************************************/

#include "Module.h"

Process* Module_get_process(Module* this, uint32_t process_index)
{
	return  &(this->feature->processes->process[process_index]);
}

Process_Context* Module_get_process_context(Module* this, uint32_t process_index)
{
	return  &(this->context->processes->item[process_index]);
}

Connection* Module_set_port_in_connection(Module* this, uint32_t process_index, uint32_t port_index, Connection* connection)
{
	if ( !this ){
		Error_print(NULL, 0, ERROR_MODULE_NULL,"Module is NULL\n", 0, 0);
		return NULL;
	}
	
	Ports_Context* ports = this->context->processes->item[process_index].ports_in;
	if (ports == NULL) {
		// error not found
		Error_print(NULL, 0, ERROR_PORT_NOT_FOUND, "Can't find ports for process %d\n", process_index, 0);
		return NULL;
	}
	
	if(ports->num_items <= port_index) {
		Error_print(NULL, 0, ERROR_INDEX_OUT_OF_RANGE, "Port Index out of range(%d) for process %d \n",port_index, process_index);
		return NULL;
	}
	ports->item[port_index].connection = connection;
	return connection;
}

Connection* Module_set_port_out_connection(Module* this, uint32_t process_index, uint32_t port_index, Connection* connection)
{
	if ( !this ){
		Error_print(NULL, 0, ERROR_INVALID_MODULE, "Module is NULL\n", 0, 0 );
		return NULL;
	}
	
	Ports_Context* ports = this->context->processes->item[process_index].ports_out;
	if (ports == NULL) {
		// error not found
		Error_print(NULL, 0, ERROR_PORT_NOT_FOUND, "Can't find ports for process %d\n",process_index, 0 );
		return NULL;
	}
	
	if(ports->num_items <= port_index) {
		Error_print(NULL, 0, ERROR_INDEX_OUT_OF_RANGE, "Port Index out of range(%d) for process %d \n", port_index, process_index);
		return NULL;
	}
	ports->item[port_index].connection = connection;
	return connection;
} 

