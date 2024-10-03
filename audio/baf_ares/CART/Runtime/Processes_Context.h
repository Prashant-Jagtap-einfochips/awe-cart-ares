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
 * @file Processes_Context.h
 * 
 * @ingroup Processes_Context
 *
 * @brief Implementation of the Class Processes_Context       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Processes_Context__INCLUDED_)
#define BC_Processes_Context__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include "Process_Context.h"


/**
 * @ingroup Processes_Context
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef struct Processes_Context
{
	uint32_t num_items;
	Process_Context item[];
}  Processes_Context;



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Processes_Context__INCLUDED_)*/
