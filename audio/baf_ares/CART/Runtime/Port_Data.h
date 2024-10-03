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
 * @file Port_Data.h
 * 
 * @ingroup Port_Data
 *
 * @brief Implementation of the Enumeration Port_Data       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Port_Data__INCLUDED_)
#define BC_Port_Data__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @ingroup Port_Data
 *
 * @brief Enumeration of the port data types
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */
typedef enum
{
	/**
	 * @ingroup Port_Data
	 *
	 * @brief 32-bit signed integer
	 */
	PORT_DATA_INT32,

	/**
	 * @ingroup Port_Data
	 *
	 * @brief 32-bit single precision float
	 */
	PORT_DATA_REAL32,

	/**
	 * @ingroup Port_Data
	 *
	 * @brief 24-bit integer
	 */
	PORT_DATA_INT24
} Port_Data;


#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Port_Data__INCLUDED_)*/
