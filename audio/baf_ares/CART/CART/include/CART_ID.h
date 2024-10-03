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
 * @file CART_ID.h
 * 
 * @ingroup CART_ID
 *
 * @brief Implementation of the Enumeration CART_ID       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_CART_ID__INCLUDED_)
#define BC_CART_ID__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @ingroup CART_ID
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef enum
{
    CART_ID_INVALID     = 0x01,
	CART_ID_BUS 	    = 0x80,
	CART_ID_APP         = 0x90,
    CART_ID_CONTROL_APP = 0x91,
    CART_ID_BVP_APP     = 0x92,
    CART_ID_TUNING_APP  = 0x93,
    // Audio Control Service.
    CART_ID_ACS         = CART_ID_APP,
    CART_ID_HEAD_UNIT   = CART_ID_ACS,
	CART_ID_PLATFORM    = 0xA0,
	CART_ID_BCF         = 0xC0,
	CART_ID_BCF1        = 0xC0,
	CART_ID_BCF_ALL     = 0xCF,
	CART_ID_BAF         = 0xB0,
	CART_ID_BAF1        = 0xB0,
	CART_ID_BAF2        = 0xB1,
	CART_ID_BAF3        = 0xB2,
	CART_ID_BAF4        = 0xB3,
	CART_ID_BAF5        = 0xB4,
	CART_ID_BAF6        = 0xB5,
	CART_ID_BAF7        = 0xB6,
	CART_ID_BAF8        = 0xB7,
	CART_ID_BAF9        = 0xB8,
	CART_ID_BAF10       = 0xB9,
	CART_ID_BAF11       = 0xBA,
	CART_ID_BAF12       = 0xBB,
	CART_ID_BAF13       = 0xBC,
	CART_ID_BAF14       = 0xBD,
	CART_ID_BAF15       = 0xBE,
	CART_ID_BAF_ALL     = 0xBF,
	CART_ID_BRF         = 0xD0,
	CART_ID_BRF_ALL     = 0xDF,
	CART_ID_BTF         = 0xE0,
    CART_ID_BTF1        = 0xE0,
    CART_ID_BTF2        = 0xE1,
	CART_ID_BTF_ALL     = 0xEF,
	CART_ID_ALL         = 0xFF
} CART_ID;


#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_CART_ID__INCLUDED_)*/
