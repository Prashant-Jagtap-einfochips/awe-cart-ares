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
 * @file Ports.h
 * 
 * @brief Implementation of the Class Ports       
 *
 * @author: cb1037694                     
 ****************************************************/

#include "Ports.h"

Port_Error Ports_compare_merge(Port* this, Port* p, Ports* thisPorts, Ports* ports)
{
	if (this->frame_size == 0 ){
		if (p->frame_size != 0 ) {
			this->frame_size = p->frame_size;
		}
		else if (this->passthru_port != 0){
			this->frame_size = thisPorts->port[this->passthru_port-1].frame_size;
		}
		else if (p->passthru_port != 0){
			p->frame_size = ports->port[p->passthru_port-1].frame_size;
			this->frame_size = p->frame_size;
		}
		else{
			return PORT_ERROR_FORMAT;
		}
	}
	if (p->frame_size == 0 ){
		if (this->frame_size != 0 ) {
			p->frame_size = this->frame_size;
		}
		else if (p->passthru_port != 0){
			p->frame_size = ports->port[p->passthru_port-1].frame_size;
		}
		else{
			return PORT_ERROR_FORMAT;
		}
	}
	if (this->frame_size != p->frame_size ){
		return PORT_ERROR_FRAME_SIZE;
	}
	
	
	if (this->num_chan == 0 ){
		if (p->num_chan != 0 ) {
			this->num_chan = p->num_chan;
		}
		else if (this->passthru_port != 0){
			this->num_chan = thisPorts->port[this->passthru_port-1].num_chan;
		}
		else if (p->passthru_port != 0){
			p->num_chan = ports->port[p->passthru_port-1].num_chan;
			this->num_chan = p->num_chan;
		}
		else{
			//return PORT_ERROR_FORMAT; port with no channels
		}
	}
	if (p->num_chan == 0 ){
		if (this->num_chan != 0 ) {
			p->num_chan = this->num_chan;
		}
		else if (p->passthru_port != 0){
			p->num_chan = ports->port[p->passthru_port-1].num_chan;
		}
		else{
			//return PORT_ERROR_FORMAT; port with no channels
		}
	}
	if (this->num_chan != p->num_chan ){
		return PORT_ERROR_FORMAT;
	}
	
	if (this->sample_rate == 0 ){
		if (p->sample_rate != 0 ) {
			this->sample_rate = p->sample_rate;
		}
		else if (this->passthru_port != 0){
			this->sample_rate = thisPorts->port[this->passthru_port-1].sample_rate;
		}
		else if (p->passthru_port != 0){
			p->sample_rate = ports->port[p->passthru_port-1].sample_rate;
			this->sample_rate = p->sample_rate;
		}
		else{
			return PORT_ERROR_FORMAT;
		}
	}
	if (p->sample_rate == 0 ){
		if (this->sample_rate != 0 ) {
			p->sample_rate = this->sample_rate;
		}
		else if (p->passthru_port != 0){
			p->sample_rate = ports->port[p->passthru_port-1].sample_rate;
		}
		else{
			return PORT_ERROR_FORMAT;
		}
	}
	if (this->sample_rate != p->sample_rate ){
		return PORT_ERROR_FORMAT;
	}
	
	if (this->sample_size != p->sample_size ){
		return PORT_ERROR_FORMAT;
	}
	if (this->direction == p->direction ){
		return PORT_ERROR_DIRECTION;
	}
	
	return  PORT_SUCCESS;
} 

