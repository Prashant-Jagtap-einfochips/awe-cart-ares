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
 * @file Jpp_Specification.h
 * 
 * @ingroup Jpp_Specification
 *
 * @brief Implementation of the Class Jpp_Specification       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Jpp_Specification__INCLUDED_)
#define BC_Jpp_Specification__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


/**
 * @ingroup Jpp_Specification
 *
 * @brief Ping Pong Control Table Record Type
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */
typedef struct Jpp_Specification
{

	/**
	 * @ingroup Jpp_Specification
	 *
	 * @brief constant byte size of a buffer (the entire size of either ping or pong)
	 */
	uint32_t buffer_size;

	/**
	 * @ingroup Jpp_Specification
	 *
	 * @brief constant byte size of how many bytes to transfer per base rate step (i.e. buffer_size / swap_factor)
	 */
	uint32_t transfer_size;

	/**
	 * @ingroup Jpp_Specification
	 *
	 * @brief constant that determines ping-pong rate based on the model base rate (step0)
	 */
	uint32_t swap_factor;

	/**
	 * @ingroup Jpp_Specification
	 *
	 * @brief (constant pointer) to the pointer used by the model for buffer access
	 */
	void** model_ptr;

	/**
	 * @ingroup Jpp_Specification
	 *
	 * @brief address of current frame's transfer buffer
	 */
	void* transfer_address;
}  Jpp_Specification;



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Jpp_Specification__INCLUDED_)*/
