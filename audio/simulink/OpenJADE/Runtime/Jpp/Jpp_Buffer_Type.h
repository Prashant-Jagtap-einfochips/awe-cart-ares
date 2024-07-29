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
 * @file Jpp_Buffer_Type.h
 * 
 * @ingroup Jpp_Buffer_Type
 *
 * @brief Implementation of the Enumeration Jpp_Buffer_Type       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Jpp_Buffer_Type__INCLUDED_)
#define BC_Jpp_Buffer_Type__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @ingroup Jpp_Buffer_Type
 *
 * @brief Enumeration describing the different buffer types
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */
typedef enum
{
	/**
	 * @ingroup Jpp_Buffer_Type
	 *
	 * @brief Buffer originating on this core and received by another core on this same chip
	 */
	JPP_BUFFER_TYPE_LOCAL_SRC,

	/**
	 * @ingroup Jpp_Buffer_Type
	 *
	 * @brief Buffer received by this core and originating from another core on this same chip.
	 */
	JPP_BUFFER_TYPE_LOCAL_DST,

	/**
	 * @ingroup Jpp_Buffer_Type
	 *
	 * @brief Buffer originating on this chip (on any core) and received by any core on a different chip.
	 */
	JPP_BUFFER_TYPE_CROSS_SRC,

	/**
	 * @ingroup Jpp_Buffer_Type
	 *
	 * @brief Buffer received by this chip (on any core) and originating from any core on a different chip.
	 */
	JPP_BUFFER_TYPE_CROSS_DST
} Jpp_Buffer_Type;


#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Jpp_Buffer_Type__INCLUDED_)*/
