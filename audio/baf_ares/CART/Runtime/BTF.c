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
 * @file BTF.h
 * 
 * @brief Implementation of the Class BTF       
 *
 * @author: cb1037694                     
 ****************************************************/

#include "BTF.h"
#include "_BTF.h"
#include "FeatureLib.h"
#include "BACR_API.h"

_BTF BTF_obj;

/**
 * Returns pointer to the BTF object associated with the CART_ID passed as argument
 */
BTF* BTF_create(CART_ID btf_id)
{
   BTF_obj.id = btf_id;

   /* set BAF id defaults */
   for (uint8_t i = 0; i < BTF_RESOURCE_NUM_BAF; i++) {
      BTF_obj.resource.baf_id[i] = (CART_ID)(CART_ID_BAF + i);
   }
   BTF_obj.resource.brf_id = CART_ID_BRF;
   BTF_obj.resource.app_id = CART_ID_APP;
   return  (BTF*) &BTF_obj;
}

BTF_Resource* BTF_resource(BTF* btf)
{
    _BTF* btf_private = (_BTF*) btf;
    return &btf_private->resource;
}

/**
 * This API initializes the BTF module.
 * Prerequisite: BTF_create()
 */
CART_STATUS BTF_init(BTF* btf, void* ctx)
{
  _BTF* btf_private = (_BTF*) btf;

  btf_private->ctx = ctx;
  btf_private->bamfRespCount = 0;
  btf_private->bamfNotifyCount = 0;
  btf_private->bamfInvalidCount = 0;

  BTF_FeatureLib_Init(btf_private);

  return CART_STATUS_SUCCESS;
}

/**
 * This API is used to send tune commands to BTF (ex. BDP. BCP)
 */
#include "bdp.h"
CART_Cmd_Ret BTF_command(BTF* btf, CART_Message* req, CART_Message* rsp)
{
   _BTF* btf_private = (_BTF*) btf;
 
   CART_Cmd_Ret cmd_ret={CART_STATUS_ERROR};
   int8_t *pCmd = (int8_t *)req->data;
   int8_t *rspBuf = (int8_t *)rsp->data;
   uint16_t rspLen = rsp->size;
   CART_ID src, dst;
   uint16_t handle;

   dst=CART_Message_get_dst(req);
   src=CART_Message_get_src(req);
   btf_private->srcId = src;

   if (btf_private->id != dst) {
      CART_Message_copy_header(rsp, req); 
      rsp->data = req->data; 
      CART_Message_set_type(rsp, CART_MESSAGE_TYPE_ERROR); 
      CART_Message_set_src(rsp, btf_private->id);
      CART_Message_set_dst(rsp, src); 
      cmd_ret.status = CART_STATUS_ERROR;
      return cmd_ret;
   }
   CART_MESSAGE_ID msg_id = CART_Message_get_id(req);

   switch (msg_id)
   {
   case CART_MESSAGE_ID_BDP:

      rspLen = BDP_ExecuteCmd(pCmd, rspBuf, rspLen);
      BTF_Callback_post(CART_EXEC_ID_DEFAULT, btf_private->ctx); 

      CART_Message_set_id(rsp, msg_id); 
      CART_Message_set_type(rsp, CART_MESSAGE_TYPE_RSP); 
      CART_Message_set_size(rsp, rspLen); 
      cmd_ret.status = CART_STATUS_SUCCESS;
      break;
   case CART_MESSAGE_ID_BCP:

      // TODO: BCP handling
      BTF_Callback_post(CART_EXEC_ID_DEFAULT, btf_private->ctx); 
      CART_Message_set_id(rsp, msg_id); 
      CART_Message_set_type(rsp, CART_MESSAGE_TYPE_RSP); 
      cmd_ret.status = CART_STATUS_SUCCESS;
      break;
   default:
      CART_Message_set_type(rsp, CART_MESSAGE_TYPE_ERROR); 
      CART_Message_set_size(rsp, 0); 
      cmd_ret.status = CART_STATUS_ERROR;
   }
   src=CART_Message_get_src(req);
   dst=CART_Message_get_dst(req);
   handle=CART_Message_get_handle(req);
   CART_Message_set_src(rsp, dst);
   CART_Message_set_dst(rsp, src); 
   CART_Message_set_handle(rsp, handle); 

   return cmd_ret;
}

/**
 * This API is invoked by the BTF_receive() dispatcher to send BAF responses to
 * BTF (ex. TOP, TSP reads)
 */
CART_Cmd_Ret BTF_receive(BTF* btf, CART_Message* rsp)
{
   _BTF* btf_private = (_BTF*) btf;
   CART_Cmd_Ret ret = {CART_STATUS_SUCCESS};
   BTF_BACR_receive(btf_private, rsp);
   return ret;
}

/**
 * This API translates Tune commands to BAF command message(s), serializes and
 * sends the message to BAF
 */
uint32_t BTF_exec(BTF* btf, CART_EXEC_ID exec_id, void* ctx)
{
   _BTF* btf_private = (_BTF*) btf;
   switch (exec_id){
      case CART_EXEC_ID_BACR:
         BTF_BACR_PeriodicWork(btf_private);
         break;
//      case CART_EXEC_ID_INPUTEQ:
//         BTF_InputEq_StateMachine();
//         break;
      default:
         break;
   }
    return 0;
} 

