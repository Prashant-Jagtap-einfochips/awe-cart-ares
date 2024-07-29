#ifndef HIFI4SIM_CCOUNT_H
#define HIFI4SIM_CCOUNT_H

//#include <xtensa/config/core.h>


//#define hifi4_get_ccount() xt_profile_enable() 


unsigned long inline hifi4sim_get_ccount()
{
    unsigned long r;
    r = xthal_get_ccount();
    //__asm__ volatile("rsr.ccount %0" : "=r"(r));
    return r;
}

#endif
