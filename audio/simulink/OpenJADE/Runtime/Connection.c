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
 * @file Connection.h
 * 
 * @brief Implementation of the Class Connection       
 *
 * @author: cb1037694                     
 ****************************************************/

#include "Connection.h"

bool Connection_create(Connection* this, uint32_t in_start, uint32_t out_start)
{
	this->in_count = in_start;
	this->out_count = out_start; 
	this->in_start  = in_start;
	this->out_start = out_start;
	this->empty = BAF_Semaphore_create(in_start);
	this->full  = BAF_Semaphore_create(out_start);
	return true;
}

void* Connection_alloc(Connection* this, void* buf[])
{
	BAF_Semaphore_pend (this->full);
	return 	(buf == NULL) ? NULL : buf[this->out_count];
}

void* Connection_receive(Connection* this, void* buf[])
{
	BAF_Semaphore_pend (this->empty);
	return 	(buf == NULL) ? NULL : buf[this->in_count];
}

void Connection_send(Connection* this)
{
	this->in_count = (this->in_count + 1) & 1; // two level fifo
	BAF_Semaphore_post(this->empty);
}

void Connection_free(Connection* this)
{
	this->out_count = (this->out_count + 1) & 1;
	BAF_Semaphore_post(this->full);
}

