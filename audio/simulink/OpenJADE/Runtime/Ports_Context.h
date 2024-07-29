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
 * @file Ports_Context.h
 * 
 * @ingroup Ports_Context
 *
 * @brief Implementation of the Class Ports_Context       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Ports_Context__INCLUDED_)
#define BC_Ports_Context__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include "Port_Context.h"


/**
 * @ingroup Ports_Context
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef struct Ports_Context
{
	uint32_t num_items;
	Port_Context item[];
}  Ports_Context;



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Ports_Context__INCLUDED_)*/
