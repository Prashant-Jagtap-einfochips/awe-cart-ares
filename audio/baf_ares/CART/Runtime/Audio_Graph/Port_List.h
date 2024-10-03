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
 * @file Port_List.h
 * 
 * @ingroup Port_List
 *
 * @brief Implementation of the Class Port_List       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Port_List__INCLUDED_)
#define BC_Port_List__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "Port_Direction.h"

#include "Port_Item.h"


/**
 * @ingroup Port_List
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef struct Port_List
{
	uint32_t num_items;
	Port_Item item[
#ifdef OJ_NUM_PORTS	
		OJ_NUM_PORTS
#endif
	];
}  Port_List;



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Port_List__INCLUDED_)*/
