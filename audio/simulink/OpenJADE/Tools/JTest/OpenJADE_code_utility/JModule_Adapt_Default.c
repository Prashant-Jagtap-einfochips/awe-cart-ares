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
 * @file JModule_Adapt_$$JModule$$.h
 * 
 * @brief Implementation of the Class JModule_Adapt_$$JModule$$       
 *
 * @author: cb1037694                     
 ****************************************************/

#include "JModule_Adapt_$$JModule$$.h"

static void JModule_Adapt_init(JModule* this, Processes* processes, Processes_Context* context)
{
}

static void JModule_Adapt_send(JModule* this, Processes_Context* context, Message* req, Message* rsp)
{
	void * req_msg = req->message_data;
	void * rsp_msg = rsp->message_data;
//  Send message to the algorithm
//	HandleAsdIdRequest( req->message_data, 16, rsp->message_data, 16 );
}

$$ADAPTER_PROCESSES$$

static void JModule_Adapt_register_notify(JModule* this, JModule_Func_Notify notify)
{
	
} 

static void JModule_Adapt_poll( JModule* this )
{
	
} 

static void JModule_Adapt_notify( JModule* this )
{
	
} 


void JModule_Adapt_$$JModule$$_create(JModule* this)
{
$$ADAPTER_STRINGS$$
	this->init = JModule_Adapt_init;
	this->command = JModule_Adapt_send;
	this->register_notify = JModule_Adapt_register_notify;
	this->poll = JModule_Adapt_poll;
	this->notify = JModule_Adapt_notify;
}

