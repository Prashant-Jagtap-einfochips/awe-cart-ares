/*******************************************************************************
*
*				Audio Framework
*				---------------
*
********************************************************************************
*	  MathHelper.h
********************************************************************************
*
*	  Description:	AudioWeaver Framework math helper functions
*
*     Copyright:    (c) 2007-2021 DSP Concepts, Inc. All rights reserved.
*                   3235 Kifer Road
*                   Santa Clara, CA 95054
*
*******************************************************************************/

#ifndef _math_helper_h
#define _math_helper_h

#if !defined(USE_AFLOAT)
#include <math.h>
#endif

#include "FractMath.h"

#ifdef WIN32
#pragma warning( disable : 4211)
#endif


#ifndef PIF
#define PIF 3.141592653589793f
#endif

#define TWO_PIF 6.283185307179586f
#define PIDIV4F .785398163397448f

#ifdef USE_SINGLE_PREC
// These are double precision values
#ifndef PI
#define PI PIF
#endif

#define TWO_PI TWO_PIF
#define PIDIV4 PIDIV4F

//math functions
typedef FLOAT32 PREC_TYPE_D;

#define SIN_D(x)      sinf(x)
#define SINH_D(x)     sinhf(x)
#define COS_D(x)      cosf(x)
#define TAN_D(x)      tanf(x)
#define EXP_D(x)      expf(x)
#define SQRT_D(x)     sqrtf(x)
#define ATAN2_D(x,y)  atan2f(x,y)
#define POW_D(x,y)    powf(x,y)
#define UNDB20_D(x)   (expf((x) * 0.115129254649702f))
#define CONSTANT_D(x) (x##f)

#else
#ifndef PI
#define PI (3.141592653589793)
#endif

#define TWO_PI (6.283185307179586)
#define PIDIV4 (0.785398163397448)

typedef FLOAT64 PREC_TYPE_D;
//math functions we typically use double precision for because they are in set functions
//most modules will use single precision exclusively, but certain filter designs will default
//to using doubles since it's assumed these designs only take place at startup.
//USE_SINGLE_PREC to force single precision throughout the system. This can shrink
//code size due to double precision constants etc.

#define SIN_D(x)    sin(x)
#define SINH_D(x)   sinh(x)
#define COS_D(x)    cos(x)
#define TAN_D(x)    tan(x)
#define EXP_D(x)    exp(x)
#define SQRT_D(x)   sqrt(x)
#define ATAN2_D(x,y)  atan2(x,y)
#define POW_D(x,y)    pow(x,y)
#define UNDB20_D(x)   (exp((x) * 0.115129254649702))
#define CONSTANT_D(x) (x)

#endif

#define AWE_FLOAT_ZERO 0.0f
#define AWE_FLOAT_ONE_HUNDREDTH 0.01f
#define AWE_FLOAT_ONE_THOUSANDTH 0.001f
#define AWE_FLOAT_QUARTER 0.25f
#define AWE_FLOAT_HALF 0.5f
#define AWE_FLOAT_ONE 1.0f
#define AWE_FLOAT_MINUS_ONE -1.0f
#define AWE_FLOAT_TWO 2.0f
#define AWE_FLOAT_FOUR 4.0f
#define AWE_FLOAT_TEN 10.0f
#define AWE_FLOAT_ONE_HUNDRED 100.0f
#define AWE_FLOAT_ONE_THOUSAND 1000.0f
#define AWE_FLOAT_E 2.7182818284f
#define AWE_FLOAT_32768 32768.0f
#define AWE_FLOAT_MINUS_32768 -32768.0f
#define AWE_FLOAT_INV_SQRT_TWO 0.707106781186548f
#define AWE_MIN_FLOAT 1.175494351e-38f
#define AWE_MAX_FLOAT 3.402823466e+38f




#if (defined(WIN32) || defined(__linux__) || defined(DSPC_CORTEX_M) || defined(OMAP_DSP) || defined(IMX25) || defined(TEAKLITE3) || defined(CEVA_X2) )

#ifndef fmaxf
#define fmaxf(x, y)	(((x) > (y)) ? (x): (y))
#endif

#ifndef fminf
#define fminf(x, y)	(((x) < (y)) ? (x) : (y))
#endif

#endif // WIN32 || DSPC_CORTEX_M || OMAP_DSP || IMX25

#if defined(_TMS320C6700)

#ifndef fminf
static VEC_INLINE FLOAT32 fminf(FLOAT32 x, FLOAT32 y)
{
  return (x < y) ? x : y;
}
#endif

#ifndef fmaxf
static VEC_INLINE FLOAT32 fmaxf(FLOAT32 x, FLOAT32 y)
{
  return (x > y) ? x : y;
}
#endif

#ifndef min
#define min(x, y) (((x) < (y)) ? (x) : (y))
#endif

#ifndef max
#define max(x, y) (((x) > (y)) ? (x) : (y))
#endif

#define divf_approx(num, den) ((num) * _rcpsp(den))

#else

#define divf_approx(num, den) ((num) / (den))

#endif // !defined(_TMS320C6700)

// Visual Studio version must be less than VS2015
#if defined(TEAKLITE3)

#ifndef fabsf
static VEC_INLINE FLOAT32 fabsf(FLOAT32 x)
{
  return (x < 0.0f) ? -x : x;
}
#endif

#if !defined(expf) && !defined(_M_AMD64)
static VEC_INLINE FLOAT32 expf(FLOAT32 v)
{
	return (FLOAT32)exp(v);
}
#endif

#if !defined(sqrtf) && !defined(_M_AMD64)
static VEC_INLINE FLOAT32 sqrtf(FLOAT32 v)
{
  return (FLOAT32)sqrt(v);
}
#endif

#if !defined(sinf) && !defined(_M_AMD64)
static VEC_INLINE FLOAT32 sinf(FLOAT32 v)
{
  return (FLOAT32)sin(v);
}
#endif

#if !defined(cosf) && !defined(_M_AMD64)
static VEC_INLINE FLOAT32 cosf(FLOAT32 v)
{
  return (FLOAT32)cos(v);
}
#endif

#if !defined(tanf) && !defined(_M_AMD64)
static VEC_INLINE FLOAT32 tanf(FLOAT32 v)
{
  return (FLOAT32)tan(v);
}
#endif

#if !defined(sinhf) && !defined(_M_AMD64)
static VEC_INLINE FLOAT32 sinhf(FLOAT32 v)
{
  return (FLOAT32)sinh(v);
}
#endif

#if !defined(floorf) && !defined(_M_AMD64)
static VEC_INLINE FLOAT32 floorf(FLOAT32 X)
{
  return (FLOAT32)floor(X);
}
#endif

#if !defined(logf) && !defined(_M_AMD64)
static VEC_INLINE FLOAT32 logf(FLOAT32 X)
{
  return (FLOAT32)log(X);
}
#endif

#if !defined(coshf) && !defined(_M_AMD64)
static VEC_INLINE FLOAT32 coshf(FLOAT32 X)
{
  return (FLOAT32)cosh(X);
}
#endif

#if !defined(ceilf) && !defined(_M_AMD64)
static VEC_INLINE FLOAT32 ceilf(FLOAT32 X)
{
  return (FLOAT32)ceil(X);
}
#endif

#if !defined(atanf) && !defined(_M_AMD64)
static VEC_INLINE FLOAT32 atanf(FLOAT32 X)
{
  return (FLOAT32)atan(X);
}
#endif

#if !defined(atan2f) && !defined(_M_AMD64)
static VEC_INLINE FLOAT32 atan2f(FLOAT32 Y, FLOAT32 X)
{
  return (FLOAT32)atan2(Y, X);
}
#endif

#if !defined(asinf) && !defined(_M_AMD64)
static VEC_INLINE FLOAT32 asinf(FLOAT32 X)
{
  return (FLOAT32)asin(X);
}
#endif

#if !defined(acosf) && !defined(_M_AMD64)
static VEC_INLINE FLOAT32 acosf(FLOAT32 X)
{
  return (FLOAT32)acos(X);
}
#endif

#if !defined(fmodf) && !defined(_M_AMD64)
static VEC_INLINE FLOAT32 fmodf(FLOAT32 X, FLOAT32 Y)
{
  return (FLOAT32)fmod(X, Y);
}
#endif

#if !defined(tanhf) && !defined(_M_AMD64)
static VEC_INLINE FLOAT32 tanhf(FLOAT32 X)
{
  return (FLOAT32)tanh(X);
}
#endif

#if !defined(powf) && !defined(_M_AMD64)
static VEC_INLINE FLOAT32 powf(FLOAT32 X, FLOAT32 Y)
{
  return (FLOAT32)pow(X, Y);
}
#endif

#ifndef frexpf
static VEC_INLINE FLOAT32 frexpf(FLOAT32 X, int *E)
{
  return (FLOAT32)frexp(X, E);
}
#endif

#ifndef ldexpf
static VEC_INLINE FLOAT32 ldexpf(FLOAT32 X, int E)
{
  return (FLOAT32)ldexp(X, E);
}
#endif

#ifndef modff
static VEC_INLINE FLOAT32 modff(FLOAT32 X, FLOAT32 *N)
{
  FLOAT32 fval;
  double dummy;

  fval = (FLOAT32)modf(X, &dummy);

  // For some reason, this call always returns nval=0.  So, compute
  // it again here.

  *N = X-fval;

  return (FLOAT32)fval;
}
#endif

#endif	//WIN32

#if defined(USE_AFLOAT)

#define frexpf(X, Y) fr32_frexpf(X, Y)
#define fabsf(X) fr32_fabsf(X)
#define ldexpf(X, E) fr32_ldexpf(X, E)
#define fminf(X, Y) fr32_fminf(X, Y)
#define fmaxf(X, Y) fr32_fmaxf(X, Y)
#define expf(X) fr32_expf(X)
#define logf(X) fr32_logf(X)
#define exp10f(X) fr32_exp10f(X)
#define log10f(X) fr32_log10f(X)
#define exp2f(X) fr32_exp2f(X)
#define log2f(X) fr32_log2f(X)
#define powf(X, Y) fr32_powf(X, Y)
#define sinf(X) fr32_sinf(X)
#define cosf(X) fr32_cosf(X)
#define tanf(X) fr32_tanf(X)
#define asinf(X) fr32_asinf(X)
#define acosf(X) fr32_acosf(X)
#define atanf(X) fr32_atanf(X)
#define atan2f(X, Y) fr32_atan2f(X, Y)
#define sinhf(X) fr32_sinhf(X)
#define coshf(X) fr32_coshf(X)
#define tanhf(X) fr32_tanhf(X)
#define sqrtf(X) fr32_sqrtf(X)
#define truncf(X) fr32_truncf(X)
#define floorf(X) fr32_floorf(X)
#define ceilf(X) fr32_ceilf(X)
#define modff(X, Y) fr32_modff(X, Y)
#define fmodf(X, Y) fr32_fmodf(X, Y)
#define fdimf(X, Y) fr32_fdimf(X, Y)

//#define db10f(X) fr32_db10f(X)
//#define undb10f(X) fr32_undb10f(X)
//#define db20f(X) fr32_db20f(X)
//#define undb20f(X) fr32_undb20f(X)

#endif

#if defined(WIN32) && !defined(DLL_COEFF_EXPORT) && !defined(BARE_METAL)
#define DLL_COEFF_IMPORT __declspec(dllimport)
#else
#define DLL_COEFF_IMPORT extern
#endif

/* ----------------------------------------------------------------------
** Fast approximation to the log2() function.  It uses a two step
** process.  First, it decomposes the floating-point number into
** a fractional component F and an exponent E.  The fraction component
** is used in a polynomial approximation and then the exponent added
** to the result.  A 3rd order polynomial is used and the result
** when computing db20() is accurate to 7.984884e-003 dB.
** ------------------------------------------------------------------- */

DLL_COEFF_IMPORT FLOAT32 log2f_approx_coeff[4];

static VEC_INLINE FLOAT32 log2f_approx(FLOAT32 X)
{
	FLOAT32 *C = &log2f_approx_coeff[0];
	FLOAT32 Y;
	FLOAT32 F;
	INT32 E;

	// This is the approximation to log2()
	F = frexpf(X, &E);

	//  Y = C[0]*F*F*F + C[1]*F*F + C[2]*F + C[3] + E;
	Y = *C;
	C++;
	Y *= F;
	Y += (*C);
	C++;
	Y *= F;
	Y += (*C);
	C++;
	Y *= F;
	Y += (*C);

#if defined(USE_AFLOAT)
	Y += int_to_afloat(E);
#else
	Y += E;
#endif

	return Y;
}

/* ----------------------------------------------------------------------
** Computes the log2 of a fractional value and returns the result in
** Q6.26 format.  The function is accurate to 1.326392691030e-3 (when
** viewed as a floating-point number.
** ------------------------------------------------------------------- */

DLL_COEFF_IMPORT fract32 log2_coeff_fract32[4];
extern INT32 signbits(fract32 x);

static VEC_INLINE fract32 log2_fract32(fract32 X)
{

  fract32 *C = log2_coeff_fract32;
  fract32 y;
  INT32 E;
  fract32 M;

  if (X == 0)
  {
	  // Special case handling for 0.  Return the smallest possible number
	  return((fract32)0x80000000);
  }
  else if (X < 0)
  {
	  X = -X;
  }

  // Turn into exponent and mantissa.
  E = signbits(X);
  M = shl_fr1x32(X, (short) E);

  y = *C;
  C++;
  y = mult_fr1x32x32NS(y, M);
  y += (*C);
  C++;
  y = mult_fr1x32x32NS(y, M);
  y += (*C);
  C++;
  y = mult_fr1x32x32NS(y, M);
  y += (*C);

  return(y - (E << 26));
}

/* ----------------------------------------------------------------------
** Computes the db20 of a fractional Q1.31 value and returns the result
** in Q6.26 format.  The function is accurate to 8e-3 (when
** viewed as a floating-point number.
** ------------------------------------------------------------------- */

static VEC_INLINE fract32 db20_fract32(fract32 X)
{
	fract32 y;
	// input is Q16.16
	// the relayed function expects a value in format Q1.31 and produces a result in Q6.26
	// add offset to compensate the difference between the actual input in Q16.16
	y = log2_fract32(X) + ((31 - 16) << 26);
	// Shift 3 spaces to allow values of up to 190dB plus 3 more to prepare the function for the next multiplication
	y >>= 3;
	// Shift 3 more bits to the right to pre-multiply by 8 for the following operation
	// Handling for 0. Return the gretest negative number
	if (X == 0)
	{
		y = (fract32)0x80000000;
	}
	else
	{
		// Multiply by db20(2) / 8
		// round((db20(2)/8)) * 2^31
		y = mult_fr1x32x32NS(y, 1616142483);
	}

	return y;
}

/* ----------------------------------------------------------------------
** Fast approximation to the pow2() function.  It uses a two step
** process.  First, it decomposes the floating-point number into
** an integer I and fractional component F.  The fraction component
** is used in a polynomial approximation and then the exponent raised to
** the power of 2 multiplies the result.  The function uses the ldexpf
** function for speed.  A 6th order polynomial is used and the result
** when computing undb20() is accurate to 3.228661e-002 dB
** ------------------------------------------------------------------- */

DLL_COEFF_IMPORT FLOAT32 pow2f_approx_coeff[6];

static VEC_INLINE FLOAT32 pow2f_approx(FLOAT32 X)
{
	FLOAT32 *C = &pow2f_approx_coeff[0];
	FLOAT32 I;
	FLOAT32 F;
	FLOAT32 Y;

	// This is the integer component
	I = floorf(X);

	// This is the fractional component
	F = X - I;

	Y = *C;
	C++;
	Y *= F;
	Y += (*C);
	C++;
	Y *= F;
	Y += (*C);
	C++;
	Y *= F;
	Y += (*C);
	C++;
	Y *= F;
	Y += (*C);
	C++;
	Y *= F;
	Y += (*C);

	  // This is the integer component
#if defined(USE_AFLOAT)
	Y = ldexpf(Y, afloat_to_int(I));
#else
	Y = ldexpf(Y, (INT32)I);
#endif

	return Y;
}



/* ----------------------------------------------------------------------
** Computes the pow2 of a fractional value. The input is in Q6.26 format
** and the output in Q16.16 format.  You have to ensure that the inputs 
** are in the proper range.  That is the integer part of the input must
** be in the range [-15 15] or there will be numerical problems.  
** (This isn't checked here because it is often easier to do in the 
** higher level calling function.)
** ------------------------------------------------------------------- */

DLL_COEFF_IMPORT fract32 pow2_coeff_fract32[7];

static VEC_INLINE fract32 pow2_fract32(fract32 x)
{
  fract32 *C = pow2_coeff_fract32;
  fract32 y;
  INT32 I;
  fract32 F;
  fract32 val;
  INT32 E;

  // Compute the integer and fractional portions of the Q6.26 input
  I = x >> 26;
  F = (0x03FFFFFF & x) << 5;

  // Evaluate the 6th order polynomial

  y = *C;
  C++;
  y = mult_fr1x32x32NS(y, F);
  y += (*C);
  C++;
  y = mult_fr1x32x32NS(y, F);
  y += (*C);
  C++;
  y = mult_fr1x32x32NS(y, F);
  y += (*C);
  C++;
  y = mult_fr1x32x32NS(y, F);
  y += (*C);
  C++;
  y = mult_fr1x32x32NS(y, F);
  y += (*C);
  C++;
  y = mult_fr1x32x32NS(y, F);
  y += (*C);

  // Now keep track of the final number of bits to shift.
  // We have to shift:
  //    15 bits right because we want to convert from Q1.31 to Q16.16
  //    3 bits left because the coefficients were scaled down by 1/8
  //    Then the integer number of bits to the right

  E = 15 - 3 - I;
  if (E < 0)
  {
    val = y << (-E);
  }
  else
  {
    val = y >> E;
  }

  return val;
}

#ifndef db10f
static VEC_INLINE FLOAT32 db10f(FLOAT32 X)
{
	X = fabsf(X);

#if defined(USE_AFLOAT)
	if (X < AWE_MIN_FLOAT)
	{
		return (fr32)0xCCF48CAB; //  -94.568686021917173
	}
	else
	{
		return (logf(X) * (fr32)0x442BE59E);
	}
#else
	if (X < AWE_MIN_FLOAT)
	{
		return -3.7929779e+02f;
	}
	else
	{
		return logf(X) * 4.34294481903252f;
	}
#endif
}

#endif

#ifndef db10f_approx
static VEC_INLINE FLOAT32 db10f_approx(FLOAT32 X)
{
	X = fabsf(X);

#if defined(USE_AFLOAT)
	if (X < AWE_MIN_FLOAT)
	{
		return (fr32)0xCCF48CAB; //  -94.568686021917173
	}
	return (log2f_approx(X) * (fr32)0x4302A305);
#else
	if (X < AWE_MIN_FLOAT)
	{
		return -3.7929779e+02f;
	}
	return log2f_approx(X) * 3.010299956639812f;
#endif
}
#endif

#ifndef undb10f

static VEC_INLINE FLOAT32 undb10f(FLOAT32 X)
{	
#if defined(USE_AFLOAT)
	return expf(X * (fr32)0x3BAF238C);
#else
    return expf(X * 0.230258509299405f);
#endif
}
#endif


#ifndef undb10f_approx
static VEC_INLINE FLOAT32 undb10f_approx(FLOAT32 X)
{
#if defined(USE_AFLOAT)
	return pow2f_approx(X * (fr32)0x3CA854B4);
#else
	return pow2f_approx(X * 0.332192809488736f);
#endif
}
#endif


#ifndef db20f
static VEC_INLINE FLOAT32 db20f(FLOAT32 X)
{
	X = fabsf(X);

#if defined(USE_AFLOAT)

	if (X < AWE_MIN_FLOAT)
	{
		return (fr32)0xCEF48CAB;  //  -1.891373720438344e+02
	} 
	else
	{
		return (logf(X) * (fr32)0x462BE59E);
	}
#else
	if (X < AWE_MIN_FLOAT)
	{
		return -7.5859558e+02f;
	} 
	else
	{
		return logf(X) * 8.68588963806504f;
	}
#endif
}
#endif

#ifndef db20f_approx
static VEC_INLINE FLOAT32 db20f_approx(FLOAT32 X)
{
	X = fabsf(X);
     
#if defined(USE_AFLOAT)
    if (X < AWE_MIN_FLOAT)
    {
		return (fr32)0xCEF48CAB;  //  -1.891373720438344e+02
    }
    return (log2f_approx(X) * (fr32)0x4502A305);
#else
    if (X < AWE_MIN_FLOAT)
    {
		return -7.5859558e+02f;
    }
    return log2f_approx(X) * 6.020599913279624f;
#endif
}
#endif

#if defined(TEAKLITE3) && !defined(log10f)
static VEC_INLINE FLOAT32 log10f(FLOAT32 X)
{
	return (FLOAT32)log10(X);
}
#endif

#ifndef undb20f
static VEC_INLINE FLOAT32 undb20f(FLOAT32 X)
{
#if defined(USE_AFLOAT)
	return expf(X * (fr32)0x39AF238C);
#else
	return expf(X * 0.115129254649702f);
#endif
}
#endif


#ifndef undb20f_approx
static VEC_INLINE FLOAT32 undb20f_approx(FLOAT32 X)
{
#if defined(USE_AFLOAT)
	// Y = pow2_approx((X/20)*log2(10), ORDER);
	return pow2f_approx(X * (fr32)0x3AA854B4);
#else
	// Y = pow2_approx((X/20)*log2(10), ORDER);
	return pow2f_approx(X * 0.166096404744368f);
#endif
}
#endif

//more complex smoother update function that accounts for some steady state issues with the smoother and
//precision - biquad coefficients don't tend to make it all the way which can affect some low frequency responses up
//to 3 dB
#define BIQUAD_NUM_COEFFICIENTS 5
static VEC_INLINE void updateCoeffArray(FLOAT32* c, FLOAT32 *desired, FLOAT32 smoothingCoeff)
{
    UINT32 cnt;
    for (cnt = 0; cnt < BIQUAD_NUM_COEFFICIENTS; cnt++)
    {
        if (*c != *desired)
        {
            FLOAT32 startC, endC;
            startC = *c;
            //this is just a rearranging of of (*c) * (1-smoothingCoeff) + desired * smoothingCoeff that saves us not having to calculate 1-smooth
            endC = startC + smoothingCoeff * (*desired - startC);
            *c = endC;
            if (startC == endC)//this one is legit because it empirically happens almost all the time. we don't get the same savings using ints here.
            {
                //we've hit a steady state and must correct
                *c = *desired;
            }
        }
        ++c;
        ++desired;
    }
}

  /* ----------------------------------------------------------------------
  ** Audio Weaver specific exponential function
  ** ------------------------------------------------------------------- */

#ifndef expf_approx

#define EXPF_APPROX_P0 0.2499999995f
#define EXPF_APPROX_P1 4.160288626e-3f
#define EXPF_APPROX_Q0 0.5000000000f
#define EXPF_APPROX_Q1 4.9987178773e-2f

#define EXPF_APPROX_C1 0.693359375f
#define EXPF_APPROX_C2 -2.1219444005469058277e-4f

// Determined manually when the floating-point math overflows;
#define EXPF_APPROX_MAX_LIN 1.2e+38f
#define EXPF_APPROX_MAX_LOG 87.6805573f

// exp(1.0E-7)=1
#define EXPF_APPROX_MIN_LOG 1.0e-7f

// EXPF_APPROX_K1 = 1/ln(2)
#define EXPF_APPROX_K1 1.4426950409f

static VEC_INLINE FLOAT32 expf_approx(FLOAT32 X)
{
  FLOAT32 y;
  FLOAT32 z;
  FLOAT32 xn;
  FLOAT32 g;
  FLOAT32 temp;
  FLOAT32 r;
  FLOAT32 q;
  INT32 sign;
  INT32 n;

  // ----------------------------------------------------------------------
  // Handle only positive numbers in the main function
  // ----------------------------------------------------------------------

  if (X >= 0)
    {
      y = X;
      sign = 0;
    }
  else
    {
      y = -X;
      sign = 1;
    }

  // ----------------------------------------------------------------------
  // Handle extremities
  // ----------------------------------------------------------------------

  if (y < EXPF_APPROX_MIN_LOG)
    {
      return(1.0);
    }

  if (y >= EXPF_APPROX_MAX_LOG)
    {
      if (sign)
	{
	  return(0);
	}
      else
	{
	  return(EXPF_APPROX_MAX_LIN);
	}
    }

  // ----------------------------------------------------------------------
  // This is the general case
  // ----------------------------------------------------------------------

  z = y * EXPF_APPROX_K1;
  n = (INT32) z;

  if (n < 0)
    {
      n--;
    }

  if ((z-n) >= 0.5f)
    {
      n++;
    }

  xn = (FLOAT32) n;
  g = (y - xn * EXPF_APPROX_C1) - xn * EXPF_APPROX_C2;

  z = g * g;

  temp = (EXPF_APPROX_P1 * z) + EXPF_APPROX_P0;
  r = temp * g;

  q = ((EXPF_APPROX_Q1 * z) + EXPF_APPROX_Q0);
  r = 0.5f + (r / (q - r));

  n++;

  z = ldexpf(r, n);

  // ----------------------------------------------------------------------
  // Incorporate the sign
  // ----------------------------------------------------------------------

  if (sign)
    {
      return(1.0f / z);
    }
  else
    {
      return(z);
    }
}

#endif




  /* ----------------------------------------------------------------------
  ** Converts floating-point values in the range [-1 +1) to fractional
  ** values
  ** ------------------------------------------------------------------- */

#ifndef float_to_fract32
static VEC_INLINE fract32 float_to_fract32(FLOAT32 x)
{
    // Clip to the allowable range
	FLOAT32 temp;

#if defined(USE_AFLOAT)

    if (x < FR32_MINUS_ONE)
    {
        return (fract32)0x80000000;
    }

    if (x >= FR32_ONE)
    {
        return 0x7FFFFFFF;
    }

	temp = x * float_to_afloat(2147483648.0f);

	if (temp > FR32_ZERO)
	{
		temp += float_to_afloat(0.5f);
	}
	else
	{
		temp += float_to_afloat(-0.5f);
	}

    // Multiply by 2^31
    return (fract32)(afloat_to_int(temp));

#else

    if (x < -1.0f)
    {
        return (fract32)0x80000000;
    }

    if (x >= 1.0f)
    {
        return 0x7FFFFFFF;
    }

	temp = x * 2147483648.0f;
	temp += temp > 0.0f? 0.5f : -0.5f;

    // Multiply by 2^31
    return (fract32)(temp);

#endif
}
#endif

  /* ----------------------------------------------------------------------
  ** Converts fractional values to their floating-point equivalents in the
  ** range [-1 +1)
  ** ------------------------------------------------------------------- */

#ifndef fract32_to_float
static VEC_INLINE FLOAT32 fract32_to_float(fract32 x)
{
#if defined(USE_AFLOAT)
    // Multiply by 2^(-31)
    return (int_to_afloat(x) * float_to_afloat(4.6566128731e-010f));
#else
    // Multiply by 2^(-31)
    return ((FLOAT32)x * 4.6566128731e-010f);
#endif
}
#endif

//// SHARC intrinsic functions
//
//#if defined(WIN32) || defined(__ADSPBLACKFIN__) || (__CC_ARM || defined(__GNUC__))
//fract32 float_to_fract32(FLOAT32 x);
//FLOAT32 fract32_to_float(fract32 x);
//#endif /* WIN32 or Blackfin */
//





#endif  // _math_helper_h
