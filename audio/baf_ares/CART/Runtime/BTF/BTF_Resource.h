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
 * @file BTF_Resource.h
 *
 * @ingroup BTF_Resource
 *
 * @brief Implementation of the Class BTF_Resource
 *
 * @author: cb1037694
 ****************************************************/

#if !defined(BC_BTF_Resource__INCLUDED_)
#define BC_BTF_Resource__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <CART.h>

#define BTF_RESOURCE_NUM_BAF 16

/**
 * @ingroup BTF_Resource
 *
 * @brief Structure describing resources used by the framework. After updating the resource structure, the 
 * Integrator should call the BTF_init() API.
 *
 * @author cb1037694
 *
 * @version 1.0
 */
typedef struct BTF_Resource
{
   CART_ID baf_id[BTF_RESOURCE_NUM_BAF];
   CART_ID brf_id;
   CART_ID app_id;
}  BTF_Resource;



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_BTF_Resource__INCLUDED_)*/
