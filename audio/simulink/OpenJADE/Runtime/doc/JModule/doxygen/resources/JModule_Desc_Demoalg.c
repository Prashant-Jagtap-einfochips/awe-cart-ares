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
 * @file JModule_Desc_Demoalg.h
 * 
 * @brief Implementation of the Class JModule_Demoalg_Param       
 *
 * @author: cb1037694                     
 ****************************************************/

#include "JModule_Desc_Demoalg.h"


/****************************************************
 *  JModule_Demoalg.c                                         
 *  Implementation of the Class JModule_Demoalg       
 *  Original author: cb1037694                     
 ****************************************************/

#include "JModule.h"
#include "JModule_Id.h"
#include "JModule_Bitfield.h"
#include "JModule_Desc_Demoalg.h"

static Ports JModule_process0_port_in = {
	.num_items = 1,
	{{ // in
		.data_type = PORT_DATA_REAL32,
		.direction = PORT_DIRECTION_IN,
		.frame_size = 32,
		.num_chan = 4,
		.sample_rate = 44100,
		.sample_size = sizeof(float)
	}}
};

static Ports JModule_process0_port_out = {
	.num_items = 1,
	{{
		.data_type = PORT_DATA_REAL32,
		.direction = PORT_DIRECTION_OUT,
		.frame_size = 32,
		.num_chan = 4,
		.sample_rate = 44100,
		.sample_size = sizeof(float)
	}}
};

static Processes JModule_processes = {
	.num_items = JMODULE_DEMOALG_NUM_PROCESSES,
	{{
		.call_interval = 1,
		.ports_in = &JModule_process0_port_in,
		.ports_out = &JModule_process0_port_out
	}}
};

/* Parameter Data block */
static JModule_Demoalg_State JModule_State_obj;
static JModule_Demoalg_Param JModule_Param_obj;

extern void JModule_Adapt_Demoalg_create();
JModule JModule_Demoalg_obj = {
	.id = JMODULE_DEMOALG,
	.processes = &JModule_processes,
	.create = JModule_Adapt_Demoalg_create,
	.state = &JModule_State_obj,
	.param = &JModule_Param_obj
};
