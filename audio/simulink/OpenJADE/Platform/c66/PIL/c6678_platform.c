#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <c6x.h>

/* CSL includes */
#define SOC_C6678 1
#include <ti/csl/csl.h>
#include <ti/csl/csl_cache.h>
#include <ti/csl/csl_cacheAux.h>
#include <ti/csl/csl_chipAux.h>
#include <ti/csl/csl_emif4fAux.h>
#include <ti/csl/csl_pscAux.h>
#include <ti/csl/csl_bootcfgAux.h>
#include <ti/csl/csl_xmcAux.h>

unsigned int c66XX_getCoreFrequency(void);
extern int platform_printf( const char *fmt, ... );

#define PRINT_VARIABLE(F, VAR) platform_printf(#VAR"\t= "#F"\n", VAR)

/**
 *  @brief This structure contains peripherals to be initialized. It provides for basic board initialization.
 *			Flash and Character device are intiialized and controlled when they are opened.
 *
 *  @remark
 *     The init flags are set when platform_init() API is called by the application during
 *     the board initialization, by default all the flags are set to 1.
 */
typedef struct {
    uint8_t pll;
    /**<1: initialize PLL */
    uint8_t ddr;
    /**<1: initialize ddr */
    uint8_t ecc;
    /**<1: initialize memory ECC checks. If 0, they are not disabled but the default power on state is disabled. */
} platform_init_flags;


#define PLLC_DIVEN_PLLDIV2    (1)
#define PLLC_DIVDS_PLLDIV2    (0)
#define PLLC_DIVEN_PLLDIV5    (1)
#define PLLC_DIVDS_PLLDIV5    (0)
#define PLLC_DIVEN_PLLDIV8    (1)
#define PLLC_DIVDS_PLLDIV8    (0)

typedef struct PllcHwSetup
{
   /** \brief Divider Enable/Disable
    *  \param CSL_BitMask32
    */
   CSL_BitMask32 divEnable;
   /** \brief PLL Multiplier
    *  \param Uint32
    */
   Uint32        pllM;
   /** \brief PLL Divider 2
    *  \param Uint32
    */
   Uint32        pllDiv2;
   /** \brief PLL Divider 5
    *  \param Uint32
    */
   Uint32        pllDiv5;
   /** \brief PLL Divider 8
    *  \param Uint32
    */
   Uint32        pllDiv8;    
   /** \brief pre Divider value
    *  \param Uint32
    */
   Uint32        preDiv;
   /** \brief post Divider value
    *  \param Uint32
    */
   Uint32        postDiv;     
   /** \brief Setup that can be used for future implementation
    *  \param void*
    */
   void*         extendSetup;
} PllcHwSetup;

CSL_Status  CorePllcGetHwSetup(PllcHwSetup*);
CSL_Status  CorePllcHwSetup (PllcHwSetup*);
CSL_Status  corePllcInit(void *);
CSL_Status  SetPaPllConfig(void);
CSL_Status  SetDDR3PllConfig();
CSL_Status  DDR3Init(void);

uint32_t platform_errno = 0;
/** Platform errno values */

#define PLATFORM_ERRNO_RESET            0				/**< No error */

#define PLATFORM_ERRNO_GENERIC          0x00000001
#define PLATFORM_ERRNO_INVALID_ARGUMENT 0x00000002		/**< NULL pointer, argument out of range, etc									*/

#define PLATFORM_ERRNO_PLL_SETUP        0x00000003		/**< Error initializing	*/
#define PLATFORM_ERRNO_EEPROM           0x00000004		/**< Error initializing	*/
#define PLATFORM_ERRNO_UART             0x00000005		/**< Error initializing	*/
#define PLATFORM_ERRNO_LED              0x00000006		/**<  Error initializing	*/
#define PLATFORM_ERRNO_I2C              0x00000007		/**<  Error initializing	*/
#define PLATFORM_ERRNO_MEMTEST          0x00000008		/**<  Error initializing	*/
#define PLATFORM_ERRNO_PHY              0x00000009		/**<  Error initializing	*/
#define PLATFORM_ERRNO_NAND             0x0000000a		/**<  Error initializing	*/
#define PLATFORM_ERRNO_NOR              0x0000000b		/**<  Generic error in NOR              */
#define	PLATFORM_ERRNO_UNSUPPORTED		0x0000000c		/**<  Functionality not supported */

#define PLATFORM_ERRNO_ECC_FAIL			0x00000010		/**<  The ECC calculation for the page read doesn't match. The application can try re-reading. */
#define PLATFORM_ERRNO_BADFLASHDEV		0x00000011		/**<  The flash routines did no not recognize the flash manufacturer			*/
#define	PLATFORM_ERRNO_FLASHADDR		0x00000012		/**<  The block or page number specified does not exist for the Flash			*/
#define	PLATFORM_ERRNO_NANDBBT			0x00000013		/**<  Could not update the NAND Bad Block Table								*/
#define PLATFORM_ERRNO_NORADDR          0x00000014		/**<  Address for NOR does not exist											*/
#define PLATFORM_ERRNO_NOFREEBLOCKS     0x00000015		/**<  There were not enough consecutive free blocks to write the data (based on your starting block number)*/

#define PLATFORM_ERRNO_DEV_TIMEOUT		0x00000020		/**<  There was an idle timeout waiting on a device action 					*/
#define PLATFORM_ERRNO_DEV_NAK			0x00000021		/**<  The device NAK'd the command												*/
#define PLATFORM_ERRNO_DEV_BUSY			0x00000022		/**<  The device reported a busy state and could not complete the operation	*/
#define PLATFORM_ERRNO_DEV_FAIL			0x00000023		/**<  Device returned a failed status											*/
#define PLATFORM_ERRNO_PSCMOD_ENABLE	0x00000024		/**<  Unable to enable the PSC Module											*/

#define PLATFORM_ERRNO_OOM				0x00000030		/**<  Out of memory.. tried to allocate RAM but could not.						*/

#define PLATFORM_ERRNO_READTO			0x00000040		/**<  UART read timeout 														*/

#define Platform_STATUS        int32_t /** Platform API return type */

#define Platform_EINVALID     -3   /**< Error code for invalid parameters */
#define Platform_EUNSUPPORTED -2   /**< Error code for unsupported feature */
#define Platform_EFAIL        -1   /**< General failure code */
#define Platform_EOK           0   /**< General success code */

/********************************************************************************************
 * 					Platform Specific Declarations											*
 *******************************************************************************************/

/* Clock rate */
#define PLATFORM_BASE_CLK_RATE_MHZ (100)

/* PREDIV */
#define PLATFORM_PLL_PREDIV_val (1)

/* POSTDIV */
#define PLATFORM_PLL_POSTDIV_val (2)

/* Default PLL PLLM value (100/1*(20/2)) = 1.0GHz) */
#define  PLATFORM_PLL1_PLLM_val (20)

/* Default PLL PLLD value for 1.0GHz) */
#define  PLATFORM_PLL1_PLLD_val (1)

/* 1/x-rate clock for CorePac (emulation) and the ADTF module */
#define  PLATFORM_PLLDIV2_val  (3)
/* 1/y-rate clock for system trace module only */
#define  PLATFORM_PLLDIV5_val (5)
/* 1/z-rate clock is used as slow_sysclck in the system */
#define  PLATFORM_PLLDIV8_val (64)

/********************************************************************************************
 * 					Function Prototypes														*
 *******************************************************************************************/

void PowerUpDomains (void);
void xmc_setup();

/********************************************************************************************
 *                     PLL control local Declarations                                          *
 *******************************************************************************************/

/**
 *  Handle to access BOOTCFG registers.
 */
#define hPscCfg     ((CSL_PscRegs*)CSL_PSC_REGS)

/**
  @}
  */
/* Boot Cfg Registers */
#define   DEVSTAT_REG           (*((volatile uint32_t *) 0x02620020))
#define   MAINPLLCTL0_REG       (*((volatile uint32_t *) 0x02620328))
#define   MAINPLLCTL1_REG       (*((volatile uint32_t *) 0x0262032C))
#define   DDR3PLLCTL0_REG       (*((volatile uint32_t *) 0x02620330))
#define   DDR3PLLCTL1_REG       (*((volatile uint32_t *) 0x02620334))

/* PA PLL Registers */
#define   PAPLLCTL0_REG         (*((volatile uint32_t *) 0x02620338))
#define   PAPLLCTL1_REG         (*((volatile uint32_t *) 0x0262033C))

#define   OBSCLCTL_REG          (*((volatile uint32_t *) 0x026203AC))

/*PLL controller registers*/
#define   PLLCTL_REG            (*((volatile uint32_t *) 0x02310100))
#define   SECCTL_REG            (*((volatile uint32_t *) 0x02310108))
#define   PLLM_REG              (*((volatile uint32_t *) 0x02310110))
#define   PLLCMD_REG            (*((volatile uint32_t *) 0x02310138))
#define   PLLSTAT_REG           (*((volatile uint32_t *) 0x0231013C))
#define   PLLALNCTL_REG         (*((volatile uint32_t *) 0x02310140))
#define   PLLDIV2_REG           (*((volatile uint32_t *) 0x0231011C))
#define   PLLDIV5_REG           (*((volatile uint32_t *) 0x02310164))
#define   PLLDIV8_REG           (*((volatile uint32_t *) 0x02310170))
#define   PREDIV_REG            (*((volatile uint32_t *) 0x02310114))

#define PA_PLL_OBS_CLK_SEL_MASK (1 << 4) /* OBSCLKCTL Register Bit 4 - set to 0 to see PA PLL reference (input) clock, set to 1 to see PA PLL output*/
#define PA_PLL_OBS_CLK_EN_MASK  (1 << 5) /* OBSCLKCTL Register Bit 5 - set to 1 to enable power to PA PLL observation clock*/

/* PA PLL Registers */
#define BYPASS_BIT_SHIFT 23
#define CLKF_BIT_SHIFT   6
#define PASSCLKSEL_MASK    (1 << 17)    /* Tells the configuration of the PASSCLKSEL pin */
#define PA_PLL_BYPASS_MASK (1 << BYPASS_BIT_SHIFT)    /* Tells whether the PA PLL is in BYPASS mode or not */
#define PA_PLL_CLKOD_MASK  (0x00780000) /* Tells the output divider value for the PA PLL */
#define PA_PLL_CLKF_MASK   (0x0007FFC0) /* Tells the multiplier value for the PA PLL */
#define PA_PLL_CLKR_MASK   (0x0000003F) /* Tells the divider value for the PA PLL */
#define PLL1_BASE           0x02310000
#define PLL1_PLLCTL              (*(unsigned int*)(PLL1_BASE + 0x100))   // PLL1 Control
#define PLL1_SECCTL              (*(unsigned int*)(PLL1_BASE + 0x108))   // PLL1 Sec Control
#define PLL1_PLLM                (*(unsigned int*)(PLL1_BASE + 0x110))   // PLL1 Multiplier
#define PLL1_DIV1                (*(unsigned int*)(PLL1_BASE + 0x118))   // DIV1 divider
#define PLL1_DIV2                (*(unsigned int*)(PLL1_BASE + 0x11C))   // DIV2 divider
#define PLL1_DIV3                (*(unsigned int*)(PLL1_BASE + 0x120))   // DIV3 divider
#define PLL1_CMD                 (*(unsigned int*)(PLL1_BASE + 0x138))   // CMD control
#define PLL1_STAT                (*(unsigned int*)(PLL1_BASE + 0x13C))   // STAT control
#define PLL1_ALNCTL              (*(unsigned int*)(PLL1_BASE + 0x140))   // ALNCTL control
#define PLL1_DCHANGE             (*(unsigned int*)(PLL1_BASE + 0x144))   // DCHANGE status
#define PLL1_CKEN                (*(unsigned int*)(PLL1_BASE + 0x148))   // CKEN control
#define PLL1_CKSTAT              (*(unsigned int*)(PLL1_BASE + 0x14C))   // CKSTAT status
#define PLL1_SYSTAT              (*(unsigned int*)(PLL1_BASE + 0x150))   // SYSTAT status
#define PLL1_DIV4                (*(unsigned int*)(PLL1_BASE + 0x160))   // DIV4 divider
#define PLL1_DIV5                (*(unsigned int*)(PLL1_BASE + 0x164))   // DIV5 divider
#define PLL1_DIV6                (*(unsigned int*)(PLL1_BASE + 0x168))   // DIV6 divider
#define PLL1_DIV7                (*(unsigned int*)(PLL1_BASE + 0x16C))   // DIV7 divider
#define PLL1_DIV8                (*(unsigned int*)(PLL1_BASE + 0x170))   // DIV8 divider
#define PLL1_DIV9                (*(unsigned int*)(PLL1_BASE + 0x174))   // DIV9 divider
#define PLL1_DIV10               (*(unsigned int*)(PLL1_BASE + 0x178))   // DIV10 divider
#define PLL1_DIV11               (*(unsigned int*)(PLL1_BASE + 0x17C))   // DIV11 divider
#define PLL1_DIV12               (*(unsigned int*)(PLL1_BASE + 0x180))   // DIV12 divider
#define PLL1_DIV13               (*(unsigned int*)(PLL1_BASE + 0x184))   // DIV13 divider
#define PLL1_DIV14               (*(unsigned int*)(PLL1_BASE + 0x188))   // DIV14 divider
#define PLL1_DIV15               (*(unsigned int*)(PLL1_BASE + 0x18C))   // DIV15 divider
#define PLL1_DIV16               (*(unsigned int*)(PLL1_BASE + 0x190))   // DIV16 divider

#define BWADJ_BIT_SHIFT     24
#define BYPASS_BIT_SHIFT    23
#define CLKF_BIT_SHIFT      6
#define PA_PLL_BYPASS_MASK (1 << BYPASS_BIT_SHIFT)    /* Tells whether the PA PLL is in BYPASS mode or not */
#define PA_PLL_BWADJ_MASK (1 << BWADJ_BIT_SHIFT) /* Tells the bandwidth adjust value for the PA PLL */
#define PA_PLL_CLKOD_MASK  (0x00780000) /* Tells the output divider value for the PA PLL */
#define PA_PLL_CLKF_MASK   (0x0007FFC0) /* Tells the multiplier value for the PA PLL */
#define PA_PLL_CLKR_MASK   (0x0000003F) /* Tells the divider value for the PA PLL */

#define ENSAT_BIT_SHIFT 6
#define RESET_BIT_SHIFT 14
#define PA_PLL_ENSAT_MASK (1 << ENSAT_BIT_SHIFT) /* Tells the configuration of the ENSAT bit */
#define PA_PLL_RESET_MASK (1 << RESET_BIT_SHIFT) /* Tells the configuration of the RESET bit */

/* PASS PLL settings for 1044 MHz */
#define PLLM_PASS 20
#define PLLD_PASS 0

/* DDR3 PLL settings for 1333 MHz */
#define PLLM_DDR3 19
#define PLLD_DDR3 0

#pragma FUNC_CANNOT_INLINE ( pll_delay );
static void pll_delay(uint32_t ix)
{
   while (ix--)
   {
      asm("   NOP");
   }
}

#pragma FUNC_CANNOT_INLINE ( platform_delaycycles );
static void platform_delaycycles(uint32_t cycles)
{
	uint32_t start_val  = CSL_chipReadTSCL();

	while ((CSL_chipReadTSCL() - start_val) < cycles);

    return;
}


#pragma FUNC_CANNOT_INLINE ( prog_pll1_values );
static int prog_pll1_values(PllcHwSetup *hwSetup)
{
   uint32_t temp;

   /* Check the Range for Multiplier and Divider (error > 63) here */
   if (hwSetup->pllM > 4095)
   {
      return -1;
   }

   if (hwSetup->preDiv > 63)
   {
      return -1;
   }

   /* Set the PLL Multiplier, Divider, BWADJ                                    *
    * The PLLM[5:0] bits of the multiplier are controlled by the PLLM Register  *
    * inside the PLL Controller and the PLLM[12:6] bits are controlled by the   *
    * chip-level MAINPLLCTL0 Register.                                          *
    * PLL Control Register (PLLM)  Layout                                       *
    * |31...6   |5...0        |                                                 *
    * |Reserved |PLLM         |                                                 *
    *                                                                           *
    * Main PLL Control Register (MAINPLLCTL0)                                   *
    * |31...24   |23...19   |18...12    | 11...6   |5...0 |                     *
    * |BWADJ[7:0]| Reserved |PLLM[12:6] | Reserved | PLLD |                     */

   /* Set pll multipler (13 bit field) */
   PLLM_REG     = (hwSetup->pllM & 0x0000003F); /* bits[5:0]  */
   temp          = (hwSetup->pllM & 0x1FC0) >> 6;       /* bits[12:6] */
   MAINPLLCTL0_REG  &=~(0x0007F000);                /*Clear PLLM field */
   MAINPLLCTL0_REG  |=((temp << 12) & 0x0007F000);

   /* Set the BWADJ     (12 bit field)                                          *
    * BWADJ[11:8] and BWADJ[7:0] are located in MAINPLLCTL0 and MAINPLLCTL1     *
    * registers. BWADJ[11:0] should be programmed to a value equal to half of   *
    * PLLM[12:0] value (round down if PLLM has an odd value)                    *
    * Example: If PLLM = 15, then BWADJ = 7                                     */
   temp = ((hwSetup->pllM + 1)>> 1) - 1; /* Divide the pllm by 2 */
   MAINPLLCTL0_REG &=~(0xFF000000);  /* Clear the BWADJ Field */
   MAINPLLCTL0_REG |=  ((temp << 24) & 0xFF000000);
   MAINPLLCTL1_REG &=~(0x0000000F);   /* Clear the BWADJ field */
   MAINPLLCTL1_REG |= ((temp >> 8) & 0x0000000F);

   /* Set the pll divider (6 bit field)                                         *
    * PLLD[5:0] is located in MAINPLLCTL0                                       */
   MAINPLLCTL0_REG   &= ~(0x0000003F);    /* Clear the Field */
   MAINPLLCTL0_REG   |= (hwSetup->preDiv & 0x0000003F);

   /* Set the OUTPUT DIVIDE (4 bit field) in SECCTL */
   SECCTL_REG    &= ~(0x00780000);     /* Clear the field       */
   SECCTL_REG   |= ((1 << 19) & 0x00780000) ;

   return (0);
}

#pragma FUNC_CANNOT_INLINE ( CorePllcHwSetup );
CSL_Status CorePllcHwSetup (PllcHwSetup *hwSetup)
{
   CSL_Status       status = CSL_SOK;
   volatile uint32_t i, loopCount;
   uint32_t temp;

   /* Unlock the Boot Config */
   CSL_BootCfgUnlockKicker();


   /* 1. Wait for Stabilization time (min 100 us)                             *
    * The below loop is good enough for the Gel file to get minimum of        *
    * 100 micro seconds, this should be appropriately modified for port       *
    * to a C function                                                         *
    * Minimum delay in GEL can be 1 milli seconds, so program to 1ms=1000us,  *
    * more than required, but should be Okay                                  */
   pll_delay(140056);

   /* 2. Check the status of BYPASS bit in SECCTL register,                   *
    *    execute the following steps if                                       *
    *    BYPASS == 1 (if bypass enabled), if BYPASS==0 then Jump to Step 3    */
   temp = SECCTL_REG & 0x00800000; /* Check the Bit 23 value */

   if (temp != 0) /* PLL BYPASS is enabled, we assume if not in Bypass ENSAT = 1 */
   {
      /* 2a. Usage Note 9: For optimal PLL operation, the ENSAT bit in the PLL control *
       * registers for the Main PLL, DDR3 PLL, and PA PLL should be set to 1.          *
       * The PLL initialization sequence in the boot ROM sets this bit to 0 and        *
       * could lead to non-optimal PLL operation. Software can set the bit to the      *
       * optimal value of 1 after boot                                                 *
       * Ref: http://www.ti.com/lit/er/sprz334b/sprz334b.pdf                           *
       * |31...7   |6     |5 4       |3...0      |                                     *
       * |Reserved |ENSAT |Reserved  |BWADJ[11:8]|                                     */

      MAINPLLCTL1_REG = MAINPLLCTL1_REG | 0x00000040;

      /* 2b. Clear PLLEN bit (bypass enabled in PLL controller mux) */
      PLLCTL_REG &= ~(1 << 0);

      /* 2c. Clear PLLENSRC bit (enable PLLEN to control PLL controller mux) */
      PLLCTL_REG &= ~(1 << 5);

      /* 2d. Wait for 4 RefClks(to make sure the PLL controller *
       * mux switches properly to the bypass)                   *
       * Assuming slowest Ref clock of 25MHz, min: 160 ns delay */
      pll_delay(225);

      /* 2e. Bypass needed to perform PWRDN cycle for C6670 and C6657              *
       * Needed on all devices when in NOBOOT, I2C or SPI boot modes               *
       * Ref: Figure 4-2 of http://www.ti.com/lit/ug/sprugv2a/sprugv2a.pdf         *
       * PLL Secondary Control Register (SECCTL)  Layout                           *
       * |31...24  |23     |22...19       |18...0   |                              *
       * |Reserved |BYPASS |OUTPUT DIVIDE |Reserved |                              */
      SECCTL_REG |= 0x00800000; /* Set the Bit 23 */

      /* 2f. Advisory 8: Multiple PLLs May Not Lock After Power-on Reset Issue     *
       * In order to ensure proper PLL startup, the PLL power_down pin needs to be *
       * toggled. This is accomplished by toggling the PLLPWRDN bit in the PLLCTL  *
       * register. This needs to be done before the main PLL initialization        *
       * sequence                                                                  *
       * Ref: Figure 4-1 of http://www.ti.com/lit/ug/sprugv2a/sprugv2a.pdf         *
       * PLL Control Register (PLLCTL)  Layout                                     *
       * |31...4   |3      |2        |1        |0        |                         *
       * |Reserved |PLLRST |Reserved |PLLPWRDN |Reserved |                         */

      PLLCTL_REG |= 0x00000002; /* Power Down the PLL */

      /* 2g. Stay in a loop such that the bit is set for 5 µs (minimum) and        *
       * then clear the bit.                                                       */

      pll_delay(14005);

      /* 2h. Power up the PLL */
      PLLCTL_REG &= ~(0x00000002);
   }
   else
   {
      /* 3. Enable BYPASS in the PLL contoller */

      /* 3a. Clear PLLEN bit (bypass enabled in PLL controller mux) */
      PLLCTL_REG &= ~(1 << 0);

      /* 3b. Clear PLLENSRC bit (enable PLLEN to control PLL controller mux) */
      PLLCTL_REG &= ~(1 << 5);

      /* 3c. Wait for 4 RefClks (to make sure the PLL controller *
       * mux switches properly to bypass)                        *
       * Assuming slowest Ref clock of 25MHz, min: 160 ns delay  */
      pll_delay(225);
   }

   /* 4, 5, 6 and 7 are done here:                                             *
    * Program the necessary multipliers/dividers and BW adjustments            */
   prog_pll1_values(hwSetup);

   /* 8. Set PLL dividers if needed */

   /* part of 8, go stat bit needs to be zero here                         *
     * Read the GOSTAT bit in PLLSTAT to make sure the bit reurns to 0 to   *
     * indicate that the GO operation has completed                         *
     * wait for the GOSTAT, but don't trap if lock is never read            */
   for (i = 0; i < 100; i++)
   {
      pll_delay(300);
      if ( (PLL1_STAT & 0x00000001) == 0 )
      {
         break;
      }
   }
   if ( i == 100 )
   {
      return CSL_ESYS_FAIL;
   }

   /* part of 8, Set PLL dividers if needed */
   PLL1_DIV2 = (0x8000) | (hwSetup->pllDiv2);
   PLL1_DIV5 = (0x8000) | (hwSetup->pllDiv5);
   PLL1_DIV8 = (0x8000) | (hwSetup->pllDiv8);

   /* part of 8, Program ALNCTLn     *
    * Set bit 1, 4 and 7             */
   PLLALNCTL_REG |= ((1 << 1) | (1 << 4) | (1 << 7));

   /* part of 8, Set GOSET bit in PLLCMD to initiate the GO operation to change the divide *
    * values and align the SYSCLKSs as programmed                                          */
   PLLCMD_REG |= 0x00000001;

   /* part of 8, go stat bit needs to be zero here                         *
    * Read the GOSTAT bit in PLLSTAT to make sure the bit reurns to 0 to   *
    * indicate that the GO operation has completed                         *
    * wait for the GOSTAT, but don't trap if lock is never read            */
   for (i = 0; i < 100; i++)
   {
      pll_delay(300);
      if ( (PLL1_STAT & 0x00000001) == 0 )
      {
         break;
      }
   }
   if ( i == 100 )
   {
      return CSL_ESYS_FAIL;
   }

   /* 9. Place PLL in Reset, In PLLCTL, write PLLRST = 1 (PLL is reset)        */
   PLLCTL_REG |= 0x00000008;

   /* 10. Wait for PLL Reset assertion Time (min: 7 us)                        */
   pll_delay(14006);

   /* 11. In PLLCTL, write PLLRST = 0 (PLL reset is de-asserted) */
   PLLCTL_REG &= ~(0x00000008);

   /* 12. PLL Lock Delay needs to be 500 RefClk periods * (PLLD + 1)           *
    * i.e., Wait for at least 500 * CLKIN cycles * (PLLD + 1) (PLL lock timer) *
    * Using 2000 25ns RefClk periods per DM                                    *
    * Wait for PLL to lock min 50 us                                           */
   pll_delay(140056 >> 1);

   /* 13. In SECCTL, write BYPASS = 0 (enable PLL mux to switch to PLL mode) */
   SECCTL_REG &= ~(0x00800000); /* Release Bypass */

   /* 14. In PLLCTL, write PLLEN = 1 (enable PLL controller mux to switch to PLL mode) */
   PLLCTL_REG |= (1 << 0);

   /* 15. The PLL and PLL Controller are now initialized in PLL mode - Completed. */

   return status;
}

#pragma FUNC_CANNOT_INLINE ( CorePllcGetHwSetup );
CSL_Status CorePllcGetHwSetup (PllcHwSetup *hwSetup)
{
   CSL_Status       status   = CSL_SOK;
   volatile uint32_t i, loopCount;

   /* Unlock the Boot Config */
   CSL_BootCfgUnlockKicker();

   hwSetup->divEnable = 0;

   hwSetup->pllM       = (PLLM_REG & 0x3F);
   hwSetup->preDiv     = PREDIV_REG;
   hwSetup->pllDiv2    = PLLDIV2_REG ;
   hwSetup->pllDiv5    = PLLDIV5_REG;
   hwSetup->pllDiv8    = PLLDIV8_REG;

   /* wait for the GOSTAT, but don't trap if lock is never read */
   for (i = 0; i < 100; i++)
   {
      pll_delay(300);
      if ( (PLL1_STAT & 0x00000001) == 0 )
      {
         break;
      }
   }
   if (i == 100)
   {
      return CSL_ESYS_FAIL;
   }

   return status;
}

#pragma FUNC_CANNOT_INLINE ( SetPaPllConfig );
CSL_Status SetPaPllConfig(void) 
{
   uint32_t passclksel = (DEVSTAT_REG & PASSCLKSEL_MASK);
   uint32_t papllctl0val = PAPLLCTL0_REG;
   uint32_t obsclkval = OBSCLCTL_REG;
   uint32_t pa_pllm = PLLM_PASS;
   uint32_t pa_plld = PLLD_PASS;
   uint32_t temp;
   int pass_freq;
   int pass_freM,pass_freD;

   if (passclksel == 0)
   {
#if defined(DEBUG)
      platform_printf("SYSCLK/ALTCORECLK is the input to the PA PLL...\n");
#endif
   }

   /* Unlock the Boot Config */
   CSL_BootCfgUnlockKicker();

   if (DNUM == 0)
   {
      /* Usage Note 9: For optimal PLL operation, the ENSAT bit in the PLL control *
       * registers for the Main PLL, DDR3 PLL, and PA PLL should be set to 1.      *
       * The PLL initialization sequence in the boot ROM sets this bit to 0 and    *
       * could lead to non-optimal PLL operation. Software can set the bit to the  *
       * optimal value of 1 after boot                                             *
       * PAPLLCTL1_REG Bit map                                                     *
       * |31...7   |6     |5 4       |3...0      |                                 *
       * |Reserved |ENSAT |Reserved  |BWADJ[11:8]|                                 */

      PAPLLCTL1_REG |= 0x00000040;

      /* Wait for the PLL Reset time (min: 1000 ns)                                */
      /*pll_delay(1400);*/

      /* Put the PLL in Bypass Mode                                                *
       * PAPLLCTL0_REG Bit map                                                     *
       * |31...24    |23     |22...19       |18...6   |5...0 |                     *
       * |BWADJ[7:0] |BYPASS |Reserved      |PLLM     |PLLD  |                     */

      PAPLLCTL0_REG |= 0x00800000; /* Set the Bit 23 */

      /*Wait 4 cycles for the slowest of PLLOUT or reference clock source (CLKIN)*/
      /*pll_delay(4);*/

      /* Wait for the PLL Reset time (min: 1000 ns)                                */
      /*pll_delay(1400);*/

      /* Program the necessary multipliers/dividers and BW adjustments             */
      /* Set the divider values */
      PAPLLCTL0_REG &= ~(0x0000003F);
      PAPLLCTL0_REG |= (pa_plld & 0x0000003F);

      /* Set the Multipler values */
      PAPLLCTL0_REG &= ~(0x0007FFC0);
      PAPLLCTL0_REG |= ((pa_pllm << 6) & 0x0007FFC0 );

      /* Set the BWADJ */
      temp = ((pa_pllm + 1) >> 1) - 1;
      PAPLLCTL0_REG &= ~(0xFF000000); 
      PAPLLCTL0_REG |= ((temp << 24) & 0xFF000000);
      PAPLLCTL1_REG &= ~(0x0000000F);
      PAPLLCTL1_REG |= ((temp >> 8) & 0x0000000F);

      /* In PLL Controller, reset the PLL (bit 14) in PAPLLCTL1_REG register       */
      PAPLLCTL1_REG |= 0x00004000;

      /*Wait for PLL to lock min 5 micro seconds*/
      pll_delay(7000);

      /*In PAPLLCTL1_REG, write PLLSELECT = 1 (for selecting the output of PASS PLL as the input to PASS) */
      PAPLLCTL1_REG |= 0x00002000;

      /*In PAPLLCTL1_REG, write PLLRST = 0 to bring PLL out of reset */
      PAPLLCTL1_REG &= ~(0x00004000);

      /*Wait for PLL to lock min 50 micro seconds*/
      pll_delay(70000);

      /* Put the PLL in PLL Mode                                                   *
       * PAPLLCTL0_REG Bit map                                                     *
       * |31...24    |23     |22...19       |18...6   |5...0 |                     *
       * |BWADJ[7:0] |BYPASS |Reserved      |PLLM     |PLLD  |                     */
      PAPLLCTL0_REG &= ~(0x00800000); /* ReSet the Bit 23 */

      papllctl0val = PAPLLCTL0_REG;

      /* Tells the multiplier value for the PA PLL */
      pa_pllm = (((papllctl0val & PA_PLL_CLKF_MASK) >> 6) + 1);
#if defined(DEBUG)
      platform_printf("PA PLL programmable multiplier = %d\n", pa_pllm);
#endif
      /* Tells the divider value for the PA PLL */
      pa_plld = (((papllctl0val & PA_PLL_CLKR_MASK) >> 0) +1);
#if defined(DEBUG)
      platform_printf("PA PLL programmable divider = %d\n", pa_plld);
#endif

      // Compute the real pass clk freq (*100)
      pass_freq = (12288 * (pa_pllm) / (pa_plld) / (1));

      // Displayed frequency in MHz
      pass_freM = pass_freq / 100;

      // passclk freq first decimal if freq expressed in MHz
      pass_freD = ((pass_freq - pass_freM * 100) + 5) / 10;

      // Add roundup unit to MHz displayed and reajust decimal value if necessary...
      if (pass_freD > 9)
      {
         pass_freD = pass_freD - 10;
         pass_freM = pass_freM + 1;
      }

#if defined(DEBUG)
      platform_printf("PLL3 Setup for PASSCLK @ %d.%d MHz... \n", pass_freM, pass_freD );
      platform_printf("PLL3 Setup... Done.\n" );
#endif

      return CSL_SOK;

   }
   else
   {
      platform_printf("DSP core #%d cannot set PA PLL \n",DNUM);
      return CSL_ESYS_FAIL;
   }

}

/***************************************************************************************
 * FUNCTION PURPOSE: Power up all the power domains
 ***************************************************************************************
 * DESCRIPTION: this function powers up the PA subsystem domains
 ***************************************************************************************/
#pragma FUNC_CANNOT_INLINE ( PowerUpDomains );
void PowerUpDomains (void)
{
   /* PASS power domain is turned OFF by default. It needs to be turned on before doing any 
    * PASS device register access. This not required for the simulator. */

   /* Set PASS Power domain to ON */
   CSL_PSC_enablePowerDomain (CSL_PSC_PD_ALWAYSON);

   /* Enable the clocks for PASS modules */
   CSL_PSC_setModuleNextState (CSL_PSC_LPSC_EMIF4F, PSC_MODSTATE_ENABLE);
   CSL_PSC_setModuleNextState (CSL_PSC_LPSC_EMIF25_SPI,  PSC_MODSTATE_ENABLE);

   /* Start the state transition */
   CSL_PSC_startStateTransition (CSL_PSC_PD_ALWAYSON);

   /* Wait until the state transition process is completed. */
   while (!CSL_PSC_isStateTransitionDone (CSL_PSC_PD_ALWAYSON));        


   /* PASS power domain is turned OFF by default. It needs to be turned on before doing any 
    * PASS device register access. This not required for the simulator. */

   /* Set PASS Power domain to ON */
   CSL_PSC_enablePowerDomain (CSL_PSC_PD_PASS);

   /* Enable the clocks for PASS modules */
   CSL_PSC_setModuleNextState (CSL_PSC_LPSC_PKTPROC, PSC_MODSTATE_ENABLE);
   CSL_PSC_setModuleNextState (CSL_PSC_LPSC_CPGMAC,  PSC_MODSTATE_ENABLE);
   CSL_PSC_setModuleNextState (CSL_PSC_LPSC_Crypto,  PSC_MODSTATE_ENABLE);

   /* Start the state transition */
   CSL_PSC_startStateTransition (CSL_PSC_PD_PASS);

   /* Wait until the state transition process is completed. */
   while (!CSL_PSC_isStateTransitionDone (CSL_PSC_PD_PASS));
}

#pragma FUNC_CANNOT_INLINE ( SetDDR3PllConfig );
CSL_Status SetDDR3PllConfig(void) 
{

   uint32_t ddr3pllctl0val = DDR3PLLCTL0_REG;
   uint32_t obsclkval = OBSCLCTL_REG;
   uint32_t ddr3_pllm = PLLM_DDR3;
   uint32_t ddr3_plld = PLLD_DDR3;
   uint32_t temp;

   /* Unlock the Boot Config */
   CSL_BootCfgUnlockKicker();

   if (DNUM == 0)
   {
      /* Usage Note 9: For optimal PLL operation, the ENSAT bit in the PLL control *
       * registers for the Main PLL, DDR3 PLL, and PA PLL should be set to 1.      *
       * The PLL initialization sequence in the boot ROM sets this bit to 0 and    *
       * could lead to non-optimal PLL operation. Software can set the bit to the  *
       * optimal value of 1 after boot                                             *
       * DDR3PLLCTL1_REG Bit map                                                     *
       * |31...7   |6     |5 4       |3...0      |                                 *
       * |Reserved |ENSAT |Reserved  |BWADJ[11:8]|                                 */

      DDR3PLLCTL1_REG |= 0x00000040;

      /* Wait for the PLL Reset time (min: 1000 ns)                                */
      /*pll_delay(1400);*/

      /* Put the PLL in Bypass Mode                                                *
       * DDR3PLLCTL0_REG Bit map                                                     *
       * |31...24    |23     |22...19       |18...6   |5...0 |                     *
       * |BWADJ[7:0] |BYPASS |Reserved      |PLLM     |PLLD  |                     */

      DDR3PLLCTL0_REG |= 0x00800000; /* Set the Bit 23 */

      /*Wait 4 cycles for the slowest of PLLOUT or reference clock source (CLKIN)*/
      /*pll_delay(4);*/

      /* Wait for the PLL Reset time (min: 1000 ns)                                */
      /*pll_delay(1400);*/

      /* Program the necessary multipliers/dividers and BW adjustments             */
      /* Set the divider values */
      DDR3PLLCTL0_REG &= ~(0x0000003F);
      DDR3PLLCTL0_REG |= (ddr3_plld & 0x0000003F);

      /* Set the Multipler values */
      DDR3PLLCTL0_REG &= ~(0x0007FFC0);
      DDR3PLLCTL0_REG |= ((ddr3_pllm << 6) & 0x0007FFC0 );

      /* Set the BWADJ */
      temp = ((ddr3_pllm + 1) >> 1) - 1;
      DDR3PLLCTL0_REG &= ~(0xFF000000); 
      DDR3PLLCTL0_REG |= ((temp << 24) & 0xFF000000);
      DDR3PLLCTL1_REG &= ~(0x0000000F);
      DDR3PLLCTL1_REG |= ((temp >> 8) & 0x0000000F);

      /* In PLL Controller, reset the PLL (bit 13) in DDR3PLLCTL1_REG register       */
      DDR3PLLCTL1_REG |= 0x00002000;

      /*Wait for PLL to lock min 5 micro seconds*/
      pll_delay(7000);

      /*In DDR3PLLCTL1_REG, write PLLRST = 0 to bring PLL out of reset */
      DDR3PLLCTL1_REG &= ~(0x00002000);

      /*Wait for PLL to lock min 50 micro seconds*/
      pll_delay(70000);

      /* Put the PLL in PLL Mode                                                   *
       * DDR3PLLCTL0_REG Bit map                                                   *
       * |31...24    |23     |22...19       |18...6   |5...0 |                     *
       * |BWADJ[7:0] |BYPASS |Reserved      |PLLM     |PLLD  |                     */
      DDR3PLLCTL0_REG &= ~(0x00800000); /* ReSet the Bit 23 */

      ddr3pllctl0val = DDR3PLLCTL0_REG;

      /* Tells the multiplier value for the DDR3 PLL */
      ddr3_pllm = (((ddr3pllctl0val & 0x0007FFC0) >> 6) + 1);

      /* Tells the divider value for the DDR3 PLL */
      ddr3_plld = (((ddr3pllctl0val & 0x0000003F) >> 0) +1);

      pll_delay(7000000);
#if defined(DEBUG)
      platform_printf("DDR3 PLL programmable multiplier = %d\n", ddr3_pllm);
      platform_printf("DDR3 PLL programmable divider = %d\n", ddr3_plld);
      platform_printf("DDR3 PLL Setup... Done.\n" );
#endif

      return CSL_SOK;

   }
   else
   {
      platform_printf("DSP core #%d cannot set DDR3 PLL \n",DNUM);
      return CSL_ESYS_FAIL;
   }
}


/*--------------------------------------------------------------*/
/* xmc_setup()                                                  */
/* XMC MPAX register setting to access DDR3 config space        */
/*--------------------------------------------------------------*/
#pragma FUNC_CANNOT_INLINE ( xmc_setup );
void xmc_setup(void)
{
   /* mapping for ddr emif registers XMPAX*2 */
   CSL_XMC_XMPAXL    mpaxl;
   CSL_XMC_XMPAXH    mpaxh;

   /* base addr + seg size (64KB)*/    //"1B"-->"B" by xj */
   mpaxh.bAddr     = (0x2100000B >> 12);
   mpaxh.segSize   = (0x2100000B & 0x0000001F);

   /* replacement addr + perm*/
   mpaxl.rAddr     = 0x100000;
   mpaxl.sr        = 1;
   mpaxl.sw        = 1;
   mpaxl.sx        = 1;
   mpaxl.ur        = 1;
   mpaxl.uw        = 1;
   mpaxl.ux        = 1;

   /* set the xmpax for index2 */
   CSL_XMC_setXMPAXH(2, &mpaxh);
   CSL_XMC_setXMPAXL(2, &mpaxl);    
}

/* Set the desired DDR3 configuration -- assumes 66.67 MHz DDR3 clock input */
#pragma FUNC_CANNOT_INLINE ( DDR3Init );
CSL_Status DDR3Init(void) 
{

   CSL_Status status = CSL_SOK;    
   volatile unsigned int loopCount;
   uint32_t ddr3config, ddrPhyCtrl;
   uint8_t ddrPHYReadLatency;
   EMIF4F_TIMING1_CONFIG sdram_tim1;
   EMIF4F_TIMING2_CONFIG sdram_tim2;
   EMIF4F_TIMING3_CONFIG sdram_tim3;
   EMIF4F_OUTPUT_IMP_CONFIG    zqcfg;
   EMIF4F_PWR_MGMT_CONFIG      pwrmgmtcfg;
   EMIF4F_SDRAM_CONFIG         sdramcfg;

   CSL_BootCfgUnlockKicker();        

   /* Wait for PLL to lock = min 500 ref clock cycles. 
      With refclk = 100MHz, = 5000 ns = 5us */
   platform_delaycycles(50000);

   /**************** 3.3 Leveling Register Configuration ********************/
   CSL_BootCfgGetDDRConfig(0, &ddr3config);
   ddr3config &= ~(0x007FE000);  // clear ctrl_slave_ratio field
   CSL_BootCfgSetDDRConfig(0, ddr3config);

   CSL_BootCfgGetDDRConfig(0, &ddr3config);
   ddr3config |= 0x00200000;     // set ctrl_slave_ratio to 0x100
   CSL_BootCfgSetDDRConfig(0, ddr3config);

   CSL_BootCfgGetDDRConfig(12, &ddr3config);
   ddr3config |= 0x08000000;    // Set invert_clkout = 1
   CSL_BootCfgSetDDRConfig(12, ddr3config);

   CSL_BootCfgGetDDRConfig(0, &ddr3config);
   ddr3config |= 0xF;            // set dll_lock_diff to 15
   CSL_BootCfgSetDDRConfig(0, ddr3config);

   CSL_BootCfgGetDDRConfig(23, &ddr3config);
   ddr3config |= 0x00000200;    // See section 4.2.1, set for partial automatic levelling
   CSL_BootCfgSetDDRConfig(23, ddr3config);

   /**************** 3.3 Partial Automatic Leveling ********************/
   ddr3config = 0x0000005E    ; CSL_BootCfgSetDDRConfig(2,  ddr3config);
   ddr3config = 0x0000005E;  CSL_BootCfgSetDDRConfig(3,  ddr3config);
   ddr3config = 0x0000005E;  CSL_BootCfgSetDDRConfig(4,  ddr3config);
   ddr3config = 0x00000051;  CSL_BootCfgSetDDRConfig(5,  ddr3config);
   ddr3config = 0x00000038;  CSL_BootCfgSetDDRConfig(6,  ddr3config);
   ddr3config = 0x0000003A;  CSL_BootCfgSetDDRConfig(7,  ddr3config);
   ddr3config = 0x00000024;  CSL_BootCfgSetDDRConfig(8,  ddr3config);
   ddr3config = 0x00000020;  CSL_BootCfgSetDDRConfig(9,  ddr3config);
   ddr3config = 0x00000044;  CSL_BootCfgSetDDRConfig(10, ddr3config);

   ddr3config = 0x000000DD;  CSL_BootCfgSetDDRConfig(14,  ddr3config);
   ddr3config = 0x000000DD;  CSL_BootCfgSetDDRConfig(15,  ddr3config);
   ddr3config = 0x000000BE;  CSL_BootCfgSetDDRConfig(16,  ddr3config);
   ddr3config = 0x000000CA;  CSL_BootCfgSetDDRConfig(17,  ddr3config);
   ddr3config = 0x000000A9;  CSL_BootCfgSetDDRConfig(18,  ddr3config);
   ddr3config = 0x000000A7;  CSL_BootCfgSetDDRConfig(19,  ddr3config);
   ddr3config = 0x0000009E;  CSL_BootCfgSetDDRConfig(20,  ddr3config);
   ddr3config = 0x000000A1;  CSL_BootCfgSetDDRConfig(21,  ddr3config);
   ddr3config = 0x000000BA;  CSL_BootCfgSetDDRConfig(22,  ddr3config);

   /*Do a PHY reset. Toggle DDR_PHY_CTRL_1 bit 15 0->1->0 */
   CSL_EMIF4F_GetPhyControl(&ddrPhyCtrl, &ddrPHYReadLatency);
   ddrPhyCtrl &= ~(0x00008000);
   CSL_EMIF4F_SetPhyControl(ddrPhyCtrl,  ddrPHYReadLatency);

   CSL_EMIF4F_GetPhyControl(&ddrPhyCtrl, &ddrPHYReadLatency);
   ddrPhyCtrl |= (0x00008000);
   CSL_EMIF4F_SetPhyControl(ddrPhyCtrl,  ddrPHYReadLatency);

   CSL_EMIF4F_GetPhyControl(&ddrPhyCtrl, &ddrPHYReadLatency);
   ddrPhyCtrl &= ~(0x00008000);
   CSL_EMIF4F_SetPhyControl(ddrPhyCtrl,  ddrPHYReadLatency);

   /***************** 3.4 Basic Controller and DRAM configuration ************/
   /* enable configuration */
   /*    hEmif->SDRAM_REF_CTRL    = 0x00005162; */
   CSL_EMIF4F_EnableInitRefresh();
   CSL_EMIF4F_SetRefreshRate(0x5162);

   /*    hEmif->SDRAM_TIM_1   = 0x1113783C; */

   sdram_tim1.t_wtr    = 4;
   sdram_tim1.t_rrd    = 7;
   sdram_tim1.t_rc     = 0x20;
   sdram_tim1.t_ras    = 0x17;
   sdram_tim1.t_wr     = 9;
   sdram_tim1.t_rcd    = 8;
   sdram_tim1.t_rp     = 8;
   CSL_EMIF4F_SetTiming1Config(&sdram_tim1);

   /*    hEmif->SDRAM_TIM_2   = 0x30717FE3; */
   sdram_tim2.t_cke    = 3;
   sdram_tim2.t_rtp    = 4;
   sdram_tim2.t_xsrd   = 0x1FF;
   sdram_tim2.t_xsnr   = 0x071;
   sdram_tim2.t_xp     = 3;
   sdram_tim2.t_odt    = 0;
   CSL_EMIF4F_SetTiming2Config (&sdram_tim2);

   /*    hEmif->SDRAM_TIM_3   = 0x559F86AF; */
   sdram_tim3.t_rasMax     = 0xF;
   sdram_tim3.t_rfc        = 0x06A;
   sdram_tim3.t_tdqsckmax  = 0;
   sdram_tim3.zq_zqcs      = 0x3F;
   sdram_tim3.t_ckesr      = 4;
   sdram_tim3.t_csta       = 0x5;
   sdram_tim3.t_pdll_ul    = 0x5;
   CSL_EMIF4F_SetTiming3Config (&sdram_tim3);    

   /*    hEmif->DDR_PHY_CTRL_1   = 0x0010010F; */
   ddrPHYReadLatency   = 0x0F;
   ddrPhyCtrl          = (0x08008);
   CSL_EMIF4F_SetPhyControl(ddrPhyCtrl,  ddrPHYReadLatency);

   /*    hEmif->ZQ_CONFIG        = 0x70073214; */
   zqcfg.zqRefInterval     = 0x3214;
   zqcfg.zqZQCLMult        = 3;
   zqcfg.zqZQCLInterval    = 1;
   zqcfg.zqSFEXITEn        = 1;
   zqcfg.zqDualCSEn        = 1;
   zqcfg.zqCS0En           = 1;
   zqcfg.zqCS1En           = 0;
   CSL_EMIF4F_SetOutputImpedanceConfig(&zqcfg);

   /*    hEmif->PWR_MGMT_CTRL    = 0x0; */
   pwrmgmtcfg.csTime           = 0;
   pwrmgmtcfg.srTime           = 0;
   pwrmgmtcfg.lpMode           = 0;
   pwrmgmtcfg.dpdEnable        = 0;
   pwrmgmtcfg.pdTime           = 0;
   CSL_EMIF4F_SetPowerMgmtConfig  (&pwrmgmtcfg); 

   /* New value with DYN_ODT disabled and SDRAM_DRIVE = RZQ/7 */
   /*    hEmif->SDRAM_CONFIG     = 0x63062A32; */
   CSL_EMIF4F_GetSDRAMConfig (&sdramcfg);
   sdramcfg.pageSize           = 2;
   sdramcfg.eBank              = 0;
   sdramcfg.iBank              = 3;
   sdramcfg.rowSize            = 4;
   sdramcfg.CASLatency         = 10;
   sdramcfg.narrowMode         = 0;
   sdramcfg.CASWriteLat        = 2;
   sdramcfg.SDRAMDrive         = 1;
   sdramcfg.disableDLL         = 0;
   sdramcfg.dynODT             = 0;
   sdramcfg.ddrDDQS            = 0;
   sdramcfg.ddrTerm            = 3;
   sdramcfg.iBankPos           = 0;
   sdramcfg.type               = 3;
   CSL_EMIF4F_SetSDRAMConfig (&sdramcfg);

   pll_delay(840336); /*Wait 600us for HW init to complete*/

   /* Refresh rate = (7.8*666MHz] */
   /*    hEmif->SDRAM_REF_CTRL   = 0x00001450;     */
   CSL_EMIF4F_SetRefreshRate(0x00001450);

   /***************** 4.2.1 Partial automatic leveling ************/
   /*    hEmif->RDWR_LVL_RMP_CTRL      =  0x80000000; */
   CSL_EMIF4F_SetLevelingRampControlInfo(1, 0, 0, 0, 0);

   /* Trigger full leveling - This ignores read DQS leveling result and uses ratio forced value */
   /*    hEmif->RDWR_LVL_CTRL          =  0x80000000; */
   CSL_EMIF4F_SetLevelingControlInfo(1, 0, 0, 0, 0);

   /************************************************************
     Wait for min 1048576 DDR clock cycles for leveling to complete 
     = 1048576 * 1.5ns = 1572864ns = 1.57ms.
     Actual time = ~10-15 ms 
    **************************************************************/
   pll_delay(4201680); //Wait 3ms for leveling to complete

   return (status);

}

#pragma FUNC_CANNOT_INLINE ( c66XX_getCoreFrequency );
unsigned int c66XX_getCoreFrequency(void)
{
   CSL_Status      status;
   PllcHwSetup     hwSetupRead;
   uint32_t        dsp_freq;

   status = CorePllcGetHwSetup (&hwSetupRead);

   if (status != CSL_SOK)
   {
      platform_printf("platform_get_info: Hardware setup parameters reading... Failed.\n");
      platform_printf("\tReason: Error setting in hardware validation."\
                             " [status = 0x%x].\n", status);
      platform_errno = PLATFORM_ERRNO_GENERIC;
      return (uint32_t)-1;
   }
   else
   {
      /* Compute the real dsp freq (*100) */
      dsp_freq = (hwSetupRead.pllM + 1)>> 1;
      dsp_freq = (dsp_freq * PLATFORM_BASE_CLK_RATE_MHZ)/(hwSetupRead.preDiv + 1);        
   }

   return (dsp_freq*1000000);

}

/* Registers to enable or disable memory ECC for L1, L2 and MSMC memories */
#define L1PEDSTAT	0x01846404
#define L1PEDCMD	0x01846408
#define L1PEDADDR	0x0184640C
#define L2EDSTAT	0x01846004
#define L2EDCMD		0x01846008
#define L2EDADDR	0x0184600C
#define L2EDCPEC	0x01846018
#define L2EDCNEC	0x0184601C
#define L2EDCEN		0x01846030
#define SMCERRAR	0x0BC00008
#define	SMCERRXR	0x0BC0000C
#define SMEDCC		0x0BC00010
#define SMCEA		0x0BC00014
#define SMSECC		0x0BC00018

/*Enable EDC on MSMC*/
/* Note: Once MSMC EDC is enabled, error correction stays enabled until
 * the MSMC is reset
 */

#pragma FUNC_CANNOT_INLINE ( MSMC_enableEDC );
static int MSMC_enableEDC (void)
{
   unsigned int status = 0;

   *(unsigned int *)(SMEDCC) &= 0x7FFFFFFF;  //Clear SEN(bit31)=0
   *(unsigned int *)(SMEDCC) |= 0x40000000;  //Set ECM(bit30)=1

   /* Check the status */
   status = *(unsigned int *)(SMEDCC);


   if ((status>>30)==0x1)
      /* Enabled */
      return 1;

   /* Failed */
   return 0;
}


/*Enable EDC on L1P*/
#pragma FUNC_CANNOT_INLINE ( enableL1PEDC );
static int enableL1PEDC (void)
{
   unsigned int status = 0;

   *(unsigned int *)(L1PEDCMD) = 0x1;  //Set EN(bit0)=1

   /* Check the status */
   status = *(unsigned int *)(L1PEDSTAT);

   if ((status<<28) == 0x10000000)
      /* Enabled */
      return 1;

   /* Failed */
   return 0;
}


/*Enable EDC on L2*/
#pragma FUNC_CANNOT_INLINE ( enableL2EDC );
static int enableL2EDC (void)
{
   unsigned int status = 0;

   *(unsigned int *)(L2EDCMD) = 0x1;

   /* Check the status */
   status = *(unsigned int *)(L2EDSTAT);

   if ((status<<28) == 0x10000000)
      /* Enabled */
      return 1;

   /* Failed */
   return 0;
}

/*Enable all bits in L2EDCEN*/
#pragma FUNC_CANNOT_INLINE ( enableEDCL2EDCEN );
static int enableEDCL2EDCEN (void)
{
   /* Set DL2CEN(bit0),PL2CEN(bit1),DL2SEN(bit2),PL2SEN(bit3),SDMAEN(bit4)=1 */
   *(unsigned int *)(L2EDCEN) |= 0x1F;
   return 1;
}

#pragma FUNC_CANNOT_INLINE ( platform_init );
static Platform_STATUS platform_init(platform_init_flags  * p_flags)
{
   CSL_Status              status;
   PllcHwSetup             pllc_hwSetup;
   PllcHwSetup             pllc_hwSetupRead;

   /*************************************************************************
    * This routine may be called before BIOS or the application has loaded.
    * Do not try and write debug statements from here.
    ***********************************************************************/

   if (p_flags == 0)
   {
      platform_errno = PLATFORM_ERRNO_INVALID_ARGUMENT;
      return ( (Platform_STATUS) Platform_EFAIL);
   }

   /* Start TCSL so its free running */
   CSL_chipWriteTSCL(0);

   /* PLLC module handle structure */
   if (p_flags->pll)
   {

      /* Set the Core PLL */
      /* Clear local data structures */
      memset(&pllc_hwSetup, 0, sizeof(PllcHwSetup));

      /* Setup PLLC hardware parameters */
      pllc_hwSetup.divEnable  = (CSL_BitMask32) (PLLC_DIVEN_PLLDIV2 |
                                                 PLLC_DIVEN_PLLDIV5 |
                                                 PLLC_DIVEN_PLLDIV8) ;
      /* Setup PLLC hardware parameters */
      pllc_hwSetup.pllM     = PLATFORM_PLL1_PLLM_val - 1;
      pllc_hwSetup.preDiv   = PLATFORM_PLL_PREDIV_val - 1;
      pllc_hwSetup.pllDiv2  = PLATFORM_PLLDIV2_val - 1;
      pllc_hwSetup.pllDiv5  = PLATFORM_PLLDIV5_val - 1;
      pllc_hwSetup.pllDiv8  = PLATFORM_PLLDIV8_val - 1;
      pllc_hwSetup.postDiv  = PLATFORM_PLL_POSTDIV_val -1;

      /* set Pll */
      status = CorePllcHwSetup (&pllc_hwSetup);

      if (status != CSL_SOK)
      {
         platform_errno = PLATFORM_ERRNO_PLL_SETUP;
         return ( (Platform_STATUS) Platform_EFAIL);
      }

      /* Read back */
      status = CorePllcGetHwSetup (&pllc_hwSetupRead);

      if (status != CSL_SOK)
      {
         platform_errno = PLATFORM_ERRNO_PLL_SETUP;
         return ( (Platform_STATUS) Platform_EFAIL);
      }

      /* Set the PA_SS PLL */
      status = SetPaPllConfig();
      if (status != CSL_SOK)
      {
         platform_errno = PLATFORM_ERRNO_PLL_SETUP;
         return ( (Platform_STATUS) Platform_EFAIL);
      }

      /* Set the DDR3 PLL */
      status = SetDDR3PllConfig();
      if (status != CSL_SOK)
      {
         platform_errno = PLATFORM_ERRNO_PLL_SETUP;
         return ( (Platform_STATUS) Platform_EFAIL);
      }
   }

   /* Initialize DDR */
   if (p_flags->ddr)
   {

      xmc_setup();

      status = DDR3Init();

      if (status != CSL_SOK)
      {
         platform_errno = PLATFORM_ERRNO_GENERIC;
         return ( (Platform_STATUS) Platform_EFAIL);
      }
   }

   PowerUpDomains();

   /* Enable Error Correction for memory */
   if (p_flags->ecc)
   {
      enableL1PEDC();
      enableEDCL2EDCEN();
      enableL2EDC();
      MSMC_enableEDC();
   }

   return Platform_EOK;
}

/* These are defined in the linker command file */
extern far uint32_t cache_l1p_size;
extern far uint32_t cache_l1d_size;
extern far uint32_t cache_l2_size;

void c6678_platform_init(void)
{
   int i;

   platform_init_flags  init_flags;

   uint32_t l1p_size = (uint32_t)&cache_l1p_size;
   uint32_t l1d_size = (uint32_t)&cache_l1d_size;
   uint32_t l2_size = (uint32_t)&cache_l2_size; 

   init_flags.pll = 1;
   init_flags.ddr = 1;
   init_flags.ecc = 1;

   if (platform_init(&init_flags) != Platform_EOK) {
     platform_printf("Platform failed to initialize, errno = 0x%x \n", platform_errno);
   }

   for (i=0x80;i<0xA0;i++)
      CACHE_enableCaching((Uint8)i);
#if 1//defined(DEBUG)
   platform_printf("Core frequency %d\n",c66XX_getCoreFrequency());
   platform_printf("__cache_l1p_size %d\n",l1p_size);
   platform_printf("__cache_l1d_size %d\n",l1d_size);
   platform_printf("__cache_l2_size %d\n",l2_size);
#endif
   switch(l1p_size)
   {
      case 0: CACHE_setL1PSize(CACHE_L1_0KCACHE); break;
      case 1: CACHE_setL1PSize(CACHE_L1_4KCACHE); break;
      case 2: CACHE_setL1PSize(CACHE_L1_8KCACHE); break;
      case 3: CACHE_setL1PSize(CACHE_L1_16KCACHE); break;
      case 4: CACHE_setL1PSize(CACHE_L1_32KCACHE); break;
   }

   switch(l1d_size)
   {
      case 0: CACHE_setL1DSize(CACHE_L1_0KCACHE); break;
      case 1: CACHE_setL1DSize(CACHE_L1_4KCACHE); break;
      case 2: CACHE_setL1DSize(CACHE_L1_8KCACHE); break;
      case 3: CACHE_setL1DSize(CACHE_L1_16KCACHE); break;
      case 4: CACHE_setL1DSize(CACHE_L1_32KCACHE); break;
   }

   switch(l2_size)
   {
      case 0: CACHE_setL2Size(CACHE_0KCACHE); break;
      case 1: CACHE_setL2Size(CACHE_32KCACHE); break;
      case 2: CACHE_setL2Size(CACHE_64KCACHE); break;
      case 3: CACHE_setL2Size(CACHE_128KCACHE); break;
      case 4: CACHE_setL2Size(CACHE_256KCACHE); break;
      case 5: CACHE_setL2Size(CACHE_512KCACHE); break;
   }

   if (l1p_size)
      CACHE_invAllL1p(CACHE_WAIT);
   if (l1d_size)
      CACHE_wbInvAllL1d(CACHE_WAIT);
   if (l2_size)
      CACHE_wbInvAllL2(CACHE_WAIT);

   /* Writing to TSCL starts the timer running */
   TSCL = 0;
   return;
}

#if 0
static inline void GETMYTIME(ALTIMETYPE * x)
{
   /* Use Time Stamp Counters */
   /* From c6x.h              */
   /*  extern __cregister volatile unsigned int TSCL; */
   /* extern __cregister volatile unsigned int TSCH; */
   /* TSCL needs to be read first, doing so will latch TSCH */ 
   unsigned int lo, hi;
   lo = TSCL;  
   hi = TSCH;
   *x = _itoll(hi, lo);
}
#endif

