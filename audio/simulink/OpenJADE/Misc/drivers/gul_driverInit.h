#if !defined(__GUL_DRIVERINIT_H__)
#define __GUL_DRIVERINIT_H__

#include <stdint.h>
#include <sys/platform.h>
#include <services/int/adi_sec.h>
#include <services/spu/adi_spu.h>

typedef struct gulDriverInit
{
   ADI_SPU_HANDLE hSpu;
   uint32_t iir0_sec_priority;
   uint32_t fir0_sec_priority;
   uint32_t emdma0_sec_priority;
   uint32_t emdma1_sec_priority;
} gulDriverInit_t;

/*
typedef int32_t (*Driver_Init_Func)(void *arg);
*/

/*
* The args is pointer to gulDriverInit_t struct.
* The function sets interrupt prioritied as supplied.
* Makes EMDMA generate secure transactions.
* Initializes EMDMA streams for use by delay and accelerator blocks.
* Initializes FIR and IIR accelerator manager frameworks.
*
* Returns 0 in success and -1 on failure.
*/
int32_t gul_driverInit(void *args);

#endif /*!defined(__GUL_DRIVERINIT_H__)*/

