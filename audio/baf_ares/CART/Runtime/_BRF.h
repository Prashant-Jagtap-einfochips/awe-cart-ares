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

#if !defined(BC__BRF__INCLUDED_)
#define BC__BRF__INCLUDED_

#include "BRF.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "BRF_Resource.h"
/**
 * @ingroup _BRF
 *
 * @brief This is the internal structure that holds the Bose RoomModelDownload Framework (BRF) details.
 * This structure is not visible to the CART Integrator and is hidden through the BRF object.
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */
typedef struct _BRF
{
   CART_ID id;
   /**
    * @ingroup _BRF
    *
    * @brief Application context
    */
   void *ctx;
   BRF_Resource resource;
   CART_ID srcId;

   uint32_t bamfRespCount;
   uint32_t bamfNotifyCount;
   uint32_t bamfInvalidCount;
   uint32_t bamfSendCount;
} _BRF;

#ifdef __cplusplus
}
#endif


#endif /*!defined(BC__BRF__INCLUDED_)*/
