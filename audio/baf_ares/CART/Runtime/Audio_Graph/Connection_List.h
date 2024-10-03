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
 * @file Connection_List.h
 * 
 * @ingroup Connection_List
 *
 * @brief Implementation of the Class Connection_List       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Connection_List__INCLUDED_)
#define BC_Connection_List__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "JModule_Bitfield.h"
#include "Port_Direction.h"

#include "Connection_Item.h"


/**
 * @ingroup Connection_List
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef struct Connection_List
{
	uint32_t num_items;
	Connection_Item item[
#ifdef OJ_NUM_CONNECTIONS	
		OJ_NUM_CONNECTIONS
#endif
	];
} Connection_List;

/**
 * @ingroup Connection_List
 *
 * @brief @param connection_list
 * 
 * @param connection_id
 */
Connection_Item* Connection_List_get_connection(Connection_List* connection_list, uint32_t connection_id); 



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Connection_List__INCLUDED_)*/
