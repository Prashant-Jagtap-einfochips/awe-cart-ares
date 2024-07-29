/**********************************************************************
 * This is a standard set of block utility functions that
 * all processor platforms should support. There are a few
 * additional ones, that are only supported for the Sharc
 * The fixed point versions don't actually make any assumptions
 * about the Q. The C reference will use Q31 for everything.
 * $Id: blkutil.h 53742 2015-08-24 15:10:05Z as70207 $
 **********************************************************************/
#ifndef __BLKUTIL_H__
#define __BLKUTIL_H__

#define QUALIF

void blkcopy(void * QUALIF SRC, void * QUALIF DST, int LENGTH);
void blkadd(const float * QUALIF SRCA, const float * QUALIF SRCB, float * QUALIF DST, int LENGTH);
void blkprod(const float * QUALIF SRCA, const float * QUALIF SRCB, float * QUALIF DST, int LENGTH);
 
#endif /* __BLKUTIL_H__ */
