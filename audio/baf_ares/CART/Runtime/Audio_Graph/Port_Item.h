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
 * @file Port_Item.h
 * 
 * @ingroup Port_Item
 *
 * @brief Implementation of the Class Port_Item       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Port_Item__INCLUDED_)
#define BC_Port_Item__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif

#include "JModule_Bitfield.h"


/**
 * @ingroup Port_Item
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef struct Port_Item
{
	Port_Id port_id;
	Buffer_Id buffer_id;
}  Port_Item;



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Port_Item__INCLUDED_)*/
