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
 * @file JExec.h
 * 
 * @brief Implementation of the Class JExec       
 *
 * @author: Constantin Bajenaru                     
 ****************************************************/

#include "JExec.h"

/**
 * Initialize an execution element
 */
uint32_t JExec_init(JExec* this, JExec_Func func, void* baf, uint32_t level, void* context)
{
	this->func = func;
	this->context = context;
	this->level = level;
	this->baf = baf;
	return 0;
} 

