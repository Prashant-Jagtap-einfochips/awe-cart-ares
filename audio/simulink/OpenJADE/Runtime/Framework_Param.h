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
 * @file Framework_Param.h
 * 
 * @ingroup Framework_Param
 *
 * @brief Implementation of the Enumeration Framework_Param       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_Framework_Param__INCLUDED_)
#define BC_Framework_Param__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @ingroup Framework_Param
 *
 * @brief @author cb1037694
 * 
 * @version 1.0
 */
typedef enum
{
	FW_NUM_THREAD = 32,
	FW_NUM_LOG = 3,
	FW_PORT_FIFO = 2 // zero if fifo is not used else fifo size
} Framework_Param;


#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_Framework_Param__INCLUDED_)*/
