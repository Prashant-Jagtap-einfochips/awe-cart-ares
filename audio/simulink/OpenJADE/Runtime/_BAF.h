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
 * @file _BAF.h
 * 
 * @ingroup _BAF
 *
 * @brief Implementation of the Class _BAF       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC__BAF__INCLUDED_)
#define BC__BAF__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include "Resource.h"
#include "JFramework.h"
#include "Framework_Builder.h"
#include "Heap.h"
#include "Logger.h"
#include "JModule_List.h"
#include "Audio_Graph.h"


/**
 * @ingroup _BAF
 *
 * @brief This is the internal structure that holds the Bose Audio Framework (BAF) details.
 * This structure is not visible to the Customer Integrator and is hidden through the BAF object.
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */
typedef struct _BAF
{

	/**
	 * @ingroup _BAF
	 *
	 * @brief Statically allocated Framework object
	 */
	JFramework framework;

	/**
	 * @ingroup _BAF
	 *
	 * @brief Statically allocated Framework_Builder
	 */
	Framework_Builder framework_builder;

	/**
	 * @ingroup _BAF
	 *
	 * @brief Statically allocated Heap management object
	 */
	Heap heap;

	/**
	 * @ingroup _BAF
	 *
	 * @brief Static Resource object
	 */
	Resource resource;

	/**
	 * @ingroup _BAF
	 *
	 * @brief Pointer to the list of feature objects available for the framework
	 */
	JModule_List* feature_list;

	/**
	 * @ingroup _BAF
	 *
	 * @brief Pointer to the Audio_Graph object, that provides the identifiers for framework modules, connections,
	 * buffers and execution threads
	 */
	Audio_Graph* audio_graph;
}  _BAF;


#ifdef __cplusplus
}
#endif


#endif /*!defined(BC__BAF__INCLUDED_)*/
