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
 * @file CMB_TYPES.h
 *
 * @brief Declaration of types (enums, structures
 *        etc.) used with CMB
 *
 ****************************************************/

#if !defined(BC_CMB_TYPES__INCLUDED_)
#define BC_CMB_TYPES__INCLUDED_

#include <stddef.h>
#include "CART_ID.h"
#include "CART_Cmd_Ret.h"

#ifdef __cplusplus
extern "C" {
#endif

// If required, make a public void type. And a private structure
// with int index.
// typddef void CMB_Adaptor_Id; here
// typedef struct {int index; } _CMB_Adaptor_Id; in adaptor_types.h
typedef uint32_t CMB_Adaptor_Id;

typedef void CMB;


typedef enum
{
    CMB_CMD_FIRST = 0,
    CMB_CMD_ADD_PROXY,
    CMB_CMD_INITIALIZE_ADAPTOR,
    CMB_CMD_SEND_MSG,
    CMB_CMB_REGISTER_SEND_CALLBACK,
    CMB_CMB_REGISTER_RECV_CALLBACK,
    CMB_CMD_DEINITIALIZE_ADAPTORS,
    CMB_CMD_LAST,
    CMB_CMD_COUNT = CMB_CMD_LAST - CMB_CMD_FIRST,
} CMB_COMMAND;


typedef CART_Cmd_Ret (*CMB_Callback_recv)(uint8_t* buffer, const uint32_t len, ssize_t* recv_length);

typedef CART_Cmd_Ret (*CMB_Callback_send)(uint8_t* payload, uint32_t size);


typedef struct CMB_Adaptor_proxyConfig
{
    // ID of the adaptor that will have a proxy added to it.
    CMB_Adaptor_Id id;

    // This is the CART ID of the component to which the adaptor proxy
    // is to communicate with.
    CART_ID dstId;

    // Mask to be used with CART ID.
    uint32_t cartIdMask;

    // This is the proxy configuration to be passed to the adaptor.
    void* configuration;
} CMB_Adaptor_proxyConfig;


typedef struct CMB_Adaptor_config
{
    // ID of the adaptor to be configured.
    CMB_Adaptor_Id id;

    // This is the configuration of the adaptor itself (as opposed to the proxy that it connects to).
    void* configuration;
} CMB_Adaptor_config;


typedef struct
{
    // ID of the adaptor to be used.
    CMB_Adaptor_Id id;

    // This is the callback function to be registered.
    CMB_Callback_recv func;
} CMB_Adaptor_recvCallbackConfig;

typedef struct
{
    // ID of the adaptor to be used.
    CMB_Adaptor_Id id;

    // This is the callback function to be registered.
    CMB_Callback_send func;
} CMB_Adaptor_sendCallbackConfig;


#if 0
typedef struct SendMsgContext
{
  void* reqeust;
  void* response;
} SendMsgContext;



typedef struct AddAdaptorInitContext
{
   CART_ID apx;
   ADAP
} AddAdaptorInitContext;
#endif

#ifdef __cplusplus
}
#endif

#endif /*!defined(BC_CMB_TYPES__INCLUDED_)*/
