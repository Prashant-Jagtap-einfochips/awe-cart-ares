/****************************************************
 *  BRF_Api.c                                         
 *  Created on: 03-Mar-2023 8:55:11 AM                      
 *  Implementation of the Class BRF_Api       
 *  Original author: cb1037694                     
 ****************************************************/

#include "BRF_Api.h"

/**
 * Returns a pointer to the current BRF object
 */
BRF* CART_create(CART_ID id)
{
	return BRF_create(id);
}

/**
 * Returns the resources required by the BRF object input parameter
 */
BRF_Resource* CART_resource(BRF* brf)
{
	return BRF_resource(brf);
}

/**
 * Initializes the BRF object. After ths API is called the real-time execution of
 * the framework can start. As pre-requisite, the resource structure returned by
 * the BRF_resource() API needs to be properly updated. 
 */
CART_STATUS CART_init(BRF* brf, void* arg)
{
	return BRF_init(brf, arg);
}

/**
 * Executes the framework thread of the BRF object associated with the
 * thread_level. This API is usually invoked periodically at a rate specific to
 * the thread_level and the current BRF object specifications.
 */
uint32_t CART_exec(BRF* brf, CART_EXEC_ID thread_level, void* arg)
{
	return BRF_exec(brf, thread_level, arg);
}

/**
 * API for sending commands to the BRF object
 */
CART_Cmd_Ret CART_command(BRF* brf, CART_Message* req, CART_Message* rsp)
{
	return  BRF_command(brf, req, rsp);
}

/**
 * API for sending commands to the BRF object
 */
CART_Cmd_Ret CART_receive(BRF* brf, CART_Message* rsp)
{
	return  BRF_receive(brf, rsp);
}


/**
 * Returns 4 hex double digit version (32bit)
 */
char* CART_version(BRF* brf)
{
	//return BRF_version(brf);
	return NULL;
} 

