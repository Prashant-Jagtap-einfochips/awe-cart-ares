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
 * @file CART_Message.h
 *
 * @ingroup CART_Message
 *
 * @brief Implementation of the Class CART_Message
 *
 * @author: cb1037694
 ****************************************************/

#if !defined(BC_CART_Message__INCLUDED_)
#define BC_CART_Message__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "CART_STATUS.h"
#include "CART_MESSAGE_ID.h"
#include "CART_MESSAGE_TYPE.h"


/**
 * @ingroup CART_Message
 *
 * @brief @author cb1037694
 *
 * @version 1.0
 */
typedef struct CART_Message
{
	uint32_t id;
	uint32_t size;
	uint32_t *data;
}  CART_Message;

typedef struct CART_Phy_Message
{
	uint32_t id;
	uint32_t size;
	uint32_t data[];
}  CART_Phy_Message;

typedef CART_Phy_Message audio_control_message;

typedef CART_Phy_Message CART_Callback_Message;

typedef CART_STATUS BCF_RET_CODE_TYPE;

#define CART_MESSAGE_HEADER_BYTES (8)

#define INDIVIDUAL_DATA_SIZE sizeof(uint32_t)

#define ACM_HEADER_BYTES (CART_MESSAGE_HEADER_BYTES)

#define CART_Message_set(_message, _id , _size) \
  (_message)->id = (_id);         \
  (_message)->size = (_size);     \

#define CART_MESSAGE_HANDLE_MASK 0xFFFFU
#define CART_MESSAGE_SIZE_MASK 0xFFFFU
#define CART_MESSAGE_ID_MASK 0xFFFU
#define CART_MESSAGE_TYPE_MASK 0xFU
#define CART_MESSAGE_CART_ID_MASK 0xFFU
#define CART_MESSAGE_SRC_ID_POS 16
#define CART_MESSAGE_DST_ID_POS 24
#define CART_MESSAGE_HANDLE_POS 16
#define CART_MESSAGE_SIZE_POS 0
#define CART_MESSAGE_ID_POS 0
#define CART_MESSAGE_TYPE_POS 12
#define CART_MESSAGE_HANDLE_IDX 1
/**
 * @ingroup CART
 *
 * @brief This API retrieves the CART component sender id for the provided CART_Message*
 * argument.
 *
 * @param message message to retrieve the sender id from
 */
#define CART_Message_get_src(_message) (CART_ID)(((_message)->size >> CART_MESSAGE_SRC_ID_POS) & CART_MESSAGE_CART_ID_MASK)

/**
 * @ingroup CART
 *
 * @brief This API retrieves the CART component destination id for the provided
 * CART_Message* argument.
 *
 * @param message message to retrieve the destination id from
 */
#define CART_Message_get_dst(_message) (CART_ID)(((_message)->size >> CART_MESSAGE_DST_ID_POS) & CART_MESSAGE_CART_ID_MASK)

#define CART_Message_get_handle(_message) (((_message)->id >> CART_MESSAGE_HANDLE_POS) & CART_MESSAGE_HANDLE_MASK)
#define CART_Message_get_size(_message) (((_message)->size >> CART_MESSAGE_SIZE_POS) & CART_MESSAGE_SIZE_MASK)
#define CART_Message_get_id(_message) (CART_MESSAGE_ID)(((_message)->id >> CART_MESSAGE_ID_POS) & CART_MESSAGE_ID_MASK)
#define CART_Message_get_type(_message) (CART_MESSAGE_TYPE)((((_message)->id >> CART_MESSAGE_TYPE_POS) & CART_MESSAGE_TYPE_MASK))

#define CART_Message_set_dst(_message, _dst_id) (_message)->size=((((_dst_id) & CART_MESSAGE_CART_ID_MASK) << CART_MESSAGE_DST_ID_POS) | ((_message)->size & ~(CART_MESSAGE_CART_ID_MASK << CART_MESSAGE_DST_ID_POS)))
#define CART_Message_set_src(_message, _src_id) (_message)->size=((((_src_id) & CART_MESSAGE_CART_ID_MASK) << CART_MESSAGE_SRC_ID_POS) | ((_message)->size & ~(CART_MESSAGE_CART_ID_MASK << CART_MESSAGE_SRC_ID_POS)))
#define CART_Message_set_handle(_message, _handle) (_message)->id=((((_handle) & CART_MESSAGE_HANDLE_MASK ) << CART_MESSAGE_HANDLE_POS) | ((_message)->id & ~(CART_MESSAGE_HANDLE_MASK  << CART_MESSAGE_HANDLE_POS)))

#define CART_Message_set_id(_message, _id) (_message)->id=((((_id) & CART_MESSAGE_ID_MASK ) << CART_MESSAGE_ID_POS) | ((_message)->id & ~(CART_MESSAGE_ID_MASK  << CART_MESSAGE_ID_POS)))
#define CART_Message_set_size(_message, _size) (_message)->size=((((_size) & CART_MESSAGE_SIZE_MASK ) << CART_MESSAGE_SIZE_POS) | ((_message)->size & ~(CART_MESSAGE_SIZE_MASK  << CART_MESSAGE_SIZE_POS)))
#define CART_Message_set_type(_message, _type) (_message)->id=((((_type) & CART_MESSAGE_TYPE_MASK ) << CART_MESSAGE_TYPE_POS) | ((_message)->id & ~(CART_MESSAGE_TYPE_MASK  << CART_MESSAGE_TYPE_POS)))


/**
 * @ingroup CART
 *
 * @brief This API constructs CART_Message from CART_Phy_Message
 *
 * @param dst Destination CART_Message pointer
 * @param src Source CART_Phy_Message pointer
 */
#define CART_Message_initialize(_dst, _src) do {\
   (_dst)->id = (_src)->id;\
   (_dst)->size = (_src)->size;\
   (_dst)->data = &((_src)->data[0]);\
} while(0)

/**
 * @ingroup CART
 *
 * @brief This API copies header words from one CART message pointer to another.
 *        It copies header words between CART_Message and CART_Phy_Message
 *
 * @param dst Destination CART_Message or CART_Phy_Message pointer
 * @param src Source CART_Message or CART_Phy_Message pointer
 */
#define CART_Message_copy_header(_dst, _src) do {\
   (_dst)->id = (_src)->id;\
   (_dst)->size = (_src)->size;\
} while(0)

#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_CART_Message__INCLUDED_)*/
