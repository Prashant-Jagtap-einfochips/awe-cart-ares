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
 * @file Ports.h
 * 
 * @ingroup Ports
 *
 * @brief Implementation of the Class Ports       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Ports__INCLUDED_)
#define BC_Ports__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include "Port.h"


/**
 * @ingroup Ports
 *
 * @brief This structure is a container of Port items.
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */
typedef struct Ports
{
	/**
	 * @ingroup Ports
	 *
	 * @brief Number of Port items in the Ports container
	 */
	uint32_t num_items;

	/**
	 * @ingroup Ports
	 *
	 * @brief Variable length array (VLA) of Port items.
	 */
	Port port[
#ifdef OJ_NUM_PORT	
		OJ_NUM_PORT
#endif
	];
} Ports;

/**
 * @ingroup Ports
 *
 * @brief Helper API for comparing if two ports are compatible, so they can be connected. It tries to merge
 * the num_chan and frame_size of the two ports, if one of them is set to any (i.e. zero)
 * 
 * @param this
 * 
 * @param p
 * 
 * @param thisPorts
 * 
 * @param ports
 */
Port_Error Ports_compare_merge(Port* this, Port* p, Ports* thisPorts, Ports* ports); 



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Ports__INCLUDED_)*/
