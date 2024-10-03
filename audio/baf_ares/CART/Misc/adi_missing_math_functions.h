/*******************************************************************************
*  BOSE CORPORATION
*  COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
*  This program may not be reproduced, in whole or in part in any
*  form or any means whatsoever without the written permission of:
*     BOSE CORPORATION
*     The Mountain
*     Framingham, MA 01701-9168
*******************************************************************************/

/**
* @file adi_missing_math_functions.h
* @brief Collection of math functions in C99 that Simulink will call, 
*     but not in ADI's math library
* @ingroup math
*/

#ifndef _BDSP_INTRINSICS_H_
#define _BDSP_INTRINSICS_H_

#include <string.h> /* memset */
#include <stdint.h> /* Standard integer types */
#include <limits.h> /* Needed for getting numeric limits */
#include <math.h>   /* Needed for fmin/fmax */

/******************************************************************************
 * Miscellaneous Constants
 ******************************************************************************/

/**
 * @brief Number of bits in dspWord_t.
 */
#define DSP_WORD_SIZE        (sizeof(float) * CHAR_BIT)


/******************************************************************************
 * Intrinsic functions prototypes
 ******************************************************************************/
static inline float roundf(float x);

static inline float roundf(float x)
{
   return (copysignf(floorf(0.5 + fabsf(x)), x));
}

static inline float truncf(float x);

static inline float truncf(float x)
{
   return (copysignf(floorf( fabsf(x) ), x));
}
#endif /* end of include guard: _BDSP_INTRINSICS_H_ */

