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
 * @file Message_Id.h
 * 
 * @ingroup Message_Id
 *
 * @brief Implementation of the Enumeration Message_Id       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Message_Id__INCLUDED_)
#define BC_Message_Id__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @ingroup Message_Id
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef enum
{
	/**
	 * @ingroup Message_Id
	 *
	 * @brief 0x00 - Passthru message to specific JModule (first in the Module_List_obj, by default)
	 */
	MESSAGE_PASSTHRU_SINGLE,

	/**
	 * @ingroup Message_Id
	 *
	 * @brief 0x01 - Passthru message to all JModules
	 */
	MESSAGE_PASSTHRU_BROADCAST,

	/**
	 * @ingroup Message_Id
	 *
	 * @brief 0x02 - Polling message. Invokes the poll API for all JModules
	 */
	MESSAGE_NOTIFICATION_POLL,

	/**
	 * @ingroup Message_Id
	 *
	 * @brief 0x03 - Id for notification response, received by the notification callback
	 */
	MESSAGE_NOTIFICATION,

	/**
	 * @ingroup Message_Id
	 *
	 * @brief 0x04 - Version query
	 */
	MESSAGE_VERSION,

	/**
	 * @ingroup BAF_Message_Id
	 *
	 * @brief 0x05 - Passthru message to specific JModule (first in the Module_List_obj, by default)
	 *               BAF_command() returns 0 for this message, but the response buffer needs to be allocated
	 */
	MESSAGE_PASSTHRU_SINGLE_BLIND,

	/**
	 * @ingroup BAF_Message_Id
	 *
	 * @brief 0x06 - Passthru message to all JModules
	 *               BAF_command() returns 0 for this message, but the response buffer needs to be allocated
	 */
	MESSAGE_PASSTHRU_BROADCAST_BLIND,

	/**
	 * @ingroup BAF_Message_Id
	 *
	 * @brief 0x07 - Add stat monitoring variable of certain size
	 * 
	 * 				 31   16 15      12 11      8 7           0
	 *               ----------------------------------------
	 * 				 | size | not used | stat id |     0x7    |
	 *               ----------------------------------------
	 *               BAF_command() returns error if id is out of range
	 */
	MESSAGE_ADD_STAT,

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
	MESSAGE_GET_STAT,

	/**
	 * @ingroup Message_Id
	 *
	 * @brief 0x80 - SYS messages start tag
	 *               BAF_command() returns 0 for this message, but the response buffer needs to be allocated
	 */
	MESSAGE_SYS_START = 128,

	/**
	 * @ingroup Message_Id
	 *
	 * @brief 0x81 - Dump log messages using printf, if implemented
	 */
	MESSAGE_SYS_LOG_DUMP,

	/**
	 * @ingroup Message_Id
	 *
	 * @brief 0x82 - Set logger to circular mode
	 */
	MESSAGE_SYS_LOG_CIRCULAR,

	/**
	 * @ingroup Message_Id
	 *
	 * @brief 0x83 - Set logger to linear mode. Logging will stop after buffer is full
	 */
	MESSAGE_SYS_LOG_LINEAR,

	/**
	 * @ingroup Message_Id
	 *
	 * @brief 0x84 - Enable (unfreeze) logging
	 */
	MESSAGE_SYS_LOG_ENABLE,

	/**
	 * @ingroup Message_Id
	 *
	 * @brief 0x85 - Disable (freeeze) logging
	 */
	MESSAGE_SYS_LOG_DISABLE,

	/**
	 * @ingroup Message_Id
	 *
	 * @brief 0x86 - Dump framework content using printf, if implemented
	 */
	MESSAGE_SYS_LOG_FRAMEWORK,

	/**
	 * @ingroup Message_Id
	 *
	 * @brief 0x87 - place message into the logger
	 */
	MESSAGE_SYS_LOG_PRINT,

	/**
	 * @ingroup Message_Id
	 *
	 * @brief 0x88 - Pull data record from the specific log
	 */
	MESSAGE_SYS_LOG_PULL,

	/**
	 * @ingroup Message_Id
	 *
	 * @brief 0x89 - Input port capture
	 */
	MESSAGE_SYS_CAPTURE_IN,

	/**
	 * @ingroup Message_Id
	 *
	 * @brief 0x8A - Output port capture
	 */
	MESSAGE_SYS_CAPTURE_OUT,

	/**
	 * @ingroup Message_Id
	 *
	 * @brief 0x8B  - Mute Process0 outputs of specific JModule
	 */
	MESSAGE_SYS_MUTE_SINGLE,

	/**
	 * @ingroup Message_Id
	 *
	 * @brief 0x8C - Copy Process0, Port0 input to output of specific JModule. If input buffer is smaller than
	 * output buffer, the input is repeated.
	 */
	MESSAGE_SYS_BYPASS_SINGLE,

	/**
	 * @ingroup Message_Id
	 *
	 * @brief 0x8D - Activate (run JModule process API) of the specific JModule. Processes are all active by
	 * default
	 */
	MESSAGE_SYS_ACTIVE_SINGLE,

	/**
	 * @ingroup Message_Id
	 *
	 * @brief 0x8E - Place internal wave generator at the Process0 outputs of specific JModule
	 */
	MESSAGE_SYS_GEN_SINGLE,

	/**
	 * @ingroup Message_Id
	 *
	 * @brief 0x8F - Stop processing after NUM_FRAMES. Format {NUM_FRAMES - 24 bit, 0x90 - 8 bit}
	 */
	MESSAGE_SYS_NUM_FRAMES,


	/**
	 * @ingroup Message_Id
	 *
	 * @brief SYS messages end tag
	 */
	MESSAGE_SYS_END
} Message_Id;


#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Message_Id__INCLUDED_)*/
