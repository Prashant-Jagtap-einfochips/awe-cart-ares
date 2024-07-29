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
 * @file Process_Context.h
 * 
 * @brief Implementation of the Class Process_Context       
 *
 * @author: cb1037694                     
 ****************************************************/

#include "Process_Context.h"

Port_Context* Process_Context_get_port_in(Process_Context* this, uint32_t port_index)
{
	return  &(this->ports_in->item[port_index]);
}

Port_Context* Process_Context_get_port_out(Process_Context* this, uint32_t port_index)
{
	return  &(this->ports_out->item[port_index]);;
} 

