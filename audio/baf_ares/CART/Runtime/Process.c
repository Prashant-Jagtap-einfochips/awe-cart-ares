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
 * @file JProcess.h
 * 
 * @brief Implementation of the Class Process       
 *
 * @author: cb1037694                     
 ****************************************************/

#include "JProcess.h"
#include "BAF_Callback.h"
#include "Port_Context.h"
/**
 * API that invokes the exec pointer to function. The API first checks the availability of the buffers
 * associated with the input/output Ports, and makes these buffers available after the exec pointer to
 * function execution.
 */
void Process_exec(Process* this, Process_Context* context, uint32_t thread_level, void* arg)
{
	Ports *ports_in = this->ports_in;
	Ports *ports_out = this->ports_out;
	Ports_Context *ports_in_ctx = context->ports_in;
	Ports_Context *ports_out_ctx = context->ports_out;
	uint32_t i, j;
#if defined(BAF_CONNECTION)
	for( i = 0; ports_in && (i < ports_in->num_items) ; i++ ) {
	    ports_in_ctx->item[i].payload= Connection_receive(ports_in_ctx->item[i].connection, ports_in_ctx->item[i].fifo);
	}
	for( i = 0; ports_out && (i <  ports_out->num_items) ; i++ ) {
	    ports_out_ctx->item[i].payload = Connection_alloc(ports_out_ctx->item[i].connection, ports_out_ctx->item[i].fifo);
	}
#endif
	if(context->mode == PROCESS_MODE_ACTIVE){
	    this->exec( this->feature, this, context, thread_level, arg );
	} 
	else if(context->mode == PROCESS_MODE_BYPASS){
	    uint8_t *in_buf, *out_buf;
	    uint32_t in_size, out_size;
	    if (ports_in && ports_out){
	        // bypass only port 0 until other use cases
	        in_size = ports_in->port[0].frame_size * ports_in->port[0].num_chan * ports_in->port[0].sample_size; 
	        out_size = ports_out->port[0].frame_size * ports_out->port[0].num_chan * ports_out->port[0].sample_size; 
	        in_buf = (uint8_t*)ports_in_ctx->item[0].payload; 
	        out_buf = (uint8_t*)ports_out_ctx->item[0].payload; 
	        for ( i = 0, j = 0; in_buf && out_buf && (j < out_size); j++, i++ ) {
	            if ( i == in_size ) { // wrap input to the output
	                i = 0;
	            }
	            out_buf[j] = in_buf[i];
	        }
	    }
	}
	else if(context->mode == PROCESS_MODE_GEN){
	    float in_buf[8] = {0, .7, 1, .7, 
	                        0,-.7,-1,-.7};
	    uint32_t in_size = 8;
	    float *out_buf;
	    uint32_t out_size;
	    if (ports_out){
	        out_size = ports_out->port[0].frame_size * ports_out->port[0].num_chan * ports_out->port[0].sample_size/sizeof(float); 
	        out_buf = (float*)ports_out_ctx->item[0].payload; 
	        for ( i = 0, j = 0; (out_buf != NULL) && (j < out_size); j++, i++ ) {
	            if ( i == in_size ) { // wrap input to the output
	                i = 0;
	            }
	            out_buf[j] = in_buf[i]*0.1;
	        }
	    }
	}
	else if(context->mode == PROCESS_MODE_MUTE){
	    uint32_t out_size;
	    uint8_t* out_buf;
	    for( i = 0; ports_out && (i < ports_out->num_items) ; i++ ) {
	        out_size = ports_out->port[i].frame_size * ports_out->port[i].num_chan * ports_out->port[i].sample_size; 
	        out_buf = (uint8_t*)ports_out_ctx->item[i].payload; 
	        for ( j = 0; out_buf && (j < out_size); j++ ) {
	            out_buf[j] = 0;
	        }
	    }
	}
#if defined(BAF_LOGGER)
	if(context->mode & PROCESS_MODE_CAPTURE_IN){
	    uint32_t in_size;
	    void* in_buf;
		 uint8_t dbg_port = context->dbg_port;
	    if( ports_in && (dbg_port < ports_in->num_items))	 {
			in_size = ports_in->port[dbg_port].frame_size * ports_in->port[dbg_port].num_chan * ports_in->port[dbg_port].sample_size; 
			in_buf = ports_in_ctx->item[dbg_port].payload; 
			Logger_print(LOG_TYPE_CAPTURE, thread_level, "CPort:%d t:%10u\n", 0, in_size>>2);
			if( in_buf ) {
				Logger_copy(LOG_TYPE_CAPTURE, in_buf, in_size);
			}
		 }
		 context->mode &= ~PROCESS_MODE_CAPTURE_IN;
	}
	if(context->mode & PROCESS_MODE_CAPTURE_OUT){
	    uint32_t out_size;
	    uint8_t* out_buf;
		 uint8_t dbg_port = context->dbg_port;
	    if(ports_out && (dbg_port < ports_out->num_items)) {
			out_size = ports_out->port[dbg_port].frame_size * ports_out->port[dbg_port].num_chan * ports_out->port[dbg_port].sample_size; 
			out_buf = (uint8_t*)ports_out_ctx->item[dbg_port].payload; 
			Logger_print(LOG_TYPE_CAPTURE, thread_level, "CPort:%d t:%10u\n", 0, out_size>>2);
			if( out_buf ) {
				Logger_copy(LOG_TYPE_CAPTURE, out_buf, out_size);
			}
		 }
		 context->mode &= ~PROCESS_MODE_CAPTURE_OUT;
	}
#endif
#if defined(BAF_CONNECTION)
	for( i = 0; ports_in && (i < ports_in->num_items) ; i++ ) {
	    Connection_free(ports_in_ctx->item[i].connection);
	}
	for( i = 0; ports_out && (i < ports_out->num_items) ; i++ ) {
	    Connection_send(ports_out_ctx->item[i].connection);
	}
#endif
}

/**
 * API that returns pointer to an input port specified by its index. The API returns NULL if the index
 * is out of range.
 */
Port* Process_get_port_in(Process* this, uint32_t port_index)
{
	return   &(this->ports_in->port[port_index]);
}

/**
 * API that returns pointer to an output port specified by its index. The API returns NULL if the
 * index is out of range.
 */
Port* Process_get_port_out(Process* this, uint32_t port_index)
{
	return   &(this->ports_out->port[port_index]);
} 

