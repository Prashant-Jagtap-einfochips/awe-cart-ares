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
 * @file Execution.h
 * 
 * @ingroup Execution
 *
 * @brief Implementation of the Class Execution       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Execution__INCLUDED_)
#define BC_Execution__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include "Process_Context.h"
#include "JProcess.h"


/**
 * @ingroup Execution
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef struct Execution
{
	Process* process;
	Process_Context* context;
}  Execution;



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Execution__INCLUDED_)*/
