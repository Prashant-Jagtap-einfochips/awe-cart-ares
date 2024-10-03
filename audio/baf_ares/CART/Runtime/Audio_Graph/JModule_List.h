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
 * @file JModule_List.h
 * 
 * @ingroup JModule_List
 *
 * @brief Implementation of the Class JModule_List       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_JModule_List__INCLUDED_)
#define BC_JModule_List__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "JModule.h"
#include "JModule_Id.h"


/**
 * @ingroup JModule_List
 *
 * @brief JModule_List element order should match JModule_Id
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */
typedef struct JModule_List
{
	int32_t num_items;
	JModule* item[
#ifdef OJ_NUM_JMODULES	
		OJ_NUM_JMODULES
#endif
	];
} JModule_List;


#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_JModule_List__INCLUDED_)*/
