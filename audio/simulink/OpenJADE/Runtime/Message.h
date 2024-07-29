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
 * @file Message.h
 * 
 * @ingroup Message
 *
 * @brief Implementation of the Class Message       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Message__INCLUDED_)
#define BC_Message__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "Framework_Param.h"


/**
 * @ingroup Message
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef struct Message
{
	int32_t module_id;
	int32_t submodule_id;
	int32_t message_id;
	int32_t message_size;
	void* message_data;
}  Message;



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Message__INCLUDED_)*/
