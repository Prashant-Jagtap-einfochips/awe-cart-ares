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
 * @file Port_Direction.h
 * 
 * @ingroup Port_Direction
 *
 * @brief Implementation of the Enumeration Port_Direction       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Port_Direction__INCLUDED_)
#define BC_Port_Direction__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @ingroup Port_Direction
 *
 * @brief Enumeration of possible port direction
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */
typedef enum
{
	/**
	 * @ingroup Port_Direction
	 *
	 * @brief Input port
	 */
	PORT_DIRECTION_IN,

	/**
	 * @ingroup Port_Direction
	 *
	 * @brief Output port
	 */
	PORT_DIRECTION_OUT,

	/**
	 * @ingroup Port_Direction
	 *
	 * @brief Bidirectional port
	 */
	PORT_DIRECTION_IO
} Port_Direction;


#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Port_Direction__INCLUDED_)*/
