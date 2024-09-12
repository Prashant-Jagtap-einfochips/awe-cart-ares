#ifndef __FW_COMMON__
#define __FW_COMMON__

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "Errors.h"
#include "TargetProcessor.h"

/** Base of custom ID range. */
#define CUSTOM_ID_BASE			(30000U)

struct _AWEInstance;

typedef union _Sample
{
	INT32 iVal;
	UINT32 uiVal;
	float fVal;
}
Sample;

#endif //__FW_COMMON__
