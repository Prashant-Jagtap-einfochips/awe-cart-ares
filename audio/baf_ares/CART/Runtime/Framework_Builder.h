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
 * @file Framework_Builder.h
 * 
 * @ingroup Framework_Builder
 *
 * @brief Implementation of the Class Framework_Builder       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Framework_Builder__INCLUDED_)
#define BC_Framework_Builder__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include "Resource.h"
#include "Error.h"

#include "JFramework.h"
#include "JModule_List.h"
#include "Heap.h"
#include "Audio_Graph.h"


/**
 * @ingroup Framework_Builder
 *
 * @brief Constructs and assembles parts of the framework
 * Defines and keeps track of the framework it creates
 * Provides an interface for retrieving the framework.
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */
typedef struct Framework_Builder
{
	Heap* heap;
	Audio_Graph* audio_graph;
	JFramework* framework;
	JModule_List* features;
} Framework_Builder;

/**
 * @ingroup Framework_Builder
 *
 * @brief @param this
 */
JFramework* Framework_Builder_get_framework(Framework_Builder* this);

/**
 * @ingroup Framework_Builder
 *
 * @brief @param this
 * 
 * @param framework
 * 
 * @param heap
 * 
 * @param graph
 * 
 * @param features
 */
void Framework_Builder_init(Framework_Builder* this, JFramework* framework, Heap* heap, Audio_Graph* graph, JModule_List* features);

/**
 * @ingroup Framework_Builder
 *
 * @brief @param this
 */
Error_Id Framework_Builder_build_framework(Framework_Builder* this);

/**
 * @ingroup Framework_Builder
 *
 * @brief @param this
 * 
 * @param resource
 * 
 * @param graph
 * 
 * @param features
 */
int32_t Framework_Builder_resource(Framework_Builder* this, Resource* resource, Audio_Graph* graph, JModule_List* features);

/**
 * @ingroup Framework_Builder
 *
 * @brief @param this
 * 
 * @param process_id
 * 
 * @param thread_level
 */
Error_Id Framework_Builder_add_execution(Framework_Builder* this, Process_Id process_id, uint32_t thread_level);

/**
 * @ingroup Framework_Builder
 *
 * @brief @param this
 * 
 * @param module_id
 */
Error_Id Framework_Builder_add_module(Framework_Builder* this, Module_Id module_id);

/**
 * @ingroup Framework_Builder
 *
 * @brief Search for buffer in the framework
 * If not found, create and add buffer to the framework
 * Attach buffer to the port
 * 
 * @param this
 * 
 * @param port_id
 * 
 * @param buffer_id
 */
Error_Id Framework_Builder_add_buffer(Framework_Builder* this, Port_Id port_id, Buffer_Id buffer_id);

/**
 * @ingroup Framework_Builder
 *
 * @brief @param this
 * 
 * @param connection_item
 */
Connection* Framework_Builder_add_connection(Framework_Builder* this, Connection_Item* connection_item); 

/**
 * @ingroup JModule_List
 *
 * @brief @param feature_list
 * 
 * @param feature_id
 */
JModule* JModule_List_get_feature(JModule_List* feature_list, JModule_Id feature_id); 

#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Framework_Builder__INCLUDED_)*/
