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
 * @file Connection_Item.h
 * 
 * @ingroup Connection_Item
 *
 * @brief Implementation of the Class Connection_Item       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Connection_Item__INCLUDED_)
#define BC_Connection_Item__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @ingroup Connection_Item
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef struct Connection_Item
{
	Port_Id src;
	Port_Id dst;
}  Connection_Item;



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Connection_Item__INCLUDED_)*/
