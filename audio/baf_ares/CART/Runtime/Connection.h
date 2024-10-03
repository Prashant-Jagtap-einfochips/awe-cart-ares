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
 * @file Connection.h
 * 
 * @ingroup Connection
 *
 * @brief Implementation of the Class Connection       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Connection__INCLUDED_)
#define BC_Connection__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "Framework_Param.h"
#include "JSemaphore.h"

/**
 * @ingroup Connection
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef struct Connection
{
	uint32_t id;
	uint32_t in_start;
	uint32_t out_start;
	BAF_Semaphore* full;
	BAF_Semaphore* empty;
	uint32_t in_count;
	uint32_t out_count;
} Connection;

/**
 * @ingroup Connection
 *
 * @brief @param this
 * 
 * @param in_start
 * 
 * @param out_start
 */
bool Connection_create(Connection* this, uint32_t in_start, uint32_t out_start);

/**
 * @ingroup Connection
 *
 * @brief @param this
 * 
 * @param buf[]
 */
void* Connection_receive(Connection* this, void* buf[]);

/**
 * @ingroup Connection
 *
 * @brief @param this
 */
void Connection_send(Connection* this);

/**
 * @ingroup Connection
 *
 * @brief @param this
 * 
 * @param buf[]
 */
void* Connection_alloc(Connection* this, void* buf[]);

/**
 * @ingroup Connection
 *
 * @brief @param this
 */
void Connection_free(Connection* this);

/**
 * @ingroup Connection
 *
 * @brief @param this
 */
void Connection_fill(Connection* this);

/**
 * @ingroup Connection
 *
 * @brief @param this
 */
void Connection_empty(Connection* this); 



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Connection__INCLUDED_)*/
