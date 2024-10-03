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
* @file ti_missing_math_functions.h
* @brief Collection of math functions in C99 that Simulink will call, 
*     but not in TI's math library
* @ingroup math
*/

#ifndef _TI_MISSING_MATH_FUNCTIONS_H_
#define _TI_MISSING_MATH_FUNCTIONS_H_

#ifdef __TI_COMPILER_VERSION__
 #if __TI_COMPILER_VERSION__ < 8000000
  #define fmaxf(a,b) ((a)>(b)?(a):(b))
  #define fminf(a,b) ((a)>(b)?(a):(b))
 #endif
#endif

#endif /* end of include guard: _TI_MISSING_MATH_FUNCTIONS_H_ */
