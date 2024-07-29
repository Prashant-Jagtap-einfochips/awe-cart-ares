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
 * @file JModule_Adapt_Demoalg.h
 * 
 * @brief Implementation of the Class JModule_Adapt_Demoalg       
 *
 * @author: cb1037694                     
 ****************************************************/

#include "JModule_Adapt_Demoalg.h"

/* Model entry point functions */
extern void Demoalg_initialize(void);

static void JModule_Adapt_init(JModule* this, Processes* processes, Processes_Context* context)
{
    Demoalg_initialize();
}

static void JModule_Adapt_send(JModule* this, Processes_Context* context, Message* req, Message* rsp)
{
	void * req_msg = req->message_data;
	void * rsp_msg = rsp->message_data;
//  Send message to the algorithm
//	HandleAsdIdRequest( req->message_data, 16, rsp->message_data, 16 );
}

/* External inputs (root inport signals with auto storage) */
typedef struct {
  float In1[128];                   /* '<Root>/In1' */
} ExtU_Demoalg_T;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  float Out1[128];                  /* '<Root>/Out1' */
} ExtY_Demoalg_T;

/* External inputs (root inport signals with auto storage) */
extern ExtU_Demoalg_T Demoalg_U;

/* External outputs (root outports fed by signals with auto storage) */
extern ExtY_Demoalg_T Demoalg_Y;

extern void Demoalg_step(void);

static void JModule_Adapt_process0(JModule* this, Process* process, Process_Context* process_ctx, uint32_t thread_level)
{
	Ports *port_in = process->ports_in;
	Ports *port_out = process->ports_out;
	Ports_Context *port_in_ctx = process_ctx->ports_in;
	Ports_Context *port_out_ctx = process_ctx->ports_out;
	uint32_t i;
	float * src = port_in_ctx->item[0].payload;
	float * dst = port_out_ctx->item[0].payload;
//	for(i = 0; i < port_in->port[0].frame_size * port_in->port[0].num_chan ; i++ ){ // passthru example
//		dst[i] = src[i];
//	}
	for(i = 0; i < port_in->port[0].frame_size * port_in->port[0].num_chan ; i++ ){ 
		Demoalg_U.In1[i] = src[i];
	}
    Demoalg_step();
	for(i = 0; i < port_out->port[0].frame_size * port_out->port[0].num_chan ; i++ ){ 
		dst[i] = Demoalg_Y.Out1[i];
	}
    
}

static void JModule_Adapt_register_notify(JModule* this, JModule_Func_Notify notify)
{
	
} 

static void JModule_Adapt_poll( JModule* this )
{
	
} 

static void JModule_Adapt_notify( JModule* this )
{
	
} 


void JModule_Adapt_Demoalg_create(JModule* this)
{
	this->processes->process[0].exec = JModule_Adapt_process0;
	this->init = JModule_Adapt_init;
	this->command = JModule_Adapt_send;
	this->register_notify = JModule_Adapt_register_notify;
	this->poll = JModule_Adapt_poll;
	this->notify = JModule_Adapt_notify;
}

