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
 * @file Semaphore.h
 * 
 * @ingroup Semaphore
 *
 * @brief Implementation of the Class Semaphore       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Semaphore__INCLUDED_)
#define BC_Semaphore__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#ifdef USE_BAF_SEMAPHORE
// Platform semaphore interface
#include "BAF_Semaphore.h"
#else
// Local semaphore mockup

/**
 * @ingroup BAF_Semaphore
 */
typedef struct BAF_Semaphore
{
	uint32_t count;
} BAF_Semaphore;

/**
 * @ingroup BAF_Semaphore
 *
 * @brief @param this
 * 
 * @param count
 */
BAF_Semaphore* BAF_Semaphore_create(uint32_t count);

/**
 * @ingroup BAF_Semaphore
 *
 * @brief @param this
 */
void BAF_Semaphore_post(BAF_Semaphore* this);

/**
 * @ingroup Semaphore
 *
 * @brief @param this
 */
void BAF_Semaphore_pend(BAF_Semaphore* this); 

#endif
#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Semaphore__INCLUDED_)*/
