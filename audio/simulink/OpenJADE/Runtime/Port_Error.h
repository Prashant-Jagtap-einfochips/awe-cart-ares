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
 * @file Port_Error.h
 * 
 * @ingroup Port_Error
 *
 * @brief Implementation of the Enumeration Port_Error       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Port_Error__INCLUDED_)
#define BC_Port_Error__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @ingroup Port_Error
 *
 * @brief Enumeration of possible port mismatching errors
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */
typedef enum
{
	/**
	 * @ingroup Port_Error
	 *
	 * @brief Success
	 */
	PORT_SUCCESS,

	/**
	 * @ingroup Port_Error
	 *
	 * @brief Different Frame Size
	 */
	PORT_ERROR_FRAME_SIZE,

	/**
	 * @ingroup Port_Error
	 *
	 * @brief Different Sample Size
	 */
	PORT_ERROR_SAMPLE_SIZE,

	/**
	 * @ingroup Port_Error
	 *
	 * @brief Different sampling rate
	 */
	PORT_ERROR_SAMPLING_RATE,

	/**
	 * @ingroup Port_Error
	 *
	 * @brief Different data type
	 */
	PORT_ERROR_DATA_TYPE,

	/**
	 * @ingroup Port_Error
	 *
	 * @brief Different formatting
	 */
	PORT_ERROR_FORMAT,

	/**
	 * @ingroup Port_Error
	 *
	 * @brief Can connect only in/out
	 */
	PORT_ERROR_DIRECTION
} Port_Error;


#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Port_Error__INCLUDED_)*/
