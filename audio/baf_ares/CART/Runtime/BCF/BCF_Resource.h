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
 * @file BCF_Resource.h
 *
 * @ingroup BCF_Resource
 *
 * @brief Implementation of the Class BCF_Resource
 *
 * @author: cb1037694
 ****************************************************/

#if !defined(BC_BCF_Resource__INCLUDED_)
#define BC_BCF_Resource__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <CART.h>

#define BCF_RESOURCE_NUM_BAF 16

/**
 * @ingroup BCF_Resource
 *
 * @brief Structure describing resources used by the framework. After updating the resource structure, the 
 * Integrator should call the BCF_init() API.
 *
 * @author cb1037694
 *
 * @version 1.0
 */
typedef struct BCF_Resource
{
   CART_ID baf_id[BCF_RESOURCE_NUM_BAF];
   CART_ID brf_id;
   CART_ID app_id;
}  BCF_Resource;



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_BCF_Resource__INCLUDED_)*/
