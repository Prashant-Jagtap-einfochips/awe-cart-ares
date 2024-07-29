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
 * @file Jpp_Off_Chip_Spec.h
 * 
 * @ingroup Jpp_Off_Chip_Spec
 *
 * @brief Implementation of the Class Jpp_Off_Chip_Spec       
 *
 * @author: Constantin Bajenaru                     
 ****************************************************/

#if !defined(BC_Jpp_Off_Chip_Spec__INCLUDED_)
#define BC_Jpp_Off_Chip_Spec__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


/**
 * @ingroup Jpp_Off_Chip_Spec
 *
 * @brief Internal Off Chip transfer list type
 * 
 * @author Constantin Bajenaru
 * 
 * @version 1.0
 */
typedef struct Jpp_Off_Chip_Spec
{

	/**
	 * @ingroup Jpp_Off_Chip_Spec
	 *
	 * @brief constant global buffer number associated with this transfer
	 */
	uint8_t buffer_number;

	/**
	 * @ingroup Jpp_Off_Chip_Spec
	 *
	 * @brief constant core number relative to local chip
	 */
	uint8_t core_number;
}  Jpp_Off_Chip_Spec;



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Jpp_Off_Chip_Spec__INCLUDED_)*/
