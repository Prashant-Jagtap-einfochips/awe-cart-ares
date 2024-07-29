/****************************************************
 *  BAF_Api.h                                         
 *  Created on: 03-Mar-2023 8:55:11 AM                      
 *  Implementation of the Class BAF_Api       
 *  Original author: cb1037694                     
 ****************************************************/

#if !defined(EA_B0A5AFE0_B1D7_4a87_A223_AE166E6871F1__INCLUDED_)
#define EA_B0A5AFE0_B1D7_4a87_A223_AE166E6871F1__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "BAF_Resource.h"
#include "BAF_Func.h"
#include "BAF_Message.h"
#include "BAF_Callback.h"


#include "BAF.h"

/**
 * This class provides the Bose Audio Framework (BAF) abstract definition and
 * implements the BAF APIs. This class statically instantiates the current BAF
 * object. The implementation will not be visible to the Customer Integrator
 */

/**
 * Returns the resources (memory_size) required by the BAF object input parameter
 */
BAF_Resource* BAF_Api_resource(BAF* baf);
/**
 * Returns a pointer to the current BAF object
 */
BAF* BAF_Api_create(BAF* baf);
/**
 * Initializes the BAF object. After ths API is called the real-time execution of
 * the framework can start. As pre-requisite, the resource structure returned by
 * the BAF_resource() API needs to be properly updated. 
 */
int32_t BAF_Api_init(BAF* baf);
/**
 * Executes the framework thread of the BAF object associated with the
 * thread_level. This API is usually invoked periodically at a rate specific to
 * the thread_level and the current BAF object specifications.
 */
uint32_t BAF_Api_exec(BAF* baf, uint32_t thread_level, void* arg);
/**
 * API for sending commands to the BAF object
 */
uint32_t BAF_Api_command(BAF* baf, BAF_Message* req, BAF_Message* rsp);
/**
 * API for registering notification callback with the BAF object.
 */
void BAF_Api_register_notify(BAF* baf, BAF_Func_Notify notify);
/**
 * Invokes OpenJADE scheduler
 */
uint32_t BAF_Api_schedule(BAF* baf, void* arg);
/**
 * Returns 4 hex double digit version (32bit)
 */
char* BAF_Api_version(BAF* baf);


#ifdef __cplusplus
}
#endif


#endif /*!defined(EA_B0A5AFE0_B1D7_4a87_A223_AE166E6871F1__INCLUDED_)*/
 