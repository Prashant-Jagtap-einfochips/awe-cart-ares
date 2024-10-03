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
 * @file CART_MESSAGE_ID.h
 *
 * @ingroup CART_MESSAGE_ID
 *
 * @brief Implementation of the Enumeration CART_MESSAGE_ID
 *
 * @author: cb1037694
 ****************************************************/

#if !defined(BC_CART_MESSAGE_ID__INCLUDED_)
#define BC_CART_MESSAGE_ID__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @ingroup CART_MESSAGE_ID
 *
 * @brief Message Identifiers for CART commands.
 *
 * @author cb1037694
 *
 * @version 1.0
 */
typedef enum
{
	/**
	 * @ingroup CART_MESSAGE_ID
	 *
	 * @brief Default message for the CART module
	 */
	CART_MESSAGE_ID_DEFAULT,

	/**
	 * @ingroup CART_MESSAGE_ID
	 *
	 * @brief Broadcast default. Applicable to BAF for sending the message to all JModules
	 */
	CART_MESSAGE_ID_ALL,

	/**
	 * @ingroup CART_MESSAGE_ID
	 *
	 * @brief Version request
	 */
	CART_MESSAGE_ID_VERSION,

	/**
	 * @ingroup CART_MESSAGE_ID
	 *
	 * @brief Notification poll message
	 */
	CART_MESSAGE_ID_NOTIFICATION_POLL,
	
   /**
	 * @ingroup CART_MESSAGE_ID
	 *
	 * @brief Debug request
	 */
	CART_MESSAGE_ID_DEBUG,

	/**
	 * @ingroup CART_MESSAGE_ID
	 *
	 * @brief BDP format. Usually, applicable to BTF
	 */
	CART_MESSAGE_ID_BDP,

	/**
	 * @ingroup CART_MESSAGE_ID
	 *
	 * @brief Talaria payload. Usually, applicable to BAF
	 */
	CART_MESSAGE_ID_TALARIA,

	/**
	 * @ingroup CART_MESSAGE_ID
	 *
	 * @brief BCP Payload format. Usually applicable to BTF.
	 */
	CART_MESSAGE_ID_BCP,

	/**
	 * @ingroup CART_MESSAGE_ID
	 *
	 * @brief Head unit specific format
	 */
	CART_MESSAGE_ID_HU,

	/**
	 * @ingroup CART_MESSAGE_ID
	 *
	 * @brief RMDL Eng Mode payload format. Applicable to BRF
	 */
	CART_MESSAGE_ID_RMDL_ENG,
	CART_MESSAGE_ID_EEPROM,

	/**
	 * @ingroup CART_MESSAGE_ID
	 *
	 * @brief SOME/IP payload format. Usually, applicable to BCF
	 */
	CART_MESSAGE_ID_SOMEIP,
	/**
	 * @ingroup CART_MESSAGE_ID
	 *
	 * @brief RMDL Init Room mode. Applicable to BRF
	 */
	CART_MESSAGE_ID_RMDL_INIT,
	/**
	 * @ingroup CART_MESSAGE_ID
	 *
	 * @brief RMDL Opt Mode payload format. Applicable to BRF
	 */
	CART_MESSAGE_ID_RMDL_OPT,
	/**
	 * @ingroup CART_MESSAGE_ID
	 *
	 * @brief RMDL Eng Mode payload with BDP command (*AD). Applicable to BRF/BTF
	 */
	CART_MESSAGE_ID_RMDL_BDP,
	/**
	 * @ingroup CART_MESSAGE_ID
	 *
	 * @brief CART Status query
	 */
	CART_MESSAGE_ID_STATUS,
	/**
	 * @ingroup CART_MESSAGE_ID
	 *
	 * @brief CART message to be used by the module wrapping CART components
	 */
	CART_MESSAGE_ID_MODULE,

} CART_MESSAGE_ID;


#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_CART_MESSAGE_ID__INCLUDED_)*/
