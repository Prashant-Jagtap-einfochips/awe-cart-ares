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
 * @file BAF_Func.h
 * 
 * @ingroup BAF_Func
 *
 * @brief Implementation of the Class BAF_Func       
 *
 * @author: cb1037694                     
 ****************************************************/

#if !defined(BC_BAF_Func__INCLUDED_)
#define BC_BAF_Func__INCLUDED_


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @ingroup BAF_Func
 *
 * @brief Type definition for the pointer to functions used inside the BAF component
 * 
 * @author cb1037694
 * 
 * @version 1.0
 */
typedef struct BAF_Func
{
	void* func;
} BAF_Func;

/**
 * @ingroup BAF_Func
 *
 * @brief Pointer to notification function API
 */
typedef void (*BAF_Func_Notify)(void* arg, BAF_Message* rsp); 



#ifdef __cplusplus
}
#endif


#endif /*!defined(BC_BAF_Func__INCLUDED_)*/
