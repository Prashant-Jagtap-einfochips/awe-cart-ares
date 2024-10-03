/****************************************************
 *  BCF_Api.h                                         
 *  Created on: 03-Mar-2023 8:55:11 AM                      
 *  Implementation of the Class BCF_Api       
 *  Original author: cb1037694                     
 ****************************************************/

#if !defined(EA_B0A5AFE0_B1D7_4a87_A223_AE166E6871F1__INCLUDED_)
#define EA_B0A5AFE0_B1D7_4a87_A223_AE166E6871F1__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "BCF_Callback_Func.h"
#include "BCF_Callback.h"
#include "BCF.h"

/**
 * This class provides the Bose Control Framework (BCF) abstract definition and
 * implements the BCF APIs. This class statically instantiates the current BCF
 * object. The implementation will not be visible to the Customer Integrator
 */

/**
 * Returns a pointer to the current BCF object
 */
BCF* CART_create(CART_ID id);
/**
 * Returns the resources required by the BCF object input parameter
 */
BCF_Resource* CART_resource(BCF* bcf);
/**
 * Initializes the BCF object. After ths API is called the real-time execution of
 * the framework can start. As pre-requisite, the resource structure returned by
 * the BCF_resource() API needs to be properly updated. 
 */
CART_STATUS CART_init(BCF* bcf, void* arg);
/**
 * Executes the framework thread of the BCF object associated with the
 * thread_level. This API is usually invoked periodically at a rate specific to
 * the thread_level and the current BCF object specifications.
 */
uint32_t CART_exec(BCF* bcf, CART_EXEC_ID thread_level, void* arg);
/**
 * API for sending commands to the BCF object
 */
CART_Cmd_Ret CART_command(BCF* bcf, CART_Message* req, CART_Message* rsp);
/**
 * API for received command for the BCF object
 */
CART_Cmd_Ret CART_receive(BCF* bcf, CART_Message* rsp);
/**
 * Returns 4 hex double digit version (32bit)
 */
char* CART_version(BCF* bcf);


#ifdef __cplusplus
}
#endif


#endif /*!defined(EA_B0A5AFE0_B1D7_4a87_A223_AE166E6871F1__INCLUDED_)*/
 