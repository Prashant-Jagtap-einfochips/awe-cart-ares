/****************************************************
 *  BRF_Api.h                                         
 *  Created on: 03-Mar-2023 8:55:11 AM                      
 *  Implementation of the Class BRF_Api       
 *  Original author: cb1037694                     
 ****************************************************/

#if !defined(EA_B0A5AFE0_B1D7_4a87_A223_AE166E6871F1__INCLUDED_)
#define EA_B0A5AFE0_B1D7_4a87_A223_AE166E6871F1__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "BRF_Callback_Func.h"
#include "BRF_Callback.h"
#include "BRF.h"

/**
 * This class provides the Bose Control Framework (BRF) abstract definition and
 * implements the BRF APIs. This class statically instantiates the current BRF
 * object. The implementation will not be visible to the Customer Integrator
 */

/**
 * Returns a pointer to the current BRF object
 */
BRF* CART_create(CART_ID id);
/**
 * Returns the resources required by the BRF object input parameter
 */
BRF_Resource* CART_resource(BRF* brf);
/**
 * Initializes the BRF object. After ths API is called the real-time execution of
 * the framework can start. As pre-requisite, the resource structure returned by
 * the BRF_resource() API needs to be properly updated. 
 */
CART_STATUS CART_init(BRF* brf, void* arg);
/**
 * Executes the framework thread of the BRF object associated with the
 * thread_level. This API is usually invoked periodically at a rate specific to
 * the thread_level and the current BRF object specifications.
 */
uint32_t CART_exec(BRF* brf, CART_EXEC_ID thread_level, void* arg);
/**
 * API for sending commands to the BRF object
 */
CART_Cmd_Ret CART_command(BRF* brf, CART_Message* req, CART_Message* rsp);
/**
 * API for received command for the BRF object
 */
CART_Cmd_Ret CART_receive(BRF* brf, CART_Message* rsp);
/**
 * Returns 4 hex double digit version (32bit)
 */
char* CART_version(BRF* brf);


#ifdef __cplusplus
}
#endif


#endif /*!defined(EA_B0A5AFE0_B1D7_4a87_A223_AE166E6871F1__INCLUDED_)*/
 