%% Included from HW_Verify_SFunc.tlc

typedef enum
{
   HWV_NOT_RUN = 0, HWV_FAIL, HWV_PASS, HWV_WARN
} HWV_TestResultStatus;

typedef enum
{
   HWV_NO_MSG = 0,
   HWV_WARN_INT_DIV_UNDEFINED,
   HWV_WARN_BYTE_ORDER_UNSPECIFIED,
   HWV_ERROR_PWS_CONFIG_SETTING
} HWV_TestResultMsgId;

typedef struct
{
   HWV_TestResultStatus status;
   HWV_TestResultMsgId msg;
} HWV_TestResult;

typedef enum
{
   HWV_LittleEndian = 0, HWV_BigEndian, HWV_Unspecified, HWV_UNABLE_TO_DETERMINE_ON_TARGET
} HWV_EndianessValue;

typedef enum
{
   HWV_Floor = 0, HWV_Zero, HWV_Undefined
} HWV_IntDivRoundToValue;

typedef struct
{
   int bitsPerChar;
   int bitsPerShort;
   int bitsPerInt;
   int bitsPerLong;
   int bitsPerLongLong;
   int bitsPerFloat;
   int bitsPerDouble;
   int bitsPerPointer;
   int bitsPerSizeT;
   int bitsPerPtrDiffT;
   HWV_EndianessValue byteOrder;
   HWV_IntDivRoundToValue intDivRoundTo;
   int shiftRightBehavior;
   int supportLongLong;
   int portableWordSizes;
   const char * hwDeviceType;
} HWV_HWImpl;

typedef struct
{
   HWV_TestResult bitsPerChar;
   HWV_TestResult bitsPerShort;
   HWV_TestResult bitsPerInt;
   HWV_TestResult bitsPerLong;
   HWV_TestResult bitsPerLongLong;
   HWV_TestResult bitsPerFloat;
   HWV_TestResult bitsPerDouble;
   HWV_TestResult bitsPerPointer;
   HWV_TestResult bitsPerSizeT;
   HWV_TestResult bitsPerPtrDiffT;
   HWV_TestResult byteOrder;
   HWV_TestResult intDivRoundTo;
   HWV_TestResult shiftRightBehavior;
   HWV_TestResult supportLongLong;
} HWV_HWImpl_TestResults;

typedef struct
{
   HWV_TestResult configSetting;
} HWV_PWS_TestResults;

void HWV_HardwareVerifyImplementation(HWV_HWImpl* actual, const HWV_HWImpl* expected,
      HWV_HWImpl_TestResults* results);
