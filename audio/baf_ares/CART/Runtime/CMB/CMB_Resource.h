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
 * @file CMB_Resource.h
 *
 * @ingroup CMB_Resource
 *
 * @brief Declaration of the CMB_Resource
 *
 * @author: st1055616
 ****************************************************/

#if !defined(BC_CMB_Resource__INCLUDED_)
#define BC_CMB_Resource__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "CART_ID.h"
#include "CART.h"


/**
 * @ingroup CMB_Resource
 *
 * @brief Structure describing resources used by the framework. After updating the resource structure, the
 * Integrator should call the CMB_init() API.
 *
 * @author st1055616
 *
 * @version 1.0
 */
typedef struct CMB_Resource
{
    /**
     * @ingroup CMB_Resource
     *
     * @brief Memory size, in bytes, required by the CCD heap. It is returned by
     *  the CMB_resource() API to the system integrator.
     */
    uint32_t memorySize;

    /**
     * @ingroup CMB_Resource
     *
     * @brief Pointer to the memory used by the CMB heap. Customer Integrator
     * should initialize this to a memory area atleast memorySize in size.
     */
    void* memoryPointer;

    /**
     * @ingroup CMB_Resource
     *
     * @brief The maximum number of adaptors need to be supported by this
     * control bus. System integrator provides this during bus configuration.
     */
    uint16_t maxAdaptors;

} CMB_Resource;


#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_CMB_Resource__INCLUDED_)*/
