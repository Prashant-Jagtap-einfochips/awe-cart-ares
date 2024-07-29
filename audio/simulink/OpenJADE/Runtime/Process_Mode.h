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
 * @file Process_Mode.h
 * 
 * @ingroup Process_Mode
 *
 * @brief Implementation of the Enumeration Process_Mode       
 *
 * @author: Constantin Bajenaru                     
 ****************************************************/

#if !defined(BC_Process_Mode__INCLUDED_)
#define BC_Process_Mode__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @ingroup Process_Mode
 *
 * @brief Defines testing modes ids for processes in JModule
 * 
 * @author Constantin Bajenaru
 * 
 * @version 2.0
 */
typedef enum
{
	/**
	 * @ingroup Process_Mode
	 *
	 * @brief Process active id
	 */
	PROCESS_MODE_ACTIVE,

	/**
	 * @ingroup Process_Mode
	 *
	 * @brief Process in bypass mode
	 */
	PROCESS_MODE_BYPASS,

	/**
	 * @ingroup Process_Mode
	 *
	 * @brief Process output muted
	 */
	PROCESS_MODE_MUTE,

	/**
	 * @ingroup Process_Mode
	 *
	 * @brief Process output generates signal
	 */
	PROCESS_MODE_GEN,

	/**
	 * @ingroup Process_Mode
	 *
	 * @brief Process mode mask. 
	 */

	PROCESS_MODE_MASK = 7,
	/**
	 * @ingroup Process_Mode
	 *
	 * @brief Process input capture. Must be power of 2, greater than the previous
	 */
	PROCESS_MODE_CAPTURE_IN = 8,
	/**
	 * @ingroup Process_Mode
	 *
	 * @brief Process output capture. Must be power of 2, greater than the previous 
	 */
	PROCESS_MODE_CAPTURE_OUT = 16
} Process_Mode;


#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Process_Mode__INCLUDED_)*/
