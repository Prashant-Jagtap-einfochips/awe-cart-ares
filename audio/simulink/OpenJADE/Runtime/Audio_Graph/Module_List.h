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
 * @file Module_List.h
 * 
 * @ingroup Module_List
 *
 * @brief Implementation of the Class Module_List       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Module_List__INCLUDED_)
#define BC_Module_List__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "JModule_Bitfield.h"



/**
 * @ingroup Module_List
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef struct Module_List
{
	uint32_t num_items;
	Module_Id item[
#ifdef OJ_NUM_JMODULES	
		OJ_NUM_JMODULES
#endif
	];
}  Module_List;



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Module_List__INCLUDED_)*/
