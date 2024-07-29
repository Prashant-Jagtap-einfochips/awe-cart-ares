
#include <xtensa/hal.h>

#if defined (__cplusplus)
extern "C"
#endif
const struct xthal_MPU_entry
__xt_mpu_init_table[] __attribute__((section(".ResetHandler.text"))) = {
  XTHAL_MPU_ENTRY(0x00000000, 1, XTHAL_AR_NONE, XTHAL_MEM_DEVICE), // unused
  XTHAL_MPU_ENTRY(0x00080000, 1, XTHAL_AR_RWrw, XTHAL_MEM_NON_CACHEABLE), // dram1
  XTHAL_MPU_ENTRY(0x000a0000, 1, XTHAL_AR_RWrw, XTHAL_MEM_NON_CACHEABLE), // dram0
  XTHAL_MPU_ENTRY(0x000c0000, 1, XTHAL_AR_RWXrwx, XTHAL_MEM_NON_CACHEABLE), // iram1
  XTHAL_MPU_ENTRY(0x000e0000, 1, XTHAL_AR_RWXrwx, XTHAL_MEM_NON_CACHEABLE), // iram0
  XTHAL_MPU_ENTRY(0x00100000, 1, XTHAL_AR_NONE, XTHAL_MEM_DEVICE), // unused
  XTHAL_MPU_ENTRY(0x20100000, 1, XTHAL_AR_RWXrwx, XTHAL_MEM_WRITEBACK), // sram
  XTHAL_MPU_ENTRY(0x20900000, 1, XTHAL_AR_NONE, XTHAL_MEM_DEVICE), // unused
};

#if defined (__cplusplus)
extern "C"
#endif
const unsigned int
__xt_mpu_init_table_size __attribute__((section(".ResetHandler.text"))) = 8;

#if defined (__cplusplus)
extern "C"
#endif
const unsigned int
__xt_mpu_init_cacheadrdis __attribute__((section(".ResetHandler.text"))) = 253;


#if defined (__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
#include <assert.h>
static_assert(sizeof(__xt_mpu_init_table)/sizeof(__xt_mpu_init_table[0]) == 8, "Incorrect MPU table size");
static_assert(sizeof(__xt_mpu_init_table)/sizeof(__xt_mpu_init_table[0]) <= XCHAL_MPU_ENTRIES, "MPU table too large");
#endif

