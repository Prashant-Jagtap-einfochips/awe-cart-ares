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
 * @file Error_Id.h
 * 
 * @ingroup Error_Id
 *
 * @brief Implementation of the Enumeration Error_Id       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Error_Id__INCLUDED_)
#define BC_Error_Id__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @ingroup Error_Id
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef enum
{
	SUCCESS = 0,
	ERROR_OUT_OF_MEMORY = -127,
	ERROR_BAD_ALIGNMENT,
	ERROR_JMODULE_NOT_FOUND,
	ERROR_INVALID_MODULE,
	ERROR_PORT_NOT_FOUND,
	ERROR_INDEX_OUT_OF_RANGE,
	ERROR_BAD_CALL_INTERVAL,
	ERROR_MODULE_TOO_MANY,
	ERROR_EXEC_TOO_MANY,
	ERROR_BUFFER_TOO_MANY,
	ERROR_BAD_CONNECTION,
	ERROR_MODULE_NULL,
	ERROR_INVALID_LOG,
	ERROR_ENABLED_LOG,
	ERROR_BUFFER_TOO_SMALL,
	ERROR_BUFFER_NULL,
	WARNING_PORT_NOT_CONNECTED,
	ERROR_INVALID_COMMAND
} Error_Id;


#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Error_Id__INCLUDED_)*/
