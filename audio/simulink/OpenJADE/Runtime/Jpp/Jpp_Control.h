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
 * @file Jpp_Control.h
 * 
 * @ingroup Jpp_Control
 *
 * @brief Implementation of the Class Jpp_Control       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Jpp_Control__INCLUDED_)
#define BC_Jpp_Control__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


/**
 * @ingroup Jpp_Control
 *
 * @brief Dynamic counters that update every frame and
 * constant buffer pointers to avoid extra address math
 * (and to share with cross chip master)
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */
typedef struct Jpp_Control
{

	/**
	 * @ingroup Jpp_Control
	 *
	 * @brief counts up how many base rate periods have elapsed since the last swap
	 */
	uint16_t swap_count;

	/**
	 * @ingroup Jpp_Control
	 *
	 * @brief index into which buffer is accessed for transfers (i.e. *not* the active buffer for model)
	 */
	uint16_t transfer_index;

	/**
	 * @ingroup Jpp_Control
	 *
	 * @brief constant pointers to ping and pong storage on this core
	 */
	void* buffers[2];
}  Jpp_Control;



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Jpp_Control__INCLUDED_)*/
