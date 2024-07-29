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
 * @file JFramework.h
 *
 * @brief Implementation of the Class JFramework
 *
 * @author: cb1037694
 ****************************************************/

#include "JFramework.h"
#include "Process_Mode.h"

extern const char *BAF_version(void*);


void JFramework_thread_exec(JFramework* this, uint32_t thread_level, void* arg)
{
	uint32_t i;
	for ( i=0; i<this->num_exec[thread_level]; i++ ){
#if defined(BAF_LOGGER)
#if defined(BAF_STATSLOGGER)
		Logger_stats(LOG_TYPE_CAPTURE, thread_level, i, 0);
#else
		Logger_print(LOG_TYPE_EXEC, thread_level, "Bgn:%d t:%10u\n", i, BAF_Callback_clock());
#endif /*defined(BAF_STATSLOGGER)*/
#endif /*defined(BAF_LOGGER)*/
		Process_exec(this->execution[thread_level][i].process, this->execution[thread_level][i].context, thread_level, arg);
#if defined(BAF_LOGGER)
#if defined(BAF_STATSLOGGER)
		Logger_stats(LOG_TYPE_CAPTURE, thread_level, i, 1);
#else
		Logger_print(LOG_TYPE_EXEC, thread_level, "End:%d t:%10u\n", i, BAF_Callback_clock());
#endif /*defined(BAF_STATSLOGGER)*/
#endif /*defined(BAF_LOGGER)*/
	}
	return;
}

void JFramework_init(JFramework* this)
{
	uint32_t i;
	this->num_module = 0;
	this->num_thread = 0;
	this->num_connection = 0;
	this->num_buffer = 0;
	for(i = 0; i<FW_NUM_THREAD; i++){
		this->num_exec[i] = 0;
	}
	
	return;
}

int32_t JFramework_add_buffer(JFramework* this, Buffer_Id buffer_id, void* payload)
{
	if( this->num_buffer >= this->size_buffer){
			return ERROR_BUFFER_TOO_MANY;
		}
	
		this->buffer[ this->num_buffer ].id = buffer_id;
		this->buffer[ this->num_buffer ].payload = payload;
		this->num_buffer++;
	
		return 0;
}

int32_t JFramework_add_module(JFramework* this, Module_Id module_id, JModule* feature, JModule_Context* context)
{
	if( this->num_module >= this->size_module){
		return ERROR_MODULE_TOO_MANY;
	}
	
	this->module[ this->num_module ].feature = feature;
	this->module[ this->num_module ].feature->framework = this;
	this->module[ this->num_module ].context = context;
	this->module[ this->num_module ].id =  module_id;
	
	this->num_module++;
	return 0;
}

Error_Id JFramework_add_execution(JFramework* this, Process* process, Process_Context* process_context, void* state, uint32_t thread_level)
{
	uint32_t process_idx = this->num_exec[thread_level];
	if( this->num_exec[thread_level] >= this->size_exec[thread_level]){
		return ERROR_EXEC_TOO_MANY;
	}
	//	if( this->num_exec[thread_level] == 0 ) {
	//		this->call_interval[thread_level] = process->call_interval;
	//	}
	if( this->call_interval[thread_level] == 0 /* inherit */  ) {
		this->call_interval[thread_level] = process->call_interval;
	}
	else {
		if ( process->call_interval != 0 /* inherit */ ){
			if ( this->call_interval[thread_level] != process->call_interval ){
				return ERROR_BAD_CALL_INTERVAL;
			}
		}
	}
	this->execution[thread_level][process_idx].process = process;
	this->execution[thread_level][process_idx].context = process_context;
	this->execution[thread_level][process_idx].context->module = state;
	
	this->num_exec[thread_level]++;
	return SUCCESS;
}

void* JFramework_get_buffer(JFramework* this, Buffer_Id buffer_id)
{
	uint32_t i;
	for ( i = 0; i < this->num_buffer; i++ ) {
		if ( this->buffer[i].id == buffer_id ) {
			return this->buffer[i].payload;
		}
	}
	return NULL; // not found
}

#if !defined(BAF_STAT_DISABLED)
#if !defined(BAF_NUM_STAT)
#define BAF_NUM_STAT 4
#endif
static struct {
	uint32_t passthru_single_count;
	uint32_t passthru_single_error;
	uint32_t passthru_single_blind_count;
	uint32_t passthru_single_blind_error;
} BAF_control_stat;

static struct {
	void * ptr;
	uint32_t size;
} BAF_stat[BAF_NUM_STAT] = {
	{   // statId = 1 -> Control stat
		&BAF_control_stat,
		sizeof(BAF_control_stat)/sizeof(uint32_t)
	}
};
#endif

int32_t JFramework_command(JFramework* this, Message* req, Message* res)
{
	int32_t status = 0;
	uint32_t i;
#if defined(BAF_LOGGER)
	Logger * log = &BAF_logger[LOG_TYPE_PRINT];
	uint32_t j, k, num_process, num_port;
	uint32_t size;
	bool logger_state;
	Ports_Context * ports;
	struct {char*msg; uint32_t data[];} *payload = req->message_data;
#endif
	
	uint32_t id = req->module_id;
	uint32_t sub_id = req->submodule_id;
	uint32_t port_id = (req->message_id >> 24) & 0xff;
#if !defined(BAF_STAT_DISABLED)
	uint32_t stat_id = (req->message_id >> 8) & 0xf;
	uint32_t stat_size = (req->message_id >> 12) & 0xf;
	uint32_t stat_offset = (req->message_id >> 16) & 0xffff;
#endif
	uint32_t mode;
	uint32_t req_message = req->message_id & 0xff;
	const char* ver;
	char* msg;

	res->message_id = req->message_id;
	if ((req_message > MESSAGE_SYS_START) && (req_message != MESSAGE_SYS_LOG_PULL))
	{
		res->message_size = 0;
	}

	switch ( req_message ) {
		case MESSAGE_ADD_STAT:
#if !defined(BAF_STAT_DISABLED)
			if (stat_id < BAF_NUM_STAT) {
				BAF_stat[stat_id].ptr = req->message_data;
			}
			else {
				status = ERROR_INDEX_OUT_OF_RANGE;
			}
#else
			status = ERROR_INVALID_COMMAND;
#endif
			break;
		case MESSAGE_GET_STAT:
#if !defined(BAF_STAT_DISABLED)
			if ((stat_id < BAF_NUM_STAT) && ((stat_size + stat_offset) <= BAF_stat[stat_id].size)) {
				for ( i = 0; i < stat_size; i++ ) {
					((uint32_t*)(res->message_data))[i] = ((uint32_t*) (BAF_stat[stat_id].ptr))[stat_offset + i];
				}
				res->message_size = stat_size * sizeof (uint32_t);
				status = stat_size * sizeof (uint32_t);
			}
			else {
				status = ERROR_INDEX_OUT_OF_RANGE;
			}
#else
			status = ERROR_INVALID_COMMAND;
#endif
			break;
		case MESSAGE_SYS_MUTE_SINGLE:
			mode = this->module[id].context->processes->item[sub_id].mode;
			mode = (mode & ~PROCESS_MODE_MASK) | PROCESS_MODE_MUTE;
			this->module[id].context->processes->item[sub_id].mode = mode;
			break;
		case MESSAGE_SYS_ACTIVE_SINGLE:
			mode = this->module[id].context->processes->item[sub_id].mode;
			mode = (mode & ~PROCESS_MODE_MASK) | PROCESS_MODE_ACTIVE;
			this->module[id].context->processes->item[sub_id].mode = mode;
			break;
		case MESSAGE_SYS_BYPASS_SINGLE:
			mode = this->module[id].context->processes->item[sub_id].mode;
			mode = (mode & ~PROCESS_MODE_MASK) | PROCESS_MODE_BYPASS;
			this->module[id].context->processes->item[sub_id].mode = mode;
			break;
		case MESSAGE_SYS_GEN_SINGLE:
			mode = this->module[id].context->processes->item[sub_id].mode;
			mode = (mode & ~PROCESS_MODE_MASK) | PROCESS_MODE_GEN;
			this->module[id].context->processes->item[sub_id].mode = mode;
			break;
		case MESSAGE_SYS_CAPTURE_IN:
			this->module[id].context->processes->item[sub_id].mode |= PROCESS_MODE_CAPTURE_IN;
			this->module[id].context->processes->item[sub_id].dbg_port = port_id;
			break;
		case MESSAGE_SYS_CAPTURE_OUT:
			this->module[id].context->processes->item[sub_id].mode |= PROCESS_MODE_CAPTURE_OUT;
			this->module[id].context->processes->item[sub_id].dbg_port = port_id;
			break;
#if defined(BAF_LOGGER)
		case MESSAGE_SYS_LOG_DUMP:
			if ( id >= FW_NUM_LOG ) break;
			log = &BAF_logger[id];
			logger_state = log->enabled;
			log->enabled = false;
			Logger_dump((Logger_Type)id);
			log->enabled = logger_state;
			break;
		case MESSAGE_SYS_LOG_PULL:
			if ( id >= FW_NUM_LOG ) {
				status = ERROR_INVALID_LOG;
				break;
			}
			log = &BAF_logger[id];
			if ( log->enabled ) {
				status = ERROR_ENABLED_LOG;
				break;
			}
			size = (req->message_id >> 16) & 0xffff;
			if ((size * sizeof(Log)) > res->message_size )
			{
				size = res->message_size/sizeof(Log);
				if (size == 0) {
					status = ERROR_BUFFER_TOO_SMALL;
					break;
				}
			}
			status = Logger_pull((Logger_Type)id, &size, (uint32_t*)res->message_data );
			res->message_id = (req->message_id & 0xFFFF) | (status << 16);
			status *= sizeof(Log);
			res->message_size = status;
			break;
		case MESSAGE_SYS_LOG_CIRCULAR:
			if ( id >= FW_NUM_LOG ) break;
			log = &BAF_logger[id];
			log->circular = true;
			break;
		case MESSAGE_SYS_LOG_LINEAR:
			if ( id >= FW_NUM_LOG ) break;
			log = &BAF_logger[id];
			log->circular = false;
			break;
		case MESSAGE_SYS_LOG_ENABLE:
			if ( id >= FW_NUM_LOG ) break;
			Logger_reset((Logger_Type)id);
			log = &BAF_logger[id];
			log->enabled = true;
			break;
		case MESSAGE_SYS_LOG_DISABLE:
			if ( id >= FW_NUM_LOG ) break;
			log = &BAF_logger[id];
			log->enabled = false;
			if (log->wrapped && log->circular) {
				log->pull = log->current;
			}
			else {
				log->pull = log->start;
			}

			break;
		case MESSAGE_SYS_LOG_PRINT:
			if ( id >= FW_NUM_LOG ) break;
			if ( req->message_size == 1 ) {
				Logger_msg ((Logger_Type)id, 0, payload->msg);
			}
			else if (req->message_size == 2) {
				Logger_print1 ((Logger_Type)id, 0, payload->msg, payload->data[0]);
			}
			else if (req->message_size == 3) {
				Logger_print ((Logger_Type)id, 0, payload->msg, payload->data[0], payload->data[0]);
			}
			break;
		case MESSAGE_SYS_LOG_FRAMEWORK:
		   id = LOG_TYPE_PRINT;
			Logger_print(LOG_TYPE_PRINT, 0,"Framework dump num_module:%d ptr:%08x\n", this->num_module, (intptr_t)this);
			for (i=0; i<this->num_module; i++){
				Logger_print(LOG_TYPE_PRINT, 0,"Module[%d]:%08x\n", i, (intptr_t)&(this->module[i]));
				Logger_print(LOG_TYPE_PRINT, 0,"nID[%d]:%08x\n", i, this->module[i].id);
				Logger_print(LOG_TYPE_PRINT, 0,"feature[%d]:%08x\n", i, (intptr_t)this->module[i].feature);
				Logger_print(LOG_TYPE_PRINT, 0,"xcontext[%d]:%08x\n", i, (intptr_t)this->module[i].context);
				num_process = this->module[i].context->processes->num_items;
				Logger_print(LOG_TYPE_PRINT, 0,"Processes[%d]:%d\n",i,num_process );
				Logger_print1(LOG_TYPE_PRINT, 0,"              %08x\n", (intptr_t)this->module[i].context->processes);
				for(j=0; j<num_process; j++) {
					Logger_print(LOG_TYPE_PRINT, 0,"process[%d]:%08x\n", j, (intptr_t)&(this->module[i].context->processes->item[j]) );
					ports = this->module[i].context->processes->item[j].ports_in;
					if(ports) {
						num_port = ports->num_items;
						Logger_print(LOG_TYPE_PRINT, 0,"in_port[%d]:%d\n", j, num_port );
						for(k=0; k<num_port; k++) {
							Logger_print(LOG_TYPE_PRINT, 0,"connection[%d]:%08x\n", k, (intptr_t)this->module[i].context->processes->item[j].ports_in->item[k].connection );
							Logger_print(LOG_TYPE_PRINT, 0,"data:  [%d]:%08x\n", k, (intptr_t)this->module[i].context->processes->item[j].ports_in->item[k].payload );
						}
					}
					ports = this->module[i].context->processes->item[j].ports_out;
					if(ports){
						num_port = ports->num_items;
						Logger_print(LOG_TYPE_PRINT, 0,"out_port[%d]:%d\n", j, num_port );
						for(k=0; k<num_port; k++) {
							Logger_print(LOG_TYPE_PRINT, 0,"connection[%d]:%08x\n", k, (intptr_t)this->module[i].context->processes->item[j].ports_out->item[k].connection );
							Logger_print(LOG_TYPE_PRINT, 0,"data:  [%d]:%08x\n", k, (intptr_t)this->module[i].context->processes->item[j].ports_out->item[k].payload );
						}
					}
				}
			}
			Logger_msg(LOG_TYPE_PRINT, 0,"execution:\n");
			for(i=0; i<FW_NUM_THREAD; i++){
				Logger_print1(LOG_TYPE_PRINT, 0, "thread[%d]\n", i);
				for ( j = 0; j < this->num_exec[i]; j++) {
					Logger_print(LOG_TYPE_PRINT, 0,"process[%d]:%08x\n", j, (intptr_t)this->execution[i][j].process);
					Logger_print(LOG_TYPE_PRINT, 0,"xcontext[%d]:%08x\n", j, (intptr_t)this->execution[i][j].context);
				}
			}
			break;
#endif

		case MESSAGE_PASSTHRU_BROADCAST:
		case MESSAGE_PASSTHRU_BROADCAST_BLIND:
			status = ERROR_INVALID_COMMAND;
			for ( id = 0; id < this->num_module; id++ ){
				if( this->module[id].feature->command ) {
					status = this->module[id].feature->command( this->module[id].feature, this->module[id].context->processes, req, res );
				}
			}
			if( MESSAGE_PASSTHRU_BROADCAST_BLIND == req_message ){
				status = 0;
			}
			break;
		case MESSAGE_PASSTHRU_SINGLE:
		case MESSAGE_PASSTHRU_SINGLE_BLIND:
			status = ERROR_INVALID_COMMAND;
			if( this->module[id].feature->command ) {
				status = this->module[id].feature->command( this->module[id].feature, this->module[id].context->processes, req, res );
			}
#if !defined(BAF_STAT_DISABLED)
			if( MESSAGE_PASSTHRU_SINGLE_BLIND == req_message ){
				if (status >= 0){
					BAF_control_stat.passthru_single_blind_count++;
					status = 0;
				}
				else {
					BAF_control_stat.passthru_single_blind_error++;
				}
			}
			else
			{
				if (status >= 0){
					BAF_control_stat.passthru_single_count++;
				}
				else {
					BAF_control_stat.passthru_single_error++;
				}
			}
#endif
			if( MESSAGE_PASSTHRU_SINGLE_BLIND == req_message ){
				if (status >= 0){
					status = 0;
				}
			}

			break;
		case MESSAGE_NOTIFICATION_POLL:
			status = ERROR_INVALID_COMMAND;
			for ( id = 0; id < this->num_module; id++ ){
				if( this->module[id].feature->poll ) {
					status = this->module[id].feature->poll( this->module[id].feature, this->module[id].context->processes, req, res );
				}
			}
			break;
		
		case MESSAGE_VERSION:
			res->message_id = req->message_id;
			ver = BAF_version(NULL);
			if (res->message_data == NULL)
			{
				status = ERROR_BUFFER_NULL;
				break;
			}
			msg = (char*)res->message_data;
			if ( res->message_size < 12 ) {
				status = ERROR_BUFFER_TOO_SMALL;
				break;
			}
			for ( i = 0; i < 9; i++ ) *msg++ = *ver++; // version string length is 9
			res->message_size = 12; // roundup to 32bit word size
			status = 12;
			break;
		
		default:
			status = ERROR_INVALID_COMMAND;
			break;
	}
	return status;
	//todo: check for feature or module broadcast
}

void JFramework_register_notify(JFramework* this, BAF_Func_Notify func)
{
	uint32_t id;
	for ( id = 0; id < this->num_module; id++ ){
		if( this->module[id].feature->register_notify ) {
		    this->module[id].feature->register_notify( this->module[id].feature, JFramework_notify, this );
		}
	}
	this->notify = func;
}

void JFramework_notify(JFramework* this, Message* msg)
{
	BAF_Message rsp;
	rsp.id = MESSAGE_NOTIFICATION | ( msg->module_id << 8 );
	rsp.size = msg->message_size;
	rsp.data = (uint32_t*)msg->message_data;
	    this->notify(NULL, &rsp);
}

void JFramework_init_exec(JFramework* this)
{
	uint32_t i;
	
	for ( i=0; i<this->num_module; i++ ){
		if ( this->module[i].feature->init ) {
			this->module[i].feature->init(NULL, this->module[i].feature->processes, this->module[i].context->processes);
		}
	}
}

Module* JFramework_get_module(JFramework* this, Module_Id module_id)
{
	uint32_t i;
	for ( i = 0; i < this->num_module; i++ ){
		if ( JModule_Bitfield_get_module(this->module[i].id) == module_id ) {
			return &this->module[i];
		}
	}
	return NULL; // not found
}

