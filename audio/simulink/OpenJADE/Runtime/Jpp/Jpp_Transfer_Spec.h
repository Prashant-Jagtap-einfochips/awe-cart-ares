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
 * @file Jpp_Transfer_Spec.h
 * 
 * @ingroup Jpp_Transfer_Spec
 *
 * @brief Implementation of the Class Jpp_Transfer_Spec       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Jpp_Transfer_Spec__INCLUDED_)
#define BC_Jpp_Transfer_Spec__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


/**
 * @ingroup Jpp_Transfer_Spec
 *
 * @brief Data type returned from transfer buffer list access methods
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */
typedef struct Jpp_Transfer_Spec
{

	/**
	 * @ingroup Jpp_Transfer_Spec
	 *
	 * @brief size in bytes of the buffer to be transferred
	 */
	uint32_t byte_size;

	/**
	 * @ingroup Jpp_Transfer_Spec
	 *
	 * @brief pointer to the buffer location
	 */
	void* buffer;

	/**
	 * @ingroup Jpp_Transfer_Spec
	 *
	 * @brief number of core that owns the buffer
	 */
	uint32_t core_number;
}  Jpp_Transfer_Spec;



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Jpp_Transfer_Spec__INCLUDED_)*/
