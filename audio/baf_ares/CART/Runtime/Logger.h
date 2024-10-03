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
 * @file Logger.h
 *
 * @ingroup Logger
 *
 * @brief Implementation of the Class Logger
 *
 * @author: cb1037694
 ****************************************************/

#if !defined(BC_Logger__INCLUDED_)
#define BC_Logger__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "Log.h"
#include "Heap.h"


/**
 * @ingroup Logger
 *
 * @brief @author cb1037694
 *
 * @version 1.0
 */
typedef struct Logger
{
	Heap heap;
	Log* pull;
	Log* start;
	Log* end;
	Log* current;
	bool wrapped;
	bool circular;
	bool enabled;
} Logger;

typedef enum
{
	LOG_TYPE_PRINT  ,
	LOG_TYPE_CAPTURE,
	LOG_TYPE_EXEC   ,
} Logger_Type;

/**
 * @ingroup Logger
 *
 * @brief @param this
 *
 * @param sender_id
 *
 * @param msg
 *
 * @param arg1
 *
 * @param arg2
 */
void Logger_print(Logger_Type id, uint32_t sender_id, const char* msg, uint32_t arg1, uint32_t arg2);

/**
 * @ingroup Logger
 *
 * @brief @param this
 *
 * @param thread_level
 *
 * @param process_id
 *
 * @param start_stop
 */
void Logger_stats(Logger_Type id, uint32_t thread_level, uint32_t process_id, uint32_t start_stop);

/**
 * @ingroup Logger
 *
 * @brief @param this
 *
 * @param sender_id
 *
 * @param msg
 *
 * @param arg1
 */
void Logger_print1(Logger_Type id, uint32_t sender_id, const char* msg, uint32_t arg1);

/**
 * @ingroup Logger
 *
 * @brief @param this
 *
 * @param sender_id
 *
 * @param msg
 */
void Logger_msg(Logger_Type id, uint32_t sender_id, const char* msg);

/**
 * @ingroup Logger
 *
 * @brief @param this
 *
 * @param heap
 *
 * @param circular
 */
void Logger_init(Logger_Type id, bool circular);

/**
 * @ingroup Logger
 *
 * @brief @param this
 *
 * @param buf
 *
 * @param size
 */
void Logger_copy(Logger_Type id, void* buf, uint32_t size);/**
 * @ingroup Logger
 *
 * @brief @param this
 *
 * @param heap
 *
 * @param circular
 */


/**
 * @ingroup Logger
 *
 * @brief @param this
 */
void Logger_dump(Logger_Type id);

void Logger_reset(Logger_Type id);

uint32_t Logger_pull(Logger_Type id, uint32_t *size, uint32_t* data );

extern Logger BAF_logger[];


#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Logger__INCLUDED_)*/
