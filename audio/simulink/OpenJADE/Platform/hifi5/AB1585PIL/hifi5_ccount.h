#ifndef HIFI5_CCOUNT_H
#define HIFI5_CCOUNT_H

#include <hifi5_platform.h>

#pragma inline
unsigned int hifi5_get_ccount(void)
{
    // The General Purpose Timer (GPT) is used because there is no timer for the DSP clock on the AB1585
    // There is a 40x scaler because the GPT runs at 13 MHz and the DSP runs at 520 MHz
    return 40 * ( (unsigned int) *( (unsigned int *) (GPT_BASE_ADDR + GPT_COUNT_OFF) ) );
}

#endif
