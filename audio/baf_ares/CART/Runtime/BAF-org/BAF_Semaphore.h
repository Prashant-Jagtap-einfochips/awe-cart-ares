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
 * @file BAF_Semaphore.h
 * 
 * @ingroup BAF_Semaphore
 *
 * @brief Implementation of the Class BAF_Semaphore       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_BAF_Semaphore__INCLUDED_)
#define BC_BAF_Semaphore__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @ingroup BAF_Semaphore
 *
 * @brief This class represents the OS specific abstraction layer for the semaphore implementation.
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */
typedef void BAF_Semaphore;

/**
 * @ingroup BAF_Semaphore
 *
 * @brief Create and return pointer to the semaphore object, and initialize the semaphore counter to the
 * count value
 * 
 * @param this Pointer to semaphore object. NULL for now.
 * 
 * @param count Semaphore counter
 */
BAF_Semaphore* BAF_Semaphore_create(uint32_t count);

/**
 * @ingroup BAF_Semaphore
 *
 * @brief Increment semaphore counter.
 * 
 * @param sem Pointer to semaphore object
 */
void BAF_Semaphore_post(BAF_Semaphore* sem);

/**
 * @ingroup BAF_Semaphore
 *
 * @brief Suspend execution if the semaphore counter is zero. Decrement counter otherwise.
 * 
 * @param sem Pointer to semaphore object
 */
void BAF_Semaphore_pend(BAF_Semaphore* sem); 



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_BAF_Semaphore__INCLUDED_)*/
