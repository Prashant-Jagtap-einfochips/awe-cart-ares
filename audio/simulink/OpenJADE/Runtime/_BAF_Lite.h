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
 * @file _BAF_Lite.h
 *
 * @ingroup _BAF
 *
 * @brief Implementation of the Class _BAF
 *
 * @author: cb1037694
 ****************************************************/

#if !defined(BC__BAF_LITE__INCLUDED_)
#define BC__BAF_LITE__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include "Resource.h"

typedef struct _BAF_Lite
{
	uint16_t task_rate_factors[MODEL_SYNC_TASK_COUNT];
	uint16_t task_counter[MODEL_SYNC_TASK_COUNT];
	uint8_t task_state[MODEL_SYNC_TASK_COUNT];
	/**
	 * @ingroup _BAF
	 *
	 * @brief Static Resource object
	 */
	Resource resource;
	BAF_Func_Notify notification_handler;

}  _BAF_Lite;

#ifdef __cplusplus
}
#endif


#endif /*!defined(BC__BAF_LITE__INCLUDED_)*/
