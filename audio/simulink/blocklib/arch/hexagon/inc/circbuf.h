/*****************************************************************************************
 *
 * BOSE CORPORATION
 * COPYRIGHT 2023 BOSE CORPORATION ALL RIGHTS RESERVED
 * This program may not be reproduced, in whole or in part in any
 * form or any means whatsoever without the written permission of:

 *     BOSE CORPORATION
 *     The Mountain
 *     Framingham, MA 01701-9168

*****************************************************************************************/

/**
 *  @file circbuf.h
 *  @brief Contains Circular buffer helper functions
 *
 *  @ingroup dsp
 *
 */
 
#ifndef __CIRCBUF_H__
#define __CIRCBUF_H__

/**
 *  circindex
 *
 *  @brief Return new index after advancing l-point circular buffer pointer by n elements
 *
 *  @param [in]   index     Last element inedex
 *  @param [in]   incr      Elements to advance
 *  @param [in]   num_items Circular buffer capacity
 *  @param [out] New element index
 *  All additional parameters described in init function.
 *
 */
inline int circindex(int index, int incr, int num_items){
	index += incr;
    if(index < 0) index += num_items;
	else if(index >= num_items) index -= num_items;
	return index;
}

#endif    /* __CIRCBUF_H__ */	
