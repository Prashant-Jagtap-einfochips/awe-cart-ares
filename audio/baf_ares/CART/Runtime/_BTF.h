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
 * @file _BAF.h
 *
 * @ingroup _BAF
 *
 * @brief Implementation of the Class _BAF
 *
 * @author: cb1037694
 ****************************************************/

#if !defined(BC__BTF__INCLUDED_)
#define BC__BTF__INCLUDED_

#include "BTF.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "BTF_Resource.h"
/**
 * @ingroup _BTF
 *
 * @brief This is the internal structure that holds the Bose Tuning Framework (BTF) details.
 * This structure is not visible to the CART Integrator and is hidden through the BTF object.
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */
typedef struct _BTF
{
   CART_ID id;
   /**
    * @ingroup _BTF
    *
    * @brief Application context
    */
   void *ctx;
   BTF_Resource resource;
   CART_ID srcId;

   uint32_t bamfRespCount;
   uint32_t bamfNotifyCount;
   uint32_t bamfInvalidCount;
   uint32_t bamfSendCount;
} _BTF;

#ifdef __cplusplus
}
#endif


#endif /*!defined(BC__BTF__INCLUDED_)*/
