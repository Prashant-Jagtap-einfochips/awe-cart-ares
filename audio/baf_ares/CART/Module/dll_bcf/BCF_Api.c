/****************************************************
 *  BCF_Api.c                                         
 *  Created on: 03-Mar-2023 8:55:11 AM                      
 *  Implementation of the Class BCF_Api       
 *  Original author: cb1037694                     
 ****************************************************/

#include "BCF_Api.h"

/**
 * Returns a pointer to the current BCF object
 */
BCF* CART_create(CART_ID id)
{
	return BCF_create(id);
}

/**
 * Returns the resources required by the BCF object input parameter
 */
BCF_Resource* CART_resource(BCF* bcf)
{
	return BCF_resource(bcf);
}


/**
 * Initializes the BCF object. After ths API is called the real-time execution of
 * the framework can start. As pre-requisite, the resource structure returned by
 * the BCF_resource() API needs to be properly updated. 
 */
CART_STATUS CART_init(BCF* bcf, void* arg)
{
	return BCF_init(bcf, arg);
}

/**
 * Executes the framework thread of the BCF object associated with the
 * thread_level. This API is usually invoked periodically at a rate specific to
 * the thread_level and the current BCF object specifications.
 */
uint32_t CART_exec(BCF* bcf, CART_EXEC_ID thread_level, void* arg)
{
	return BCF_exec(bcf, thread_level, arg);
}

/**
 * API for sending commands to the BCF object
 */
CART_Cmd_Ret CART_command(BCF* bcf, CART_Message* req, CART_Message* rsp)
{
	return  BCF_command(bcf, req, rsp);
}

/**
 * API for sending commands to the BCF object
 */
CART_Cmd_Ret CART_receive(BCF* bcf, CART_Message* rsp)
{
	return  BCF_receive(bcf, rsp);
}


/**
 * Returns 4 hex double digit version (32bit)
 */
char* CART_version(BCF* bcf)
{
	//return BCF_version(bcf);
	return NULL;
} 

