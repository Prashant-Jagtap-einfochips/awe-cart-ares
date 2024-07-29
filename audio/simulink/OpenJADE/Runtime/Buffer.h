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
 * @file Buffer.h
 * 
 * @ingroup Buffer
 *
 * @brief Implementation of the Class Buffer       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Buffer__INCLUDED_)
#define BC_Buffer__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "JModule_Bitfield.h"


/**
 * @ingroup Buffer
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef struct Buffer
{
	Buffer_Id id;
	void* payload;
}  Buffer;



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Buffer__INCLUDED_)*/
