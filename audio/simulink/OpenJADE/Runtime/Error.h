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
 * @file Error.h
 * 
 * @ingroup Error
 *
 * @brief Implementation of the Class Error       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Error__INCLUDED_)
#define BC_Error__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>

#include "Error_Id.h"


/**
 * @ingroup Error
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef struct Error
{
	Error_Id id;
	uint32_t sender_id;
	const char* msg;
	uint32_t arg1;
	uint32_t arg2;
} Error;

/**
 * @ingroup Error
 *
 * @brief @param this
 * 
 * @param sender_id
 * 
 * @param error_id
 * 
 * @param msg
 * 
 * @param arg1
 * 
 * @param arg2
 */
void Error_print(Error* this, uint32_t sender_id, Error_Id error_id, const char* msg, uint32_t arg1, uint32_t arg2); 



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Error__INCLUDED_)*/
