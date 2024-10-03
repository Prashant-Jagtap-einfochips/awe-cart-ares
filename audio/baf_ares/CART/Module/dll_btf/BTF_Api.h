/****************************************************
 *  BTF_Api.h                                         
 *  Created on: 03-Mar-2023 8:55:11 AM                      
 *  Implementation of the Class BTF_Api       
 *  Original author: cb1037694                     
 ****************************************************/

#if !defined(EA_B0A5AFE0_B1D7_4a87_A223_AE166E6871F1__INCLUDED_)
#define EA_B0A5AFE0_B1D7_4a87_A223_AE166E6871F1__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "BTF_Callback_Func.h"
#include "BTF_Callback.h"
#include "BTF.h"

/**
 * This class provides the Bose Control Framework (BTF) abstract definition and
 * implements the BTF APIs. This class statically instantiates the current BTF
 * object. The implementation will not be visible to the Customer Integrator
 */

/**
 * Returns a pointer to the current BTF object
 */
BTF* CART_create(CART_ID id);
/**
 * Returns the resources required by the BTF object input parameter
 */
BTF_Resource* CART_resource(BTF* btf);
/**
 * Initializes the BTF object. After ths API is called the real-time execution of
 * the framework can start. As pre-requisite, the resource structure returned by
 * the BTF_resource() API needs to be properly updated. 
 */
CART_STATUS CART_init(BTF* btf, void* arg);
/**
 * Executes the framework thread of the BTF object associated with the
 * thread_level. This API is usually invoked periodically at a rate specific to
 * the thread_level and the current BTF object specifications.
 */
uint32_t CART_exec(BTF* btf, CART_EXEC_ID thread_level, void* arg);
/**
 * API for sending commands to the BTF object
 */
CART_Cmd_Ret CART_command(BTF* btf, CART_Message* req, CART_Message* rsp);
/**
 * API for received command for the BTF object
 */
CART_Cmd_Ret CART_receive(BTF* btf, CART_Message* rsp);
/**
 * Returns 4 hex double digit version (32bit)
 */
char* CART_version(BTF* btf);


#ifdef __cplusplus
}
#endif


#endif /*!defined(EA_B0A5AFE0_B1D7_4a87_A223_AE166E6871F1__INCLUDED_)*/
 