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
 * @file Semaphore.h
 * 
 * @brief Implementation of the Class Semaphore       
 *
 * @author: cb1037694                     
 ****************************************************/

#include "JSemaphore.h"
// Local Semaphore mockup
static BAF_Semaphore Semaphore_pool[32];
uint32_t BAF_semaphore_index = 0;

BAF_Semaphore* BAF_Semaphore_create(uint32_t count)
{	
	BAF_Semaphore *sem;
	sem = &Semaphore_pool[BAF_semaphore_index++]; 
	sem->count = count;
	return sem;
}

void BAF_Semaphore_post(BAF_Semaphore* this)
{
	this->count++;
}

void BAF_Semaphore_pend(BAF_Semaphore* this)
{
	if(this->count)
	{
	   this->count--;
	}
	else
	{
	    while(1);
	}
} 

