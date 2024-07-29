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
 * @file Port.h
 * 
 * @brief Implementation of the Class Port       
 *
 * @author: cb1037694                     
 ****************************************************/

#include "Port.h"

Port_Error Port_compare(Port* this, Port* p)
{
	if (this->frame_size == 0 && p->frame_size == 0){
	    return PORT_ERROR_FRAME_SIZE;
	}
	if (this->frame_size != p->frame_size ){
	    return PORT_ERROR_FRAME_SIZE;
	}
	if (this->num_chan == 0 && p->num_chan == 0){
	    return PORT_ERROR_FORMAT;
	}
	if (this->num_chan != p->num_chan ){
	    return PORT_ERROR_FORMAT;
	}
	if (this->sample_rate != p->sample_rate ){
	    return PORT_ERROR_FORMAT;
	}
	if (this->sample_size != p->sample_size ){
	    return PORT_ERROR_FORMAT;
	}
	return  PORT_SUCCESS;
}

bool Port_setup(Port* this, uint32_t frame_size, uint32_t num_chan, uint32_t sample_size, uint32_t sample_rate, Port_Data data_type)
{
	this->frame_size = frame_size;
	this->num_chan = num_chan;
	this->sample_size = sample_size;
	this->sample_rate = sample_rate;
	this->data_type = data_type;
	this->block_size = num_chan * frame_size * sample_size;
	return true;
}

uint32_t Port_block_size(Port* this)
{
	return ( this->frame_size * this->num_chan * this->sample_size );
} 

