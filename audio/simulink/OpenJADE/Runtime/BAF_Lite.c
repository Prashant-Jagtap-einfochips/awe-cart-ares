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
 * @file BAF_Lite.c
 *
 * @brief Implementation of the Class BAF_Lite
 *
 * @author: cb1037694
 ****************************************************/

#include <stddef.h>
#include "BAF.h"
#include "BAF_Lite.h"
#include "_BAF_Lite.h"
#include "Message_id.h"
#include "Error_Id.h"

#define BAF_EXEC_NOT_RUNNING 0
#define BAF_EXEC_RUNNING     1

static _BAF_Lite BAF_Lite_obj = {
   .task_rate_factors = {MODEL_TASK_RATE_FACTORS}
};

BAF_IO_POINTERS;

BAF_Resource* BAF_resource(BAF* baf)
{
    _BAF_Lite* this_private = (_BAF_Lite*) baf;
    this_private->resource.memory_size = BAF_RESOURCE_MEMORY_SIZE;
    this_private->resource.num_threads = MODEL_SYNC_TASK_COUNT;
    this_private->resource.base_thread = SYSTEM_BASE_RATE_TID;
    this_private->resource.call_interval = SYSTEM_BASE_RATE_FACTOR;
    this_private->resource.ext_scheduler = 1;
    this_private->resource.mask = 0;
    this_private->resource.num_modules = 2;
    this_private->resource.num_connections = 2;
    return (BAF_Resource*) &this_private->resource;
}

BAF* BAF_create(BAF* baf)
{
    BAF_Lite_obj.notification_handler = NULL;
    return (BAF*) &BAF_Lite_obj;
}

int32_t BAF_init(BAF* baf)
{
    _BAF_Lite* this_private = (_BAF_Lite*) baf;
    uint32_t i;

    BAF_IO_POINTERS_INIT(this_private);

    for ( i = 0; i< MODEL_SYNC_TASK_COUNT; i++){
        this_private->task_counter[i] = this_private->task_rate_factors[i] - 1; // step back counters one unit
        this_private->task_state[i] = BAF_EXEC_NOT_RUNNING;
    }
    MODEL_FUNC(initialize)();

    return 0;
}

uint32_t BAF_exec(BAF* baf, uint32_t thread_level, void* arg)
{
    _BAF_Lite* this_private = (_BAF_Lite*) baf;
    uint32_t i;
    uint32_t current_time;
    uint32_t base_thread = this_private->resource.base_thread;
    uint32_t overrun = 0;

    this_private->task_state[thread_level] = BAF_EXEC_RUNNING;
    if(thread_level == base_thread) { // base rate
        current_time = 0;
        do {
            if (base_thread != 0){
                MODEL_STEP(0);
            }
            if (current_time == 0){ // base thread should run between thread 0 and the rest
                BAF_INPUT_CALLBACK_SYS(thread_level, arg);
                MODEL_STEP(thread_level);
                BAF_OUTPUT_CALLBACK_SYS(thread_level, arg);
            }
            for(i = base_thread + 1; i < MODEL_SYNC_TASK_COUNT; i++){ // skip base thread
                this_private->task_counter[i]++;
                if ( this_private->task_counter[i] == this_private->task_rate_factors[i] ){
                    this_private->task_counter[i] = 0;
                    if ( this_private->task_state[i] == BAF_EXEC_RUNNING ){
                        overrun |= (1 << i); // overrun mask
                    }
                    BAF_Callback_post(i, arg);
                }
            }
            current_time++;
        } while (current_time < this_private->resource.call_interval);
    }
    else
    {
        // thread_level > base_thread (it may run on reentrancy from BAF_Callback_post)
        BAF_INPUT_CALLBACK_SYS(thread_level, arg);
        MODEL_STEP(thread_level);
        BAF_OUTPUT_CALLBACK_SYS(thread_level, arg);
    }
   this_private->task_state[thread_level] = BAF_EXEC_NOT_RUNNING;
   return overrun;
}

uint32_t BAF_schedule(BAF* baf, void* arg)
{
    _BAF_Lite* this_private = (_BAF_Lite*) baf;
    uint32_t i;
    uint32_t current_time;
    uint32_t base_thread = this_private->resource.base_thread;

    current_time = 0;
    do {
        if (base_thread != 0) {
            MODEL_STEP(0);
        }
        for(i = base_thread + 1; i < MODEL_SYNC_TASK_COUNT; i++){ // skip base thread
            this_private->task_counter[i]++;
            if ( this_private->task_counter[i] == this_private->task_rate_factors[i] ){
                this_private->task_counter[i] = 0;
                BAF_Callback_post(i, arg);
            }
        }
        current_time++;
    } while (current_time < this_private->resource.call_interval);
   return 0;
}

int32_t BAF_command(BAF* baf, BAF_Message* req, BAF_Message* rsp)
{
    _BAF_Lite* this_private = (_BAF_Lite*) baf;
    int32_t status = -1;
    uint32_t id = req->id & 0xFF;
    uint32_t size = req->size;

    rsp->id = req->id;

    if( rsp != NULL ) {
      switch (id) {
        case MESSAGE_NOTIFICATION_POLL:
           status = MODEL_FUNC(ServiceNotifications)(rsp->data, rsp->size);
           if (status > 0) {
               if (this_private->notification_handler != NULL) {
                   rsp->id = MESSAGE_NOTIFICATION;
                   rsp->size = status;
                   this_private->notification_handler(baf, rsp);
               }
           }
           break;
        case MESSAGE_PASSTHRU_SINGLE:
        case MESSAGE_PASSTHRU_SINGLE_BLIND:
           if (0 == size) {
             status = 0;
           }
           else {
             status = MODEL_FUNC(HandleAsdIdRequest)(req->data, req->size, rsp->data, rsp->size);
             if( MESSAGE_PASSTHRU_SINGLE_BLIND == id ){
                if (status >= 0){
                   status = 0;
                }
             }
           }
           break;
        case MESSAGE_VERSION:
           if (rsp->data == NULL)
           {
               status = ERROR_BUFFER_NULL;
               break;
           }
           if ( rsp->size < 12 ) {
               status = ERROR_BUFFER_TOO_SMALL;
               break;
           }
           {
              uint32_t i;
              const char* ver = BAF_version(NULL);
              char* msg = (char*)rsp->data;
              for ( i = 0; i < 9; i++ ) *msg++ = *ver++; // version string length is 9
              rsp->size = 12; // roundup to 32bit word size
              status = 12;
           }
           break;
        default:
           status = ERROR_INVALID_COMMAND;
           break;
      }
      if (status >= 0) {
          rsp->size = status;
      }
    }
    return status;
}

void BAF_register_notify(BAF* baf, BAF_Func_Notify notify)
{
    _BAF_Lite* this_private = (_BAF_Lite*) baf;
    this_private->notification_handler = notify;
}

const char *BAF_version(BAF* baf)
{
    baf = (baf == NULL) ? (NULL) : (baf);
    return _BAF_VERSION_;
}

size_t BAF_logger_size(void)
{
   return (size_t)(0);
}

void *BAF_logger_addr(void)
{
   return (void *)NULL;
}

