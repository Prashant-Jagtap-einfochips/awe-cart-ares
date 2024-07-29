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
 * @file BAF_Message_Id.h
 * 
 * @ingroup BAF_Message_Id
 *
 * @brief Implementation of the Enumeration BAF_Message_Id       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_BAF_Message_Id__INCLUDED_)
#define BC_BAF_Message_Id__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @ingroup BAF_Message_Id
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef enum
{
	/**
	 * @ingroup BAF_Message_Id
	 *
	 * @brief 0x00 - Passthru message to specific JModule (first in the Module_List_obj, by default)
	 */
	BAF_MESSAGE_PASSTHRU_SINGLE,

	/**
	 * @ingroup BAF_Message_Id
	 *
	 * @brief 0x01 - Passthru message to all JModules
	 */
	BAF_MESSAGE_PASSTHRU_BROADCAST,

	/**
	 * @ingroup BAF_Message_Id
	 *
	 * @brief 0x02 - Polling message. Invokes the poll API for all JModules
	 */
	BAF_MESSAGE_NOTIFICATION_POLL,

	/**
	 * @ingroup BAF_Message_Id
	 *
	 * @brief 0x03 - Id for notification response, received by the notification callback
	 */
	BAF_MESSAGE_NOTIFICATION,

	/**
	 * @ingroup BAF_Message_Id
	 *
	 * @brief 0x04 - Message querry
	 */
	BAF_MESSAGE_VERSION,

	/**
	 * @ingroup BAF_Message_Id
	 *
	 * @brief 0x05 - Passthru message to specific JModule (first in the Module_List_obj, by default)
	 *               BAF_command() returns 0 for this message, but the response buffer needs to be allocated
     */
	BAF_MESSAGE_PASSTHRU_SINGLE_BLIND,

	/**
	 * @ingroup BAF_Message_Id
	 *
	 * @brief 0x06 - Passthru message to all JModules
	 *               BAF_command() returns 0 for this message, but the response buffer needs to be allocated
	 */
	BAF_MESSAGE_PASSTHRU_BROADCAST_BLIND,

	/**
	 * @ingroup BAF_Message_Id
	 *
	 * @brief 0x07 - Add stat monitoring variable of certain size
	 * 
	 * 				 31   16 15      12 11      8 7           0
	 *               ------------------------------------------
	 * 				 | size | not used | stat id |     0x7    |
	 *               ------------------------------------------
	 *               BAF_command() returns error if id is out of range
	 */
	BAF_MESSAGE_ADD_STAT,

	/**
	 * @ingroup BAF_Message_Id
	 *
	 * @brief 0x08 - Return stat monitoring content
	 * 				 31     16 15  12 11      8 7           0
	 *               ----------------------------------------
	 * 				 | offset | size | stat id |     0x8    |
	 *               ----------------------------------------
	 *               BAF_command() returns error if id or size is out of range
	 * 				 The message response points to the monitored variable at the requested offset
	 */
	BAF_MESSAGE_GET_STAT
} BAF_Message_Id;


#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_BAF_Message_Id__INCLUDED_)*/
