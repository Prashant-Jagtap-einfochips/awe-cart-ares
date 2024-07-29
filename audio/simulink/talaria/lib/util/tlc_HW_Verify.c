%% Included from HW_Verify_SFunc.tlc

#include "HW_Verify.h"
#include <stddef.h>
#include <limits.h>


/* return the minimum number of bits needed to 
 * represent the input value */
static unsigned int numBits(unsigned long value)
{
   /* note: returns 1 for zero as well as 1 */
   unsigned int num = 1;

   while (value >>= 1)
   {
      num++;
   }
   return num;
}

/* get CPU characteristics */
static void getCPUInfo(HWV_HWImpl * actual)
{
   int i1, i2, i3, i4, i5, i6, i7, i8, i9, r1, r2;

   /*-----------------------------------------------------------------------
    * shifts right on signed integers
    */
   i1 = -28;
   i2 = (i1 >> 2);

   actual->shiftRightBehavior = (i2 == (i1 / 4));

   /*-----------------------------------------------------------------------
    * negative operand integer division rounding
    */

   {
      /* num and denom must be declared volatile to force the divisions to
       * happen on the target, instead of being optimized away by the compiler.
       */
      volatile int num, denom;

      denom = 4;
      num = -7;
      i1 = num / denom; /* -7/4 */
      num = -6;
      i2 = num / denom; /* -6/4 */
      num = -5;
      i3 = num / denom; /* -5/4 */
      denom = -4;
      num = 7;
      i4 = num / denom; /* 7/-4 */
      num = 6;
      i5 = num / denom; /* 6/-4 */
      num = 5;
      i6 = num / denom; /* 5/-4 */
      num = -7;
      i7 = num / denom; /* -7/-4 */
      num = -6;
      i8 = num / denom; /* -6/-4 */
      num = -5;
      i9 = num / denom; /* -5/-4 */
   }

   /* round toward floor test */
   r1 = ((i1 == -2) && (i2 == -2) && (i3 == -2) && (i4 == -2) && (i5 == -2) && (i6 == -2)
         && (i7 == 1) && (i8 == 1) && (i9 == 1));

   /* round toward zero test */
   r2 = ((i1 == -1) && (i2 == -1) && (i3 == -1) && (i4 == -1) && (i5 == -1) && (i6 == -1)
         && (i7 == 1) && (i8 == 1) && (i9 == 1));

   /* set rounding behaviour*/
   if (r1 && !r2)
   {
      /* Rounds to floor */
      actual->intDivRoundTo = HWV_Floor;
   }
   else if (!r1 && r2)
   {
      /* rounds to zero */
      actual->intDivRoundTo = HWV_Zero;
   }
   else
   {
      /* undefined */
      actual->intDivRoundTo = HWV_Undefined;
   }

   {
      /*-----------------------------------------------------------------------
       * Byte order test
       */

      /* Avoid compiler warning about constant expression */
      volatile int isSameSize = (sizeof(long) == sizeof(char));
      if (isSameSize)
      {
         /* This Byte order test assumes that the size of char is 
          * less than the size of long. If this is not the case (for 
          * example TigerSharc integer data type sizes are all 32-bit), 
          * then this test will always return that the target is little 
          * endian. For such targets, the byte order becomes irrelevant 
          * and is safe to skip.
          */
         actual->byteOrder = HWV_UNABLE_TO_DETERMINE_ON_TARGET;
      }
      else
      {
         long s1 = 1;
         actual->byteOrder = (*((char *) &s1) == 1) ? HWV_LittleEndian : HWV_BigEndian;
      }
   }

   /*-----------------------------------------------------------------------
    * bits per char, short, int, long
    *
    * limits.h based check supports the case where a smaller data
    * type is implemented in a larger container, e.g. an 8-bit integer
    * implemented in a 16-bit container on some word addressable systems, 
    * e.g. DSP563xx 16-bit mode - sizeof(char) would be one 16-bit word whereas
    * UCHAR_MAX would be limited to 255.
    */
   actual->bitsPerChar = numBits(UCHAR_MAX);
   actual->bitsPerShort = numBits(USHRT_MAX);
   actual->bitsPerInt = numBits(UINT_MAX);
   actual->bitsPerLong = numBits(ULONG_MAX);

   /* expect "long long" compilation failure to have occurred 
    * during production code compilation, e.g. rtwtypes.h 
    * 
    * report bits per long long based on sizeof rather than 
    * ULLONG_MAX which may not exist pre-C99 */
   actual->bitsPerLongLong = sizeof(long long) * actual->bitsPerChar;
   actual->supportLongLong = 1;

   return;
}

static HWV_TestResult testPass(const HWV_TestResultMsgId msg)
{
   HWV_TestResult result;
   result.status = HWV_PASS;
   result.msg = msg;
   return result;
}

static HWV_TestResult testFail(const HWV_TestResultMsgId msg)
{
   HWV_TestResult result;
   result.status = HWV_FAIL;
   result.msg = msg;
   return result;
}

static HWV_TestResult testWarn(const HWV_TestResultMsgId msg)
{
   HWV_TestResult result;
   result.status = HWV_WARN;
   result.msg = msg;
   return result;
}

static HWV_TestResult runTest(const int actual, const int config)
{
   HWV_TestResult result;
   if (actual == config)
   {
      result = testPass(HWV_NO_MSG);
   }
   else
   {
      result = testFail(HWV_NO_MSG);
   }
   return result;
}

void HWV_HardwareVerifyImplementation(HWV_HWImpl* actual, const HWV_HWImpl* expected,
      HWV_HWImpl_TestResults* results)
{

   /* 
    * Get the actual CPU info 
    */
   getCPUInfo(actual);

   /* 
    * Run tests against expected CPU info 
    */
   if ((actual->intDivRoundTo == HWV_Undefined) && (expected->intDivRoundTo != HWV_Undefined))
   {
      /* actual behavior undefined => config must 
       * choose undefined too */
      results->intDivRoundTo = testFail(HWV_NO_MSG);
   }
   else
   {
      if (expected->intDivRoundTo == HWV_Undefined)
      {
         /* Undefined is sub-optimal */
         results->intDivRoundTo = testWarn(HWV_WARN_INT_DIV_UNDEFINED);
      }
      else
      {
         if (actual->intDivRoundTo == expected->intDivRoundTo)
         {
            results->intDivRoundTo = testPass(HWV_NO_MSG);
         }
         else
         {
            results->intDivRoundTo = testFail(HWV_NO_MSG);
         }
      }
   }

   if (expected->byteOrder == HWV_Unspecified)
   {
      /* Undefined is sub-optimal */
      results->byteOrder = testWarn(HWV_WARN_BYTE_ORDER_UNSPECIFIED);
   }
   else if (actual->byteOrder == HWV_UNABLE_TO_DETERMINE_ON_TARGET)
   {
      /* skip byte order test for targets that has all of its integer 
       * types size set to 32-bit */
      results->byteOrder = testPass(HWV_NO_MSG);
   }
   else
   {
      if (actual->byteOrder == expected->byteOrder)
      {
         results->byteOrder = testPass(HWV_NO_MSG);
      }
      else
      {
         results->byteOrder = testFail(HWV_NO_MSG);
      }
   }

   /* shiftRightBehavior
    *
    * note that shift right behavior has no impact on portable 
    * word sizes behavior because even when this option is off, 
    * portable code is generated */
   results->shiftRightBehavior = runTest(actual->shiftRightBehavior, expected->shiftRightBehavior);

   /* bitsPerChar */
   results->bitsPerChar = runTest(actual->bitsPerChar, expected->bitsPerChar);

   /* bitsPerShort */
   results->bitsPerShort = runTest(actual->bitsPerShort, expected->bitsPerShort);

   /* bitsPerInt */
   results->bitsPerInt = runTest(actual->bitsPerInt, expected->bitsPerInt);

   /* bitsPerLong */
   results->bitsPerLong = runTest(actual->bitsPerLong, expected->bitsPerLong);

   /* user has to opt in to support long long */
   if (expected->supportLongLong)
   {
      /* check actual support */
      results->supportLongLong = runTest(actual->supportLongLong, expected->supportLongLong);

      /* check bits per long long */
      results->bitsPerLongLong = runTest(actual->bitsPerLongLong, expected->bitsPerLongLong);
   }
   else
   {
      /* ok */
      results->supportLongLong = testPass(HWV_NO_MSG);
      /* ok */
      results->bitsPerLongLong = testPass(HWV_NO_MSG);
   }

   actual->bitsPerFloat = sizeof(float) * actual->bitsPerChar;
   actual->bitsPerDouble = sizeof(double) * actual->bitsPerChar;

   /* float is not configurable in the config so just warn */
   if (actual->bitsPerFloat == expected->bitsPerFloat)
   {
      results->bitsPerFloat = testPass(HWV_NO_MSG);
   }
   else
   {
      results->bitsPerFloat = testWarn(HWV_NO_MSG);
   }

   /* double is not configurable in the config so just warn
    *
    * Allow either 32 or 64 bits without warning.
    * Note that PIL has an additional run time check for 
    * non-64-bit double and will error in this case if 
    * double data types are used at the component boundary.*/
   switch (actual->bitsPerDouble)
   {
      case 32: /* drop through */
      case 64:
      results->bitsPerDouble = testPass(HWV_NO_MSG);
      break;
      default:
      results->bitsPerDouble = testWarn(HWV_NO_MSG);
      break;
   }

   /* pointer is fully configurable => error on failure */
   actual->bitsPerPointer = sizeof(void *) * actual->bitsPerChar;
   results->bitsPerPointer = runTest(actual->bitsPerPointer, expected->bitsPerPointer);

   /* size_t and ptrdiff_t defined in stddef.h */
   actual->bitsPerSizeT = sizeof(size_t) * actual->bitsPerChar;
   results->bitsPerSizeT = runTest(actual->bitsPerSizeT, expected->bitsPerSizeT);

   actual->bitsPerPtrDiffT = sizeof(ptrdiff_t) * actual->bitsPerChar;
   results->bitsPerPtrDiffT = runTest(actual->bitsPerPtrDiffT, expected->bitsPerPtrDiffT);
}

