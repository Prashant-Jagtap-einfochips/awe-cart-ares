/* ----------------------------------------------------------------------------
 *	 Preprocessor Definitions
 * ------------------------------------------------------------------------- */

#ifndef WIN_TARGET_PROCESSOR_H_
#define WIN_TARGET_PROCESSOR_H_

#define VEC_RESTRICT	__restrict
#define VEC_INLINE		__inline

#define DISABLE_INTERRUPTS()
#define ENABLE_INTERRUPTS()

#include "Windows.h"

#if defined(_MSC_VER) && defined(USEFRAMEWORKDLL) //TODO: Is it possible to remove all of this stuff below? Definitely confusing
#ifndef DLLSYMBOL
#	define DLLSYMBOL			__declspec(dllimport)
#endif
#ifndef CXX_NOWARN_DLLCLASSIF
#	define CXX_NOWARN_DLLCLASSIF		__pragma(warning(push))\
		__pragma(warning(disable: 4251))
#endif
#ifndef CXX_RESTORE_DLLCLASSIF
#	define CXX_RESTORE_DLLCLASSIF		__pragma(warning(pop))
#endif
#endif

/*
 * @def FW_RESTRICT
 * @brief Compiler-specific C99 restrict keyword.
 */

#define FW_RESTRICT

#define FIXNAME(x)      x

#include "StandardDefs.h"

/* ----------------------------------------------------------------------
** Cycle counting macros
** ------------------------------------------------------------------- */

//UINT32 awe_pltGetCycleCount(void);
//UINT32 awe_pltElapsedCycles(UINT32 start_time, UINT32 end_time);

static VEC_INLINE void awe_fwCycleInit(void)
{
}

static VEC_INLINE UINT32 awe_getCycleCount(void)
{
  LARGE_INTEGER ltime;
  LARGE_INTEGER freq;
  UINT32 cycles32;
  UINT64 cycles64;
  double ftime;
  QueryPerformanceFrequency(&freq);
  QueryPerformanceCounter(&ltime);
  ftime = ((double)ltime.QuadPart / (double)freq.QuadPart) * 1e7;
#ifdef PROFILE_DIVIDER
  cycles64 = (UINT64)(ftime / PROFILE_DIVIDER);
  cycles32 = cycles64 & 0xFFFFFFFF; // truncate to 32 bits
#else
  cycles64 = (UINT64)ftime;
  cycles32 = cycles64 & 0xFFFFFFFF; // truncate to 32 bits
#endif
  return (cycles32);
}

static VEC_INLINE UINT32 awe_getElapsedCycles(UINT32 start_time, UINT32 end_time)
{
#ifdef PROFILE_DIVIDER
    #if (PROFILE_DIVIDER == 2)
        return (end_time - start_time) & 0x7FFFFFFF;
    #elif (PROFILE_DIVIDER == 4)
        return (end_time - start_time) & 0x3FFFFFFF;
    #elif (PROFILE_DIVIDER == 8)
        return (end_time - start_time) & 0x1FFFFFFF;
    #elif (PROFILE_DIVIDER == 16)
        return (end_time - start_time) & 0x0FFFFFFF;
    #elif (PROFILE_DIVIDER == 32)
        return (end_time - start_time) & 0x07FFFFFF;
    #elif (PROFILE_DIVIDER == 64)
        return (end_time - start_time) & 0x03FFFFFF;
    #elif (PROFILE_DIVIDER == 128)
        return (end_time - start_time) & 0x01FFFFFF;
    #else
        #error "Illegal PROFILE_DIVIDER value (must be a power of 2)"
    #endif
#else
    return end_time - start_time;
#endif
}

#endif	  /* !defined  WIN_TARGET_PROCESSOR_H_ */
