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
 * @file Port_Context.h
 * 
 * @ingroup Port_Context
 *
 * @brief Implementation of the Class Port_Context       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Port_Context__INCLUDED_)
#define BC_Port_Context__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "Connection.h"


/**
 * @ingroup Port_Context
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef struct Port_Context
{
	uint32_t id;
	uint32_t num_buf;
	Connection* connection;
	void* payload;
//#if defined(BAF_CONNECTION)
	void* fifo[2];
//#endif
}  Port_Context;



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Port_Context__INCLUDED_)*/
