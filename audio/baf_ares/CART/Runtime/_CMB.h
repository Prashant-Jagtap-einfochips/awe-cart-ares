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
 * @file _CMB.h
 *
 * @ingroup _CMB
 *
 * @brief Declaration of CART Message Bus (CMB)
 *
 ****************************************************/

#if !defined(BC__CMB__INCLUDED_)
#define BC__CMB__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include "CMB.h"
#include "CART_ID.h"
#include "CMB_TYPES.h"
#include "CMB_Resource.h"
#include "CMB_AdaptorInterface.h"

typedef struct CMB_AdaptorProxyContext
{
    CART_ID destinationId[MAX_NUM_OF_DESTINATIONS];

    uint8_t numberOfDestinations;

} CMB_AdaptorProxyContext;


/**
 * @ingroup _CMB
 *
 * @brief This is the internal structure that holds the CMB details.
 * This structure is not visible to the CART Integrator and is hidden through the CMB object.
 *  
 * @version 1.0
 */
typedef struct
{
    uint8_t created;

    uint8_t initialized;

    // CART ID of the CMB.
    CART_ID id;

    // A structure to hold memory required by CMB.
    CMB_Resource resource;

    // Pointer to a list of adaptors added to the CMB.
    // This list in indexed by CMB_Adaptor_Id.
    CMB_AdaptorInterface* adaptors;

    // Number of adaptors added to the CMB.
    uint32_t adaptorCount;

    // A lookup table to find the adaptor responsible to handle a message for a given 
    // destination identified by CART ID. The tabled is indexed by CART_ID.
    // Its size should be fixed and equal to CART_ID_ALL enum.
    CMB_AdaptorInterface** adaptorTable;

    // The data needed by the adaptors.
    CMB_Context* context;
} _CMB;


#ifdef __cplusplus
}
#endif


#endif /*!defined(BC__CMB__INCLUDED_)*/
