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
 * @file BCF.h
 * 
 * @brief Implementation of the Class BCF       
 *
 * @author: cb1037694                     
 ****************************************************/

#include "BCF.h"
#include "_BCF.h"
#include "FeatureLib.h"
#include "BACR_API.h"

_BCF BCF_obj;

/**
 * Returns pointer to the BCF object associated with the CART_ID passed as argument
 */
BCF* BCF_create(CART_ID bcf_id)
{
   BCF_obj.id = bcf_id;

   /* set BAF id defaults */
   for (uint8_t i = 0; i < BCF_RESOURCE_NUM_BAF; i++) {
      BCF_obj.resource.baf_id[i] = (CART_ID)(CART_ID_BAF + i);
   }
   BCF_obj.resource.brf_id = CART_ID_BRF;
   BCF_obj.resource.app_id = CART_ID_APP;
   return  (BCF*) &BCF_obj;
}

BCF_Resource* BCF_resource(BCF* bcf)
{
    _BCF* bcf_private = (_BCF*) bcf;
    return &bcf_private->resource;
}

/**
 * This API initializes the BCF module.
 * Prerequisite: BCF_create()
 */
CART_STATUS BCF_init(BCF* bcf, void* ctx)
{
  _BCF* bcf_private = (_BCF*) bcf;

  bcf_private->ctx = ctx;
  bcf_private->bamfRespCount = 0;
  bcf_private->bamfNotifyCount = 0;
  bcf_private->bamfInvalidCount = 0;

  BCF_FeatureLib_Init(bcf_private);

  return CART_STATUS_SUCCESS;
}

/**
 * Usually HU command.
 * This API return success if command is valid. If valid the command will be
 * translated and sent to BAF, if required..
 */
#include "BcfParser.h"
CART_Cmd_Ret BCF_command(BCF* bcf, CART_Message* req, CART_Message* rsp)
{
   _BCF* bcf_private = (_BCF*) bcf;
 
   CART_Cmd_Ret cmd_ret={CART_STATUS_ERROR};
   uint8_t const *pCmd = (uint8_t const*)req->data;
   uint8_t *rspBuf = (uint8_t *)rsp->data;
   uint32_t reqLen = CART_Message_get_size(req);
   uint32_t rspLen = CART_Message_get_size(req);
   CART_ID src, dst;
   uint16_t handle;

   dst=CART_Message_get_dst(req);
   src=CART_Message_get_src(req);
   bcf_private->srcId = src;

   if (bcf_private->id != dst) {
      CART_Message_copy_header(rsp, req); 
      rsp->data = req->data; 
      CART_Message_set_type(rsp, CART_MESSAGE_TYPE_ERROR); 
      CART_Message_set_src(rsp, bcf_private->id);
      CART_Message_set_dst(rsp, src); 
      cmd_ret.status = CART_STATUS_ERROR;
      return cmd_ret;
   }
   CART_MESSAGE_ID msg_id = CART_Message_get_id(req);

   BCF_RET_CODE_TYPE retCode;
   switch (msg_id)
   {
   case CART_MESSAGE_ID_SOMEIP:
      retCode = BcpParser(pCmd, reqLen, rspBuf, &rspLen);
      CART_Message_set_size(rsp, rspLen);
      BCF_Callback_post(CART_EXEC_ID_DEFAULT, bcf_private->ctx); 
      CART_Message_set_id(rsp, CART_MESSAGE_ID_SOMEIP); 
      if (retCode == BCF_RET_SUCCESS){
         CART_Message_set_type(rsp, CART_MESSAGE_TYPE_RSP); 
         cmd_ret.status = CART_STATUS_SUCCESS;
      }
      else {
         CART_Message_set_type(rsp, CART_MESSAGE_TYPE_ERROR); 
         cmd_ret.status = CART_STATUS_ERROR;
      }
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
 * This API is invoked by the BCF_receive() dispatcher to send BAF responses to
 * BRF (ex. TOP, TSP reads)
 */
CART_Cmd_Ret BCF_receive(BCF* bcf, CART_Message* rsp)
{
   _BCF* bcf_private = (_BCF*) bcf;
   CART_Cmd_Ret ret = {CART_STATUS_SUCCESS};
   BCF_BACR_receive(bcf_private, rsp);
   return ret;
}


/**
 * This API translates HU/User command to BAF command() message(s), serializes and
 * sends the message to BAF.
 * In general this API is called on a different thread than the one that calls
 * BCF_command(), but it is not required.
 * This API can be invoked periodically, or it can be triggered by
 * BCF_Callback_post()
 * If BCF_Callback_post() calls BCF_exec(), then BCF_exec() will execute in the
 * same thread as BCF_command() 
 */
uint32_t BCF_exec(BCF* bcf, CART_EXEC_ID exec_id, void* ctx)
{
   _BCF* bcf_private = (_BCF*) bcf;
   switch (exec_id){
      case CART_EXEC_ID_DEFAULT:
         BCF_BACR_PeriodicWork(bcf_private);
         break;
      default:
         break;
   }
    return 0;
} 

