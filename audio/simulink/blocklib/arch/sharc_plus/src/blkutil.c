/*************************************************************************
*
*  BOSE CORPORATION
*  COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
*  This program may not be reproduced, in whole or in part in any
*  form or any means whatsoever without the written permission of:
*
*      BOSE CORPORATION
*      The Mountain
*      Framingham, MA 01701-9168
*
*  $Header$
*
*  Description: Generic C code block processing utilities
*
*************************************************************************/
#include "blkutil.h"

/**********************************************************************
 * BLKCOPY_C : block copy (could use memcpy() instead)
 *             does NOT handle overalpping arrays well
 **********************************************************************/
void blkcopy(void * QUALIF SRC, void * QUALIF DST, int LENGTH)
{
    float *px,*py;
    px = SRC;
    py = DST;
    while(LENGTH-- > 0) *(py++) = *(px++);
}


/**********************************************************************
 * BLKADD_C : add two blocks
 **********************************************************************/
void blkadd(const float * QUALIF SRCA, const float * QUALIF SRCB, float * QUALIF DST, int LENGTH)
{
    while(LENGTH-- > 0) *(DST++) = *(SRCA++) + *(SRCB++);
}


/*************************************************************************
 * blkprod: computes the product of two buffers into a third.
 * This is new stuff added by Bruce Levens, initially for the no clipper. 
 ************************************************************************/
void  blkprod(const float * QUALIF SRCA, const float * QUALIF SRCB, float * QUALIF DST, int LENGTH)
{
    while(LENGTH-- > 0) *(DST++) = *(SRCA++) * *(SRCB++);
}                                                              
