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
 * @file BAF.h
 *
 * @brief Implementation of the Class BAF
 *
 * @author: cb1037694
 ****************************************************/

#include <stddef.h>
#include "BAF.h"
#include "_BAF.h"
#include "Audio_Graph.h"
#if defined (BAF_PRIORITY_SCHEDULER)
#include "JSched/JSched.h"
#endif

#define BAF_EXEC_RUNNING 1
#define BAF_EXEC_NOT_RUNNING 0

extern JModule_List JModule_List_obj;
extern Audio_Graph Audio_Graph_obj;

#if defined(BAF_LOGGER)
Logger BAF_logger[FW_NUM_LOG];
#endif
extern _BAF BAF_obj;


#if defined (BAF_PRIORITY_SCHEDULER)
static void BAF_poll(BAF* baf, uint32_t level, void* context){
    int32_t error;
    BAF_Message baf_message_req;

    baf_message_req.id = MESSAGE_NOTIFICATION_POLL;
    error = BAF_command(baf, &baf_message_req, NULL); // poll
    if (error < 0)
    {
      //_DERR(printf("BAF_command error: %lu ",error));
      return;
    }
}


static JExec exec_array[ FW_NUM_THREAD + 1 ];
static JExec exec_poll;
static JSched scheduler;

static void _BAF_Callback_post(uint32_t priority, void* arg)
{
   JSched_post(&scheduler, &exec_array[priority]);
   if (priority == 0 ) { // poll often at low priority
       JSched_post(&scheduler, &exec_poll);
   }
   //BAF_exec(baf, priority, NULL);
}
#else // BAF_PRIORITY_SCHEDULER
static void _BAF_Callback_post(JFramework* framework, uint32_t exec_id, void* arg)
{
	if(framework->callback_register.post != NULL) {
		framework->callback_register.post(exec_id, arg);
	}
#ifndef BAF_REMOVE_OJ3_SUPPORT
	else {
		BAF_Callback_post(exec_id, arg);
	}
#endif  // BAF_REMOVE_OJ3_SUPPORT
}
#endif // BAF_PRIORITY_SCHEDULER

#if defined (BAF_CALLBACK_CONTEXT)
void _BAF_Callback_sys_process_ctx(JFramework* framework, uint32_t proc_id, uint32_t frame_size, uint32_t num_chan_in, uint32_t num_chan_out, void* src, void* dst, void* arg)
{
	if(framework->callback_register.sys_process_ctx != NULL) {
		framework->callback_register.sys_process_ctx(proc_id, frame_size, num_chan_in, num_chan_out, src, dst, arg);
	}
#ifndef BAF_REMOVE_OJ3_SUPPORT
	else {
		BAF_Callback_sys_process_ctx(proc_id, frame_size, num_chan_in, num_chan_out, src, dst, arg);
	}
#endif // BAF_REMOVE_OJ3_SUPPORT
}
#endif // BAF_CALLBACK_CONTEXT

BAF_Resource* BAF_resource(BAF* baf)
{
	_BAF* this_private = (_BAF*) baf;
	Framework_Builder_resource(&this_private->framework_builder, &this_private->resource,
								this_private->audio_graph, this_private->feature_list);
    this_private->resource.mask = 0;
	return (BAF_Resource*) &this_private->resource;
}

#ifndef BAF_REMOVE_OJ3_SUPPORT
extern _BAF* pBAF_obj;
#endif // BAF_REMOVE_OJ3_SUPPORT
BAF* BAF_create(BAF* baf)
{
	baf = (baf == NULL) ? (NULL) : (baf);
#ifndef BAF_REMOVE_OJ3_SUPPORT
	return (BAF*) pBAF_obj;
#else
	return (BAF*) NULL;
#endif // BAF_REMOVE_OJ3_SUPPORT
}

int32_t BAF_init(BAF* baf)
{
	_BAF* this_private = (_BAF*) baf;
	Error_Id error_id;

#if defined (BAF_PRIORITY_SCHEDULER)
    uint32_t num_threads = this_private->resource.num_threads;
    if (num_threads < FW_NUM_THREAD)
    {
       uint32_t thread_idx;
       for (thread_idx = 0; thread_idx < num_threads; thread_idx++){
          JExec_init(&exec_array[thread_idx], BAF_exec, baf, thread_idx, NULL);
       }
       if ( thread_idx < FW_NUM_THREAD) {
          JExec_init(&exec_poll, BAF_poll, baf, thread_idx, NULL);
       }
       else {
           error_id = ERROR_EXEC_TOO_MANY;
           Error_print(NULL, 0,error_id,"BAF_init: failed: %d\n", error_id, 0 );
           return error_id;
       }
    }
    else
    {
       error_id = ERROR_EXEC_TOO_MANY;
       Error_print(NULL, 0,error_id,"BAF_init: failed: %d\n", error_id, 0 );
       return error_id;
    }
    JSched_init(&scheduler);

#endif	

#if defined(BAF_LOGGER)
    if (this_private->resource.memory_log_size > 0)
    {
       uint32_t logger_id;
       uint32_t ind_heap_size;
       uint32_t heap_size[FW_NUM_LOG];
       uint8_t *memory_log_pointer = (uint8_t*)this_private->resource.memory_log_pointer;

       /* If memory for log is provided, but individual sizes are not specified, split the heap */
       if ((this_private->resource.print_log_size == 0) &&
           (this_private->resource.capture_log_size == 0) &&
           (this_private->resource.exec_log_size == 0))
       {
          ind_heap_size = (this_private->resource.memory_log_size/FW_NUM_LOG);
          for ( logger_id = 0; logger_id < FW_NUM_LOG; logger_id++) {
             heap_size[logger_id] = ind_heap_size;
          }
       }
       else
       {
          heap_size[LOG_TYPE_PRINT]   = this_private->resource.print_log_size;
          heap_size[LOG_TYPE_CAPTURE] = this_private->resource.capture_log_size;
          heap_size[LOG_TYPE_EXEC]    = this_private->resource.exec_log_size;
       }

       for ( logger_id = 0; logger_id < FW_NUM_LOG; logger_id++) {

          /* Truncate to fit whole logs, which are power of 2 in size */
          ind_heap_size = heap_size[logger_id] & ~(sizeof(Log)-1);

          Heap_init(&(BAF_logger[logger_id].heap),
                    memory_log_pointer,
                    ind_heap_size);

          Logger_init((Logger_Type)logger_id, true);

          memory_log_pointer += ind_heap_size;
       }
    }
#endif

	Heap_init(&this_private->heap, this_private->resource.memory_pointer, this_private->resource.memory_size);
	
	Framework_Builder_init(&this_private->framework_builder, &this_private->framework, &this_private->heap, this_private->audio_graph, this_private->feature_list);
	error_id = Framework_Builder_build_framework(&this_private->framework_builder);
	if ( error_id != SUCCESS ){
		Error_print(NULL, 0,error_id,"BAF_init: Framework_Builder_build_framework failed: %d\n", error_id, 0 );
		return error_id;
	}
	JFramework_init_exec(&this_private->framework);
	return 0;
}

uint32_t BAF_exec(BAF* baf, uint32_t thread_level, void* arg)
{
	_BAF* this_private = (_BAF*) baf;
	uint32_t i;
	uint32_t current_time;
	uint32_t base_thread = this_private->resource.base_thread;
	uint32_t ext_scheduler = this_private->resource.ext_scheduler;

	uint32_t call_interval_base_thread = this_private->framework.call_interval[base_thread];
	uint32_t call_interval_thread_zero = this_private->framework.call_interval[0];
	uint32_t overrun = 0;

	this_private->framework.thread_state[thread_level] = BAF_EXEC_RUNNING;
#if !defined(BAF_PRIORITY_SCHEDULER)
#if !defined(BAF_USE_SCHEDULE)
	if(thread_level == base_thread) { // base rate
		current_time = 0;
		do {
			if ( (base_thread != 0) && (ext_scheduler != 0) ){
				JFramework_thread_exec(&this_private->framework, 0, arg);
			}
			if (current_time == 0){ // base thread should run between thread 0 and the rest
				JFramework_thread_exec(&this_private->framework, thread_level, arg);
			}
			for(i = base_thread + 1; i < this_private->framework.num_thread; i++){ // skip base thread
				this_private->framework.thread_counter[i] += call_interval_thread_zero;
				if ( this_private->framework.thread_counter[i] == (this_private->framework.call_interval[i]) ){
					this_private->framework.thread_counter[i] = 0;
					if ( this_private->framework.thread_state[i] == BAF_EXEC_RUNNING ){
						overrun |= (1 << i); // overrun mask
					}
					_BAF_Callback_post(&this_private->framework,i, arg);
				}
			}
			current_time += call_interval_thread_zero;
		} while (current_time < call_interval_base_thread);
	}
	else
#endif
#endif
	{
		// thread_level > base_thread (it may run on reentrancy from BAF_Callback_post)
		JFramework_thread_exec(&this_private->framework, thread_level, arg);
	}
   this_private->framework.thread_state[thread_level] = BAF_EXEC_NOT_RUNNING;
   return overrun;
}

int32_t BAF_command(BAF* baf, BAF_Message* req, BAF_Message* rsp)
{
	_BAF* this_private = (_BAF*) baf;
    Message Message_req, Message_rsp;
	int32_t status;
	
	Message_req.module_id = (req->id >> 8) & 0xff;
	Message_req.submodule_id = (req->id >> 16) & 0xff;
	Message_req.message_id = req->id;
	Message_req.message_size = req->size;
	Message_req.message_data = req->data;
	
    if( rsp != NULL ) {
        Message_rsp.message_size = rsp->size;	
        Message_rsp.message_data = rsp->data;
    }
	 else {
        Message_rsp.message_size = 0;	
        Message_rsp.message_data = NULL;
	 }

	status = JFramework_command(&this_private->framework, &Message_req, &Message_rsp);

    if( rsp != NULL ) {
        rsp->id = Message_rsp.message_id;
        rsp->size = Message_rsp.message_size;
    }
	return status;
}

void BAF_register_notify(BAF* baf, BAF_Func_Notify notify)
{
	_BAF* this_private = (_BAF*) baf;
	 JFramework_register_notify( &this_private->framework, notify);
}

uint32_t BAF_schedule(BAF* baf, void* arg)
{


	_BAF* this_private = (_BAF*) baf;
	uint32_t i;
	uint32_t current_time;
	uint32_t base_thread = this_private->resource.base_thread;
	uint32_t ext_scheduler = this_private->resource.ext_scheduler;

	uint32_t call_interval_base_thread = this_private->framework.call_interval[base_thread];
	uint32_t call_interval_thread_zero = this_private->framework.call_interval[0];

	uint32_t overrun = 0;
    uint32_t exec_mask = 0;

	current_time = 0;
	do {
		if ( (base_thread != 0) && (ext_scheduler != 0) ){
			JFramework_thread_exec(&this_private->framework, 0, arg);
		}
		for(i = base_thread; i < this_private->framework.num_thread; i++){ // skip base thread
			this_private->framework.thread_counter[i] += call_interval_thread_zero;
			if ( this_private->framework.thread_counter[i] == (this_private->framework.call_interval[i]) ){
				this_private->framework.thread_counter[i] = 0;
				if ( this_private->framework.thread_state[i] == BAF_EXEC_RUNNING ){
                   overrun |= (1 << i); // overrun mask
				}
                exec_mask |= (1 << i);
#if defined(BAF_PRIORITY_SCHEDULER)
				_BAF_Callback_post(i, arg);
#else
				_BAF_Callback_post(&this_private->framework, i, arg);
#endif
			}
		}
		current_time += call_interval_thread_zero;
	} while (current_time < call_interval_base_thread);
    if (exec_mask != 0) {
        BAF_Callback_post_mask(exec_mask, arg);
    }

	return overrun;
}

const char *BAF_version(BAF* baf)
{
	baf = (baf == NULL) ? (NULL) : (baf);
	return _BAF_VERSION_;
}

size_t BAF_logger_size(void)
{
#if defined(BAF_LOGGER)
   return (size_t)(sizeof(Logger)*FW_NUM_LOG);
#else
   return (size_t)(0);
#endif
}

void *BAF_logger_addr(void)
{
#if defined(BAF_LOGGER)
   return (void *)&BAF_logger[0];
#else
   return (void *)NULL;
#endif
}


void BAF_Register_post(BAF* baf, BAF_Callback_Func_Post funcptr)
{
	_BAF* this_private = (_BAF*) baf;
	this_private->framework.callback_register.post = funcptr;
}

void BAF_Register_sys_process_ctx(BAF* baf, BAF_Callback_Func_Sys_Process_Ctx funcptr)
{
	_BAF* this_private = (_BAF*) baf;
	this_private->framework.callback_register.sys_process_ctx = funcptr;
}

