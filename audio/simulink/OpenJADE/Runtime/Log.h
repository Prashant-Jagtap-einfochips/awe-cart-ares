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
 * @file Log.h
 * 
 * @ingroup Log
 *
 * @brief Implementation of the Class Log       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Log__INCLUDED_)
#define BC_Log__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


/**
 * @ingroup Log
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef struct Log_record
{
	uint8_t msg[2];
	uint8_t sender_id;
	uint8_t arg_byte;
	uint32_t arg_word;
}  Log_record;

typedef union {
  Log_record record;
  float flt[2];
  uint32_t word[2];
} Log;

#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Log__INCLUDED_)*/
