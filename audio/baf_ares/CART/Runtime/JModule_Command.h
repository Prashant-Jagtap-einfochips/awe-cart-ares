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
 * @file JModule_Command.h
 * 
 * @ingroup JModule_Command
 *
 * @brief Implementation of the Class JModule_Command       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_JModule_Command__INCLUDED_)
#define BC_JModule_Command__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include "JModule_Func.h"


/**
 * @ingroup JModule_Command
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef struct JModule_Command
{
	uint32_t num_command;
	JModule_Func_Payload payload[];
} JModule_Command;

 



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_JModule_Command__INCLUDED_)*/
