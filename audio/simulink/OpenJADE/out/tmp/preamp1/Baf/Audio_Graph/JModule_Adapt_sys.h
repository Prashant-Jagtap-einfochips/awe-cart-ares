/*******************************************************************************
*  BOSE CORPORATION
*  COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
*  This program may not be reproduced, in whole or in part in any
*  form or any means whatsoever without the written permission of:
*     BOSE CORPORATION
*     The Mountain
*     Framingham, MA 01701-9168
******************************************************************************/

/**
 * @file JModule_Adapt_sys.h
 * @date 07/17/24
 * @brief 
 *
 * @details <OPTIONAL: Some details about file>
 *
 * @see <OPTIONAL: Links to reference/Another software component>
 *
 */

#if !defined(BC_JModule_Adapt_sys__INCLUDED_)
#define BC_JModule_Adapt_sys__INCLUDED_




#ifdef __cplusplus
extern "C" {
#endif



#include <stdlib.h>

#include <stdint.h>
#include "Processes_Context.h"
#include "Message.h"
#include "JModule.h"


/**
 * @ingroup JModule_Adapt_Sys
 *
 * @brief @param this
 */



#ifdef __cplusplus
}
#endif

#endif /*!defined(BC_JModule_Adapt_sys__INCLUDED_)*/
/**
 * @file JModule_Desc_sys
 * @date 07/17/24
 * @brief Declaration of the Enumeration JModule_Desc_sys
 *
 * @details <OPTIONAL: Some details about file>
 *
 * @see <OPTIONAL: Links to reference/Another software component>
 *
 */

#if !defined(BC_JModule_Desc_sys__INCLUDED_)
#define BC_JModule_Desc_sys__INCLUDED_




#ifdef __cplusplus
extern "C" {
#endif



typedef enum
{
	JMODULE_SYS_NUM_PROCESSES = 2,
	JMODULE_SYS_STATE_SIZE = 0,
	JMODULE_SYS_PARAM_SIZE = 0
} JModule_Desc_Sys;


typedef enum
{
    PORT_IN_SYS_CR1_OUT__IN1
} PORT_SYS_CR1_OUT_IN;


typedef enum
{
    PORT_OUT_SYS_CR1_IN__OUT1
} PORT_SYS_CR1_IN_OUT;


typedef enum
{
    PROCESS_SYS_CR1_IN__OUT1 = 0,
    PROCESS_SYS_CR1_OUT__IN1 = 1
} PROCESS_JCASENAME_SYS;


typedef enum
{
    PROCESS_SYS_CR1_IN,
    PROCESS_SYS_CR1_OUT
} PROCESS_JMODULENAME_SYS;


#ifdef __cplusplus
}
#endif

#endif /*!defined(BC_JModule_Desc_sys__INCLUDED_)*/
/**
 * @file JModule_Adapt_sys.h
 * @date 07/17/24
 * @brief 
 *
 * @details <OPTIONAL: Some details about file>
 *
 * @see <OPTIONAL: Links to reference/Another software component>
 *
 */

void BAF_Callback_sys_process(uint32_t proc_Id, uint32_t frame_size, uint32_t num_chan_in, uint32_t num_chan_out, void* src, void* dst);

static void JModule_Adapt_Sys_init(JModule* this, Processes* processes, Processes_Context* context)
{
    this = (this == NULL) ? (NULL) : (this);
    processes = (processes == NULL) ? (NULL) : (processes);
    context = (context == NULL) ? (NULL) : (context);
}

static void JModule_Adapt_Sys_cr1_in_port_process(JModule* this, Process* process, Process_Context* process_ctx, uint32_t thread_level, void* arg)
{
    Ports *port_out = process->ports_out;
    Ports_Context *port_out_ctx = process_ctx->ports_out;
    void* dst;

    this = (this == NULL) ? (NULL) : (this);
    thread_level = (thread_level == 0) ? (0) : (thread_level);
    dst =  port_out_ctx->item[0].payload;
    BAF_Callback_sys_process(PROCESS_SYS_CR1_IN__OUT1/*proc_id*/, port_out->port[0].frame_size,
	    0 /* num_chan_in*/, port_out->port[0].num_chan, NULL, dst);
}

static void JModule_Adapt_Sys_cr1_out_port_process(JModule* this, Process* process, Process_Context* process_ctx, uint32_t thread_level, void* arg)
{
    Ports *port_in = process->ports_in;
    Ports_Context *port_in_ctx = process_ctx->ports_in;
    void* src;

    this = (this == NULL) ? (NULL) : (this);
    thread_level = (thread_level == 0) ? (0) : (thread_level);
    src =  port_in_ctx->item[0].payload;
    BAF_Callback_sys_process(PROCESS_SYS_CR1_OUT__IN1/*proc_id*/, port_in->port[0].frame_size,
	    port_in->port[0].num_chan, 0 /* num_chan_out*/, src, NULL);
}

static void JModule_Adapt_sys_register_notify(JModule* this, JModule_Func_Notify* notify)
{
    this = (this == NULL) ? (NULL) : (this);
    notify = (notify == NULL) ? (NULL) : (notify);
}
static void JModule_Adapt_Sys_create(JModule* this)
{
    this->processes->process[0].exec = JModule_Adapt_Sys_cr1_in_port_process;
    this->processes->process[1].exec = JModule_Adapt_Sys_cr1_out_port_process;
    this->init = JModule_Adapt_Sys_init;
    this->command = NULL;
    this->register_notify = NULL;
}

/**
 * @file JModule_Desc_sys
 * @date 07/17/24
 * @brief Implementation of the Class JModule_Desc_sys
 *
 * @details <OPTIONAL: Some details about file>
 *
 * @see <OPTIONAL: Links to reference/Another software component>
 *
 */


#include "JModule.h"
#include "_JModule_Id.h"
#include "JModule_Bitfield.h"


static Ports JModule_Sys_process_cr1_in_out_port = {
    .num_items = 1,
    {{
        .data_type = PORT_DATA_REAL32,
        .direction = PORT_DIRECTION_OUT,
        .frame_size = 0,
        .num_chan = 0,
        .sample_rate = 0,
        .sample_size = 4,
        .passthru_port = 0
    }}
};

static Ports JModule_Sys_process_cr1_out_in_port = {
    .num_items = 1,
    {{
        .data_type = PORT_DATA_REAL32,
        .direction = PORT_DIRECTION_IN,
        .frame_size = 0,
        .num_chan = 0,
        .sample_rate = 0,
        .sample_size = 4,
        .passthru_port = 0
    }}
};


static Processes JModule_Sys_processes = {
    .num_items = 2,
    {{
        .ports_in = NULL,
        .ports_out = &JModule_Sys_process_cr1_in_out_port,
        .call_interval = 1,
        .exec = NULL,
        .feature = NULL
    },{
        .ports_in = &JModule_Sys_process_cr1_out_in_port,
        .ports_out = NULL,
        .call_interval = 1,
        .exec = NULL,
        .feature = NULL
    }}
};


static JModule JModule_Sys_obj = {
    .id = JMODULE_SYS,
    .create = JModule_Adapt_Sys_create, 
    .processes = &JModule_Sys_processes
};
