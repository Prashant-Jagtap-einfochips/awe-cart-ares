/****************************************************
 *  BAF_Api.c                                         
 *  Created on: 03-Mar-2023 8:55:11 AM                      
 *  Implementation of the Class BAF_Api       
 *  Original author: cb1037694                     
 ****************************************************/

#include "BAF_Api.h"

/**
 * Returns the resources (memory_size, number of threads) required by the BAF object input parameter
 */
BAF_Resource* BAF_Api_resource(BAF* baf)
{
	return BAF_resource(baf);
}

/**
 * Returns a pointer to the current BAF object
 */
BAF* BAF_Api_create(BAF* baf)
{
	return BAF_create(baf);
}

/**
 * Initializes the BAF object. After ths API is called the real-time execution of
 * the framework can start. As pre-requisite, the resource structure returned by
 * the BAF_resource() API needs to be properly updated. 
 */
int32_t BAF_Api_init(BAF* baf)
{
	return BAF_init(baf);
}

/**
 * Executes the framework thread of the BAF object associated with the
 * thread_level. This API is usually invoked periodically at a rate specific to
 * the thread_level and the current BAF object specifications.
 */
uint32_t BAF_Api_exec(BAF* baf, uint32_t thread_level, void* arg)
{
	return BAF_exec(baf, thread_level, arg);
}

/**
 * API for sending commands to the BAF object
 */
uint32_t BAF_Api_command(BAF* baf, BAF_Message* req, BAF_Message* rsp)
{
	return  BAF_command(baf, req, rsp);
}

/**
 * API for registering notification callback with the BAF object.
 */
void BAF_Api_register_notify(BAF* baf, BAF_Func_Notify notify)
{
	BAF_register_notify(baf, notify);
}

/**
 * Invokes OpenJADE scheduler
 */
uint32_t BAF_Api_schedule(BAF* baf, void* arg)
{
	return BAF_schedule(baf, arg);
}

/**
 * Returns 4 hex double digit version (32bit)
 */
char* BAF_Api_version(BAF* baf)
{
	return BAF_version(baf);
} 

