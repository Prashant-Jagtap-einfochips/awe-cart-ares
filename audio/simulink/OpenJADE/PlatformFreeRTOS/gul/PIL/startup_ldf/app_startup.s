/*
** ADSP-21569 startup code generated on Jun 07, 2019 at 14:44:27.
*/
/*
** Copyright (C) 2000-2018 Analog Devices Inc., All Rights Reserved.
**
** This file is generated automatically based upon the options selected
** in the System Configuration utility. Changes to the Startup Code configuration
** should be made by modifying the appropriate options rather than editing
** this file. To access the System Configuration utility, double-click the
** system.svc file from a navigation view.
**
** Custom additions can be inserted within the user-modifiable sections,
** these are bounded by comments that start with "$VDSG". Only changes
** placed within these sections are preserved when this file is re-generated.
**
** Product      : CrossCore Embedded Studio
** Tool Version : 6.2.0.1
*/

.FILE_ATTR libGroup="startup";
.FILE_ATTR libName="libc";

.FILE_ATTR libFunc="___lib_prog_term";
.FILE_ATTR FuncName="___lib_prog_term";
.FILE_ATTR libFunc="start";
.FILE_ATTR FuncName="start";
.FILE_ATTR prefersMem="any";
.FILE_ATTR prefersMemNum="50";

#include <sys/anomaly_macros_rtl.h> // defines silicon anomaly macros
#include <interrupt.h>              // defines interrupt support
#include <platform_include.h>       // defines MMR macros
#include <adi_osal.h>               // OSAL support
#include <sys/fatal_error_code.h>   // defines fatal error support

/* Suppress the assemblers informational message regarding the automatic
** call sequence change it does when building -swc.
*/
.MESSAGE/SUPPRESS 2536;


.GLOBAL start;

.SECTION/CODE/DOUBLEANY seg_pmco;

start:

      /* Disable the BTB. */
      R0 = DM(REG_SHBTB0_CFG);              /* Get current BTB configuration. */
      R0 = BSET R0 BY BITP_SHBTB_CFG_DIS;   /* Disable the BTB                */
      DM(REG_SHBTB0_CFG) = R0;              /* Write back to BTB_CFG MMR.     */
      /* Wait 11 cycles for the disabling of the BTB to take effect. */
      NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;

      /* Enable instruction, data and system transfer parity checking for
      ** parts and revisions that have all required support. The ADSP-SC58x
      ** and ADSP-2158x revisions 0.0 and 0.1 system reset does not work and
      ** this is required so we don't enable parity for these.
      */
#if !defined(__ADSP2156XFPGA__) && \
    (!defined(__ADSPSC589_FAMILY__) || \
     (!defined(__SILICON_REVISION__) || \
      ((__SILICON_REVISION__ >= 0x0100) && (__SILICON_REVISION__ != 0xFFFF))))
      BIT SET MODE1 (BITM_REGF_MODE1_IPERREN | BITM_REGF_MODE1_DPERREN |
                     BITM_REGF_MODE1_SPERREN );
      /* Wait 11 cycles for the change to take effect. */
      NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;
      #define ENABLED_PARITY_ERROR_CHECKING
#else
      /* Explicitly disable parity error checking when we don't enable it
      ** for the application. This may be necessary for ADSP-215xx (non-ARM)
      ** parts as the SHARC+ boot can enable parity error checking.
      */
      BIT CLR MODE1 (BITM_REGF_MODE1_IPERREN | BITM_REGF_MODE1_DPERREN |
                     BITM_REGF_MODE1_SPERREN );
      /* Wait 11 cycles for the change to take effect. */
      NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;
#endif

      /* Zero all the BTB memory locations to initialize the parity bits. */
      I0 = 0x31000;
      L0 = 0;
      M0 = 1;
      R0 = 0;
      LCNTR = 0x400, DO (PC,.BTB_init_loop) UNTIL LCE;
.BTB_init_loop:
         DM(I0, M0) = R0;

      /* Also zero the L1 cache memory locations... **/

      /* ... Icache tag */
      I0 = 0x3E400;
      LCNTR = 0x400, DO (PC,.icache_tag_loop) UNTIL LCE;
.icache_tag_loop:
         DM(I0, M0) = R0;

      /* ... then dcache (dmcache + pmcache) tag */
      I0 = 0x3C000;
      LCNTR = 0x1000, DO (PC,.dcache_tag_loop) UNTIL LCE;
.dcache_tag_loop:
        DM(I0, M0) = R0;

      /* ... then dmcache LRU/DIRTY */
      I0 = 0x3D100;
      LCNTR = 0x100, DO (PC,.dmcache_dirty_loop) UNTIL LCE;
.dmcache_dirty_loop:
        DM(I0, M0) = R0;

      /* ... then pmcache LRU/DIRTY */
      I0 = 0x3D300;
      LCNTR = 0x100, DO (PC,.pmcache_dirty_loop) UNTIL LCE;
.pmcache_dirty_loop:
        DM(I0, M0) = R0;

      /* ... then icache LRU/DIRTY */
      I0 = 0x3d500;
      LCNTR = 0x80, DO (PC,.icache_dirty_loop) UNTIL LCE;
.icache_dirty_loop:
        DM(I0, M0) = R0;

      R0 = DM(REG_SHBTB0_CFG);              /* Get current BTB configuration. */
      R0 = BCLR R0 BY BITP_SHBTB_CFG_DIS;   /* Enable the BTB                 */
      R0 = BSET R0 BY BITP_SHBTB_CFG_INVAL; /* Invalidate the BTB             */
      /* Turn off software return prediction in the BTB, because we can't
      ** guarantee i12 has a valid code address, and a speculative access
      ** to an invalid address can cause a hardware error.
      */
      R0 = BSET R0 BY BITP_SHBTB_CFG_SRETDIS; /* Software return disable.     */
      DM(REG_SHBTB0_CFG) = R0;              /* Write back to BTB_CFG MMR.     */

      /* Wait 11 cycles for the invalidation of the BTB to take effect. */
      NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;

     /* There might be entries on the loop, PC and status stacks. Clear them all. 
     ** First, clear the loop stack.
     */
     BIT TST STKY BITM_REGF_STKYX_LSEM;
.pop_loop_stack:
     IF TF JUMP (PC, .loop_stack_empty);
     JUMP (PC, .pop_loop_stack) (DB);
       POP LOOP;
       BIT TST STKY BITM_REGF_STKYX_LSEM;
.loop_stack_empty:

     /* Then, clear the status stack. */
     BIT TST STKY BITM_REGF_STKYX_SSEM;
.pop_status_stack:
     IF TF JUMP (PC, .status_stack_empty);
     JUMP (PC, .pop_status_stack) (DB);
       POP STS;
       BIT TST STKY BITM_REGF_STKYX_SSEM;
.status_stack_empty:

     /* Then clear the PC stack. PC stack cannot be popped in a delay slot. */
.pop_pc_stack:
     BIT TST STKY BITM_REGF_STKYX_PCEM;
     IF TF JUMP (PC, .pc_stack_empty);
     POP PCSTK;
     JUMP (PC, .pop_pc_stack);
.pc_stack_empty:

      /* Set the vector register to 'start' so that if the processor
      ** is reset the processor bypasses the boot ROM code.
      */
      R0 = start;
      DM(REG_RCU0_SVECT0) = R0;

      /*
      ** Paint heap and stack
      */
	.extern heapstack_table.;
	.type heapstack_table.,STT_OBJECT;

	  M0=1;
	  I4=DM(heapstack_table.+0);
	  R2=DM(heapstack_table.+4);
	  R2=lshift R2 by -2;
	  
	  lcntr=R2, do (pc,.paintheap) until lce;
.paintheap:
	  DM(I4,M0)=0xDEADBEEF;
	
	  I4=DM(heapstack_table.+8);
	  R2=DM(heapstack_table.+12);
	  R2=lshift R2 by -2;
	
	  lcntr=R2, do (pc,.paintstack) until lce;
.paintstack:
	  DM(I4,M0)=0xDEADBEEF;

      /*
      ** Initializes the processor, memory, C runtime and heaps.
      */
      .EXTERN ___lib_setup_c;
      CALL ___lib_setup_c;

      /*
      ** Enable the ILOPI interrupt to support illegal opcode detection.
      ** The interrupt vector code for Illegal Opcode detection
      ** jumps directly to stub handler _adi_ilop_detected.
      */
      .EXTERN adi_rtl_activate_dispatched_handler.;
      R4 = ADI_CID_ILOPI;
      CJUMP adi_rtl_activate_dispatched_handler. (DB);
         DM(I7, M7) = R2;
         DM(I7, M7) = PC;

#if defined(ENABLED_PARITY_ERROR_CHECKING)
      /*
      ** Enable the PARI interrupt to support L1 parity error detection.
      ** The interrupt vector code for this interrupt jumps directly
      ** to stub handler _adi_parity_error_detected.
      */
      .EXTERN adi_rtl_activate_dispatched_handler.;
      R4 = ADI_CID_PARI;
      CJUMP adi_rtl_activate_dispatched_handler. (DB);
         DM(I7, M7) = R2;
         DM(I7, M7) = PC;
#endif

      /*
      ** Call the OSAL init function.
      */
      .EXTERN adi_osal_Init.;    // ADI_OSAL_STATUS adi_osal_Init(void);
      CJUMP adi_osal_Init. (DB);
         DM(I7, M7) = R2;
         DM(I7, M7) = PC;

      R1 = E_ADI_OSAL_SUCCESS;
      COMPU(R0, R1);
      IF NE JUMP .osal_Init_failed;

      R4 = R4 - R4,              // argc = 0
         R8 = M5;                // argv = NULL

      .EXTERN main.;
      JUMP main. (DB);           // call main()
         DM(I7, M7) = 0;         // NULL FP to terminate call stack unwinding
         DM(I7, M7) = PC;

      .GLOBAL ___lib_prog_term;
___lib_prog_term:
      IDLE;
      JUMP ___lib_prog_term;     // Stay put.
.___lib_prog_term.end:

      /*
      ** The call to _adi_osal_Init returned an error so call _adi_fatal_error.
      */
      .EXTERN adi_fatal_error.;
.osal_Init_failed:
      R12 = R0;                  // pass adi_osal_Init result value
      JUMP adi_fatal_error. (DB); // doesn't return
         R4 = _AFE_G_LibraryError;
         R8 = _AFE_S_adi_osal_Init_failure;
.osal_Init_failed.end:

.start.end:
