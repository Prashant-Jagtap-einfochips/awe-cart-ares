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
 * @file Execution_List.h
 * 
 * @ingroup Execution_List
 *
 * @brief Implementation of the Class Execution_List       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Execution_List__INCLUDED_)
#define BC_Execution_List__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "Framework_Param.h"
#include "JModule_Bitfield.h"


/**
 * @ingroup Execution_List
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef struct Execution_List
{
	uint32_t num_items;
	Process_Id item[
#ifdef OJ_NUM_PROCESSES	
		OJ_NUM_PROCESSES
#endif
	];
}  Execution_List;



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Execution_List__INCLUDED_)*/
