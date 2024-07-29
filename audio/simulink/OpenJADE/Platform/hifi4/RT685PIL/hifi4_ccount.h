#ifndef HIFI4_CCOUNT_H
#define HIFI4_CCOUNT_H

unsigned long inline hifi4_get_ccount(void)
{
    unsigned long r;
    __asm__ volatile("rsr.ccount %0" : "=r"(r));
    return r;
}

#endif
