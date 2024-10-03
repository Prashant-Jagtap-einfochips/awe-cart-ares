/****************************************************
 *  BTF_Api.c                                         
 *  Created on: 03-Mar-2023 8:55:11 AM                      
 *  Implementation of the Class BTF_Api       
 *  Original author: cb1037694                     
 ****************************************************/

#include "BTF_Api.h"

/**
 * Returns a pointer to the current BTF object
 */
BTF* CART_create(CART_ID id)
{
	return BTF_create(id);
}

/**
 * Returns the resources required by the BTF object input parameter
 */
BTF_Resource* CART_resource(BTF* btf)
{
	return BTF_resource(btf);
}

/**
 * Initializes the BTF object. After ths API is called the real-time execution of
 * the framework can start. As pre-requisite, the resource structure returned by
 * the BTF_resource() API needs to be properly updated. 
 */
CART_STATUS CART_init(BTF* btf, void* arg)
{
	return BTF_init(btf, arg);
}

/**
 * Executes the framework thread of the BTF object associated with the
 * thread_level. This API is usually invoked periodically at a rate specific to
 * the thread_level and the current BTF object specifications.
 */
uint32_t CART_exec(BTF* btf, CART_EXEC_ID thread_level, void* arg)
{
	return BTF_exec(btf, thread_level, arg);
}

/**
 * API for sending commands to the BTF object
 */
CART_Cmd_Ret CART_command(BTF* btf, CART_Message* req, CART_Message* rsp)
{
	return  BTF_command(btf, req, rsp);
}

/**
 * API for sending commands to the BTF object
 */
CART_Cmd_Ret CART_receive(BTF* btf, CART_Message* rsp)
{
	return  BTF_receive(btf, rsp);
}


/**
 * Returns 4 hex double digit version (32bit)
 */
char* CART_version(BTF* btf)
{
	//return BTF_version(btf);
	return NULL;
} 

