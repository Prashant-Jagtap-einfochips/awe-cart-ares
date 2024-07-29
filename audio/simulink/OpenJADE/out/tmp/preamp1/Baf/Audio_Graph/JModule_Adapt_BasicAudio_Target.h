/*******************************************************************************
*  BOSE CORPORATION
*  COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
*  This program may not be reproduced, in whole or in part in any
*  form or any means whatsoever without the written permission of:
*     BOSE CORPORATION
*     The Mountain
*     Framingham, MA 01701-9168
******************************************************************************/

/**
 * @file JModule_Adapt_BasicAudio_Target.h
 * @date 07/17/24
 * @brief 
 *
 * @details <OPTIONAL: Some details about file>
 *
 * @see <OPTIONAL: Links to reference/Another software component>
 *
 */

#if !defined(BC_JModule_Adapt_BasicAudio_Target__INCLUDED_)
#define BC_JModule_Adapt_BasicAudio_Target__INCLUDED_




#ifdef __cplusplus
extern "C" {
#endif



#include "Process_Context.h"
#include "Processes_Context.h"
#include "Message.h"
#include "JModule.h"
 
/**
 * @ingroup JModule_Adapt_Basicaudio_target
 *
 * @brief @param processes
 * 
 * @param context
 */



#ifdef __cplusplus
}
#endif

#endif /*!defined(BC_JModule_Adapt_BasicAudio_Target__INCLUDED_)*/
/**
 * @file JModule_Desc_BasicAudio_Target
 * @date 07/17/24
 * @brief Declaration of the ClassJModule_BasicAudio_Target_Param
 *
 * @details <OPTIONAL: Some details about file>
 *
 * @see <OPTIONAL: Links to reference/Another software component>
 *
 */

#if !defined(BC_JModule_BasicAudio_Target_Param__INCLUDED_)
#define BC_JModule_BasicAudio_Target_Param__INCLUDED_




#ifdef __cplusplus
extern "C" {
#endif



#include <stdint.h>


typedef struct JModule_Basicaudio_target_Param
{
   uint32_t id;
} JModule_Basicaudio_target_Param;

typedef struct JModule_Basicaudio_target_State
{
   uint32_t id;
} JModule_Basicaudio_target_State;

enum JModule_Desc_Basicaudio_target
{
   JMODULE_BASICAUDIO_TARGET_NUM_PROCESSES = 1,
   JMODULE_BASICAUDIO_TARGET_STATE_SIZE = sizeof(JModule_Basicaudio_target_State),
   JMODULE_BASICAUDIO_TARGET_PARAM_SIZE = sizeof(JModule_Basicaudio_target_Param)
};

typedef enum
{
    PORT_IN_BASICAUDIO_TARGET_STEP0_IN1
} PORT_BASICAUDIO_TARGET_STEP0_IN;


typedef enum
{
    PORT_OUT_BASICAUDIO_TARGET_STEP0_OUT1
} PORT_BASICAUDIO_TARGET_STEP0_OUT;




typedef enum
{
    PROCESS_BASICAUDIO_TARGET_STEP0
} PROCESS_JMODULENAME_BASICAUDIO_TARGET;


#ifdef __cplusplus
}
#endif

#endif /*!defined(BC_JModule_BasicAudio_Target_Param__INCLUDED_)*/
/**
 * @file JModule_Adapt_BasicAudio_Target.h
 * @date 07/17/24
 * @brief 
 *
 * @details <OPTIONAL: Some details about file>
 *
 * @see <OPTIONAL: Links to reference/Another software component>
 *
 */



/* Definition of audio In/Out buffers expected by Talaria */
float* UnitTest_Target_AudioIn;
float* UnitTest_Target_AudioOut;

/* Talaria functions */
extern void BasicAudio_Target_initialize(void);
extern void BasicAudio_Target_step(uint32_t stepnum);
extern int32_t BasicAudio_Target_HandleAsdIdRequest(void *pReqBuf, uint32_t reqBufSize, void *pRspBuf, uint32_t rspBufSize);
extern int32_t BasicAudio_Target_ServiceNotifications(void *pNtfBuf, uint32_t ntfBufSize);


static void JModule_Adapt_BasicAudio_Target_init(JModule *this, Processes *processes, Processes_Context *context)
{
   this = (this == NULL) ? (NULL) : (this);
   processes = (processes == NULL) ? (NULL) : (processes);
   context = (context == NULL) ? (NULL) : (context);
   UnitTest_Target_AudioIn = (float *)(context->item[0].ports_in->item[0].payload);
   UnitTest_Target_AudioOut = (float *)(context->item[0].ports_out->item[0].payload);


   BasicAudio_Target_initialize();
}

static uint32_t JModule_Adapt_BasicAudio_Target_send(JModule *this, Processes_Context *context, Message *req, Message *rsp)
{
   int32_t status = BasicAudio_Target_HandleAsdIdRequest(req->message_data, req->message_size, rsp->message_data, rsp->message_size);
   this = (this == NULL) ? (NULL) : (this);
   context = (context == NULL) ? (NULL) : (context);
   if (status >= 0)
   {
      rsp->message_id = req->message_id;
      rsp->message_size = status;
   }
   return (uint32_t)status;
}

// Initialization routine called by model initialize
void Basicaudio_target_Init_IPC( void ){
};

/* Ping Pong: active buffer pointer declarations */
float *Basicaudio_target_Fwd_Ipc_buffer;

static void JModule_Adapt_BasicAudio_Target_process_step0(JModule *this, Process *process, Process_Context *process_ctx, uint32_t thread_level)
{
    
    this = (this == NULL) ? (NULL) : (this);
    process = (process == NULL) ? (NULL) : (process);
    thread_level = (thread_level == 0) ? (0) : (thread_level);

    BasicAudio_Target_step(0);
}


static void JModule_Adapt_BasicAudio_Target_register_notify(JModule *this, JModule_Func_Notify notify, void *framework)
{
   this->notify = notify;
   this->framework = framework;
}

static uint32_t JModule_Adapt_BasicAudio_Target_poll(JModule *this, Processes_Context *context, Message *req, Message *rsp)
{
   uint32_t *msg = rsp->message_data;
   uint32_t ntfBytes;
   context = (context == NULL) ? (NULL) : (context);
   req = (req == NULL) ? (NULL) : (req);
   ntfBytes = BasicAudio_Target_ServiceNotifications(msg, rsp->message_size);
   if ((ntfBytes > 0) && (this->notify))
   {
      if (msg[2]==1)
      {
         rsp->module_id = this->id;
         rsp->message_size = ntfBytes; 
         this->notify(this->framework, rsp);
      }
   }
   return ntfBytes;
}

static void JModule_Adapt_BasicAudio_Target_create(JModule *this)
{
    this->processes->process[0].exec = JModule_Adapt_BasicAudio_Target_process_step0;
   this->init = JModule_Adapt_BasicAudio_Target_init;
   this->command = JModule_Adapt_BasicAudio_Target_send;
   this->register_notify = JModule_Adapt_BasicAudio_Target_register_notify;
   this->poll = JModule_Adapt_BasicAudio_Target_poll;
}

/**
 * @file JModule_Desc_BasicAudio_Target
 * @date 07/17/24
 * @brief Implementation of the ClassJModule_BasicAudio_Target_Param
 *
 * @details <OPTIONAL: Some details about file>
 *
 * @see <OPTIONAL: Links to reference/Another software component>
 *
 */


#include "JModule.h"
#include "_JModule_Id.h"
#include "JModule_Bitfield.h"


static Ports JModule_Basicaudio_target_process_step0_in_port = {
    .num_items = 1,
    {{
        .data_type = PORT_DATA_REAL32,
        .direction = PORT_DIRECTION_IN,
        .frame_size = 32,
        .num_chan = 2,
        .sample_rate = 48000,
        .sample_size = 4,
        .passthru_port = 0
    }}
};

static Ports JModule_Basicaudio_target_process_step0_out_port = {
    .num_items = 1,
    {{
        .data_type = PORT_DATA_REAL32,
        .direction = PORT_DIRECTION_OUT,
        .frame_size = 32,
        .num_chan = 2,
        .sample_rate = 48000,
        .sample_size = 4,
        .passthru_port = 0
    }}
};


static Processes JModule_Basicaudio_target_processes = {
    .num_items = 1,
    {{
        .ports_in = &JModule_Basicaudio_target_process_step0_in_port,
        .ports_out = &JModule_Basicaudio_target_process_step0_out_port,
        .call_interval = 1,
        .exec = NULL,
        .feature = NULL
    }}
};


static JModule_Basicaudio_target_State JModule_Basicaudio_target_State_obj;
static JModule_Basicaudio_target_Param JModule_Basicaudio_target_Param_obj;

static JModule JModule_Basicaudio_target_obj = {
    .id = JMODULE_BASICAUDIO_TARGET,
    .processes = &JModule_Basicaudio_target_processes,
    .create = JModule_Adapt_BasicAudio_Target_create,
    .param = &JModule_Basicaudio_target_Param_obj,
    .state = &JModule_Basicaudio_target_State_obj,
    .init = NULL,
    .command = NULL,
    .register_notify = NULL,
    .state_size = 0,
    .param_size = 0
};
