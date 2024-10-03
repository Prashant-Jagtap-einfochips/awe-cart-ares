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
 * @file CMB.h
 *
 * @ingroup CMB
 *
 * @brief Declaration of CART Message Bus (CMB)
 *
 ****************************************************/

#if !defined(BC_CMB__INCLUDED_)
#define BC_CMB__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "CMB_Resource.h"
#include "CART_Cmd_Ret.h"
#include "CART.h"
#include "CMB_TYPES.h"
#include "CMB_AdaptorInterface.h"

/**
 * @ingroup CMB
 *
 * @brief 1. CART Message Bus (CMB)
 * 2. Processor and operating system independent Message Bus.
 * 3. Uses Adaptors a. for communicating between CART components b. to interface
 *    to other non-CART components (for tuning and testing for example AATMA).
 *
 * @author st1055616
 *
 * @version 1.0
 */

#define CMB_VERSION "00.00.07"

/**
 * @ingroup _CMB
 *
 * @brief This is the internal structure that holds the Bose Tuning Framework (BTF) details.
 * This structure is not visible to the CART Integrator and is hidden through the BTF object.
 */
typedef struct
{
   /**
    * @ingroup CMB
    *
    * @brief CART ID of the bus.
    */
    CART_ID id;

    /**
     * @ingroup CMB
     *
     * @brief Maximum number of adaptors that will be added to the bus.
     * Used to determine the memory required by the bus.
     */
     uint16_t maxNumOfAdaptors;
} CMB_Config;

/**
 * @ingroup _CMB
 *
 * @brief This structure is used to pass control related information to between
 * application and adaptors.
 */
typedef struct CMB_Context
{
    /**
     * @ingroup CMB
     *
     * @brief Pointer to the Message Bus in case the adaptor needs it in API
     * that do not have access to it.
     */
    CMB* cmb;
} CMB_Context;


/**
 * @ingroup CMB
 *
 * @brief This API returns a pointer to the CMB object corresponding to the
 * CART_ID provided as argument.
 *
 * @param cfg Configuration of the bus.
 */
CMB* CMB_create(const CMB_Config* cfg);

/**
 * @ingroup CMB
 *
 * @brief This API destroys the CMB object and CMB* should not be used after
 * this call.
 *
 * @param ccb Pointer to the CMB object
 */
void CMB_destroy(CMB* ccb);

/**
 * @ingroup CMB
 *
 * @brief Returns the resources required by the CMB object
 *
 * @param ccb Pointer to the CMB object
 */
CMB_Resource* CMB_resource(CMB* ccb);

/**
 * @ingroup CMB
 *
 * @brief This API initializes the CMB module
 *
 * @param ccb Pointer to the CMB object
 *
 * @param arg context pointer to be supplied
 */
CART_STATUS CMB_init(CMB* ccb, void* arg);

/**
 * @ingroup CMB
 *
 * @brief This API is used to send commands to CMB. Bus takes appropriate action
 *        including routing the command messages to adaptors as determined by
 *        the message type and CART header in the CART message passed into
 *        parameter inoutparams.
 *
 * @param msgtype BUS_COMMAND to be processed by the bus.
 *
 * @param inoutparams Pointer to data structure for input and out parameters
 *                    corresponding to the command to be executed by the bus.
 *
 */
CART_Cmd_Ret CMB_command(CMB* ccb, const CMB_COMMAND msgtype, void* inoutparams);

/**
 * @ingroup CMB
 *
 * @brief This API is used to add a adaptor that the CMB must use to communicate to
 * CART components defined in CMB_AdaptorInterface's destination list.
 *
 * @param ccb Pointer to the CMB object
 *
 * @param adaptor Adaptor interface to be added to the bus.
 *
 * @param id Adaptor identifier returned by the CMB to the application.
 *
 */
CART_STATUS CMB_add_adaptor(CMB* ccb, CMB_AdaptorInterface* adaptor, CMB_Adaptor_Id* id);

/**
 * @ingroup CMB
 *
 * @brief This API is used to get a handle to a CMB instance that has already been created
 * previously by call to CMB_create(). The handle is needed to call the remaining CMB APIs.
 *
 * @return Handle to CART Message bus to be passed to other CMB APIs.
 *
 */
CMB* CMB_get_handle();

/**
 * @ingroup CMB
 *
 * @brief This API is used get a handle to the adaptor that has already been added to the
 * bus to communicate to CART component identified by CART_ID parameter.
 *
 * @param ccb Pointer to the CMB object
 *
 * @param id CART ID of the destination (CART component) whose adaptor is required.
 *
 * @return Handle to the adaptor that CMB has been configured to send messages to CART_ID id.
 *
 */
// CMB_AdaptorInterface* CMB_get_adaptor(CMB* handle, const CART_ID id);


#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_CMB__INCLUDED_)*/
