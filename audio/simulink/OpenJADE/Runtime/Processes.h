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
 * @file Processes.h
 * 
 * @ingroup Processes
 *
 * @brief Implementation of the Class Processes       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Processes__INCLUDED_)
#define BC_Processes__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include "JProcess.h"


/**
 * @ingroup Processes
 *
 * @brief This structure is a container of the JModule processes. A feature may contain any number of
 * processes, which can run at different thread priorities.
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */
typedef struct Processes
{

	/**
	 * @ingroup Processes
	 *
	 * @brief Number of Process items in the Processes container
	 */
	int32_t num_items;

	/**
	 * @ingroup Processes
	 *
	 * @brief Variable length array (VLA) of Process items.
	 */
	Process process[
#ifdef OJ_NUM_PROCESS	
		OJ_NUM_PROCESS
#endif
	];
}  Processes;



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Processes__INCLUDED_)*/
