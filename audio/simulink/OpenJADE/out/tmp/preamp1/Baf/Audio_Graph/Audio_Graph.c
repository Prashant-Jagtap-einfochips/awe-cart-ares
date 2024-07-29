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
 * @file 
 * @date 07/17/24
 * @brief Implementation of Module List
 *
 * @details <OPTIONAL: Some details about file>
 *
 * @see <OPTIONAL: Links to reference/Another software component>
 *
 */

#include "_BAF.h"
#include "Module_List.h"
#include "_JModule_Id.h"
#include "JModule_Adapt_BasicAudio_Target.h"
#include "JModule_Adapt_sys.h"


static Module_List Module_List_obj = {
    .num_items = 2,
    {
        JModule_Bitfield_module(JMODULE_BASICAUDIO_TARGET, CONTEXT_0),
        JModule_Bitfield_module(JMODULE_SYS, CONTEXT_0)
    }
};

/**
 * @file 
 * @date 07/17/24
 * @brief Implementation of Connection List
 *
 * @details <OPTIONAL: Some details about file>
 *
 * @see <OPTIONAL: Links to reference/Another software component>
 *
 */

#include "Connection_List.h"


#define Connection_List_get_connection(connection_list, connection_id) &(connection_list)->item[connection_id]


static Connection_List Connection_List_obj = {
    .num_items = 2,
    .item = {
                 {
                     JModule_Bitfield_port (PORT_DIRECTION_OUT, PORT_OUT_SYS_CR1_IN__OUT1, PROCESS_SYS_CR1_IN, JMODULE_SYS, CONTEXT_0),
                     JModule_Bitfield_port (PORT_DIRECTION_IN, PORT_IN_BASICAUDIO_TARGET_STEP0_IN1, PROCESS_BASICAUDIO_TARGET_STEP0, JMODULE_BASICAUDIO_TARGET, CONTEXT_0)
                 },
                 {
                     JModule_Bitfield_port (PORT_DIRECTION_OUT, PORT_OUT_BASICAUDIO_TARGET_STEP0_OUT1, PROCESS_BASICAUDIO_TARGET_STEP0, JMODULE_BASICAUDIO_TARGET, CONTEXT_0),
                     JModule_Bitfield_port (PORT_DIRECTION_IN, PORT_IN_SYS_CR1_OUT__IN1, PROCESS_SYS_CR1_OUT, JMODULE_SYS, CONTEXT_0)
                 },
    }
};
/**
 * @file 
 * @date 07/17/24
 * @brief Implementation of Execution List
 *
 * @details <OPTIONAL: Some details about file>
 *
 * @see <OPTIONAL: Links to reference/Another software component>
 *
 */

#include "Execution_List.h"


static Execution_List Execution_List_0 = {
    .num_items = 3,
    {
        JModule_Bitfield_process(PROCESS_SYS_CR1_IN, JMODULE_SYS,  CONTEXT_0),
        JModule_Bitfield_process(PROCESS_BASICAUDIO_TARGET_STEP0, JMODULE_BASICAUDIO_TARGET,  CONTEXT_0),
        JModule_Bitfield_process(PROCESS_SYS_CR1_OUT, JMODULE_SYS,  CONTEXT_0),
    }
};

/**
 * @file 
 * @date 07/17/24
 * @brief Implementation of Port List
 *
 * @details <OPTIONAL: Some details about file>
 *
 * @see <OPTIONAL: Links to reference/Another software component>
 *
 */

#include "Port_List.h"


static Port_List Port_List_obj = {
    .num_items = 4,
    .item = {
        { JModule_Bitfield_port (PORT_DIRECTION_OUT, PORT_OUT_SYS_CR1_IN__OUT1, PROCESS_SYS_CR1_IN, JMODULE_SYS, CONTEXT_0), BUFFER_0},
        { JModule_Bitfield_port (PORT_DIRECTION_IN, PORT_IN_BASICAUDIO_TARGET_STEP0_IN1, PROCESS_BASICAUDIO_TARGET_STEP0, JMODULE_BASICAUDIO_TARGET, CONTEXT_0), BUFFER_0},
        { JModule_Bitfield_port (PORT_DIRECTION_OUT, PORT_OUT_BASICAUDIO_TARGET_STEP0_OUT1, PROCESS_BASICAUDIO_TARGET_STEP0, JMODULE_BASICAUDIO_TARGET, CONTEXT_0), BUFFER_1},
        { JModule_Bitfield_port (PORT_DIRECTION_IN, PORT_IN_SYS_CR1_OUT__IN1, PROCESS_SYS_CR1_OUT, JMODULE_SYS, CONTEXT_0), BUFFER_1},
    }
};
/**
 * @file 
 * @date 07/17/24
 * @brief Implementation of Audio Graph
 *
 * @details <OPTIONAL: Some details about file>
 *
 * @see <OPTIONAL: Links to reference/Another software component>
 *
 */

#include "Audio_Graph.h"


static Audio_Graph Audio_Graph_obj = {
    .modules = &Module_List_obj,
    .connections = &Connection_List_obj,
    .ports = &Port_List_obj,
    .num_exec = 1,
    .execution = {
        &Execution_List_0,
    }
};

/**
 * @file 
 * @date 07/17/24
 * @brief Implementation of the Class JModule_List
 *
 * @details <OPTIONAL: Some details about file>
 *
 * @see <OPTIONAL: Links to reference/Another software component>
 *
 */

#include "JModule_List.h"



static JModule_List JModule_List_obj = {
    .num_items = 2,
    .item = {
        &JModule_Basicaudio_target_obj,
        &JModule_Sys_obj,
    }
};

static _BAF BAF_obj = {
	.feature_list = &JModule_List_obj,
	.audio_graph = &Audio_Graph_obj
};

BAF* pBAF_obj = &BAF_obj;
