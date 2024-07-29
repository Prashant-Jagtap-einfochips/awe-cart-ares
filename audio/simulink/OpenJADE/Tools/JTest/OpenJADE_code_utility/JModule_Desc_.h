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
 * @file JModule_Desc_$$JModule$$.h
 * 
 * @ingroup JModule_$$JModule$$_Param
 *
 * @brief Implementation of the Class JModule_$$JModule$$_Param       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_JModule_$$JModule$$_Param__INCLUDED_)
#define BC_JModule_$$JModule$$_Param__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


/**
 * @ingroup JModule_$$JModule$$_Param
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef struct JModule_$$JModule$$_Param
{
	uint32_t id;
} JModule_$$JModule$$_Param;


/**
 * @ingroup JModule_$$JModule$$_State
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef struct JModule_$$JModule$$_State
{
	uint32_t id;
} JModule_$$JModule$$_State;


/**
 * @ingroup JModule_Desc_$$JModule$$
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
enum JModule_Desc_$$JModule$$
{
	JMODULE_$$JMODULE$$_NUM_PROCESSES = $$NUM_PROCESSES$$,
	JMODULE_$$JMODULE$$_STATE_SIZE = sizeof(JModule_$$JModule$$_State),
	JMODULE_$$JMODULE$$_PARAM_SIZE = sizeof(JModule_$$JModule$$_Param)
};

 


 



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_JModule_$$JModule$$_Param__INCLUDED_)*/
