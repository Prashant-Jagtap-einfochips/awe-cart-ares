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
 * @file BAF_Message.h
 * 
 * @ingroup BAF_Message
 *
 * @brief Implementation of the Class BAF_Message       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_BAF_Message__INCLUDED_)
#define BC_BAF_Message__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


/**
 * @ingroup BAF_Message
 *
 * @brief Structure describing the message format used for storing the command()  request and response.
 * Because the data member is of variable size, the message has to be initialized when defined.
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */
typedef struct BAF_Message
{

	/**
	 * @ingroup BAF_Message
	 *
	 * @brief Message identifier
	 */
	uint32_t id;

	/**
	 * @ingroup BAF_Message
	 *
	 * @brief Size of data payload in 32-bit words
	 */
	uint32_t size;

	/**
	 * @ingroup BAF_Message
	 *
	 * @brief Data payload
	 */
	void *data;
}  BAF_Message;



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_BAF_Message__INCLUDED_)*/
