/*******************************************************************************
*  BOSE CORPORATION
*  COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
*  This program may not be reproduced, in whole or in part in any
*  form or any means whatsoever without the written permission of:
*     BOSE CORPORATION
*     The Mountain
*     Framingham, MA 01701-9168
********************************************************************************
*  Title:    pooliirSplitProcessAsm.asm
*
*  $Header$
*
*  Description:  Process and Reset functions for a bdsppooliir module.
*                Implements a multi-channel direct form type II IIR filter. 
*                Based on Bruce Levens's jkiir implementation.
*  
*  References:   \BDSP_Toolbox\bdspblocks\bdsppooliir.m and 
*                \BDSP_Toolbox\src\sharc\jkiirProcessAsm.asm.
*               Testing can be initiated by test_pooliirSplitProcess.c
*               This asm implementation is the hand optimized version
*                of the C functions in pooliirProcess.c. 
********************************************************************************/

/**** assembler directives *****************************************************/

/**** Include Files ************************************************************/

#include "asm_sprt.h"         /* useful macros                     */


/**** External Variable References  ********************************************/
        
//.import "pooliir.h";


/***  Module Macros  ***********************************************************/

#define off(x) offsetof(pooliir_t,x)


/***  Module Types  ***********************************************************/


/***  Module Function Prototypes  *********************************************/
.segment /pm seg_pmco;
.global _pooliirSplitProcess;


/**********************************************************************************
                             Main Entry Function:
 
Function Name : _pooliirSplitProcess
C Callable : Yes, leaf function

C ProtoType :void pooliirSplitProcess(float *out, float *in, uint32 frame_size,
	                              uint32 ncha, uint32 *num_stages,
	                              float *coeffs, float *states);

Arguments:   r4 = block_t * - DM Memory Pointer to pooliir_t  (overlaid on block_t)
             r8 = float **  - ppx   -  (BDSP parameter passing)
                              This is an array of pointers (DM Memory).  
                              First num_in points to input channels in DM Memory,
                              next  num_out points to output channels in DM Memory
                             
             r12 = int      - frameSize (Number of samples to process) in DM memory
                              must be an even number
                       
Outputs:     Replaces the output sample blocks pointed to by ppx.

Return Value:None
                             
Registers Used: 
           Core:   r0, r1, r2, r3, r4, r5, r6, r8, r9, r10, r12, r13, r14
           DAG :   i1, i2, i3, i4 ,i5, i10, i12, m4, m12
           Other: None
           
Max Clock Cycles : TBD

Processor Type :ADI SHARC 21365, 21479

Key Assumptions : Coeffs in PM space and ordered as: gain, a2, a1, b2, b1.....
                  C Calling Convention
                  FrameSize is an even number
                             
Special Coding Tricks : 

Mathematical Equations:

     Each second order section implements the basic difference equation:

     y[n] = b0*x[n]+b1*x[n-1]+b2*x[n-2]- a1*y[n-1]-a2*y[n-2]      (1)

     Without loss generality we can assume that all b0 = 1, since the gains
     of all sections can be cascaded into a single overall gain (which will
     later be applied in the first stage).
     We can rewrite the equation above as 3 coupled difference equations

     y[n]  = w1[n-1] + b0*x[n]                                    (2a)	
     w1[n] = b1*x[n] - a1*y[n] + w2[n-1]                          (2b)
     w2[n] = b2*x[n] - a2*y[n]                                    (2c)

     This represents a transposed form II topology of an IIR filter. 
     This form minimizes the amount state and also keeps the state
     reasonably bounded (provided that the input and output are bounded).

        |\
  x[n] -| >->+-------- + ------->+--- .. -->+-------- + ------->+-- y[n]
        |/  |          ^         |         |          ^         |
        k   |          |         |         |          |         |
            |         wn1        |         |         wn1        |
            |         ---        |         |         ---        |
            |        |   |       |         |        |   |       |
            |   a11   ---  b11   |         |   a1M   ---  b1M   |
            v   |\     |     /|  v         v   |\     |     /|  v
            +---| >----+----< |--+         +---| >----+----< |--+
            |   |/     ^     \|  |         |   |/     ^     \|  |
            |          |         |         |          |         |
            |         wn2        |         |         wn2        |
            |         ---        |         |         ---        |
            |        |   |       |         |        |   |       |
            |   a21   ---   b21  |         |   a2M   ---   b2M  |
            v   |\     |     /|  v         v   |\     |     /|  v
            +---| >----+----< |--+         +---| >----+----< |--+
                |/           \|                |/           \|
  
*************************************************************************/

/**********************************************************************
 * Register allocation
 ************************************************************************/
#define S             i4   /* pointer to pooliir_t and block_t structs */
                           /* pooliir_t and block_t overlay            */  
                           /* scratch */        
#define SRC           i1
#define DST           i2
#define STAGES        i3
#define STATE         i5
#define COEFF         i10
#define XBASE         i12  /* scratch */

#define A1_COEFF      f0   /* scratch */
#define A2_COEFF      f1   /* scratch */
#define B1_COEFF      f2   /* scratch */
#define B2_COEFF      f3   

#define Y             f4   /* scratch */
#define X             f5
#define GAIN          f6
#define X_NEXT        f14
#define P             X_NEXT /* can be shared*/  



#define Q             f10   
#define W1            f9
#define W2            f13 
   
#define NSTAGES       r8   /* scratch */

#define SAMPLECOUNTER r12  /* scratch */ 

#define NCHA          m12  /* scratch */
#define CHA           m4   /* scratch */

//macro to save registers used by this routine (non scratch) onto stack 
#define save_reg_piir   puts=r3; \
                        puts=r5; \
                        puts= r6; \
                        puts=r9; \
                        puts=r10;\
                        puts=r13;\
                        puts= r14;\
                        r0=i1; \
                        puts=r0; \
                        r0=i2; \
                        puts=r0; \
                        r0=i3; \
                        puts=r0; \
                        r0=i5; \
                        puts=r0; \
                        r0= i10;\
                        puts=r0
                                                                         
//macro to restore registers used by this routine and reset stack pointer
#define restore_reg_piir i10 =gets(1);\
                        i5=gets(2);\
                         i3  =gets(3);\
                         i2  =gets(4);\
                         i1  =gets(5);\
                         r14 =gets(6);\
                         r13 =gets(7);\
                         r10 =gets(8);\
                         r9  =gets(9);\
                         r6  =gets(10);\
                        r5 =gets(11);\
                        r3 =gets(12);\
                        alter(12)
                        


/**********************************************************************
 *  PROCESS pooliir
 **********************************************************************/
_pooliirSplitProcess:
  /* get some regs */
  save_reg_piir;  

  SAMPLECOUNTER = SAMPLECOUNTER-1, S=r4;/* S: pointer to pooliir structure */
  /* set up the channel loop */
  XBASE  = r8; 
  CHA    = 0;
  STAGES = dm(2,i6);
  STATE  = dm(4,i6);



/* get coefficient pointer */
  COEFF  = dm(3,i6);
  
  /*********************************************************************** Outer Loop over all channels
   **********************************************************************/  
  NCHA = dm(1,i6);
  DST = r4;
  SRC = r8;

  LCNTR = NCHA, do channel_loop until lce;
    XBASE = DST; /* save start of output buffer, needed later */
    
    /***********************************************************************  Do the first biquad and apply the gain while were at it (saves
     *  1op per sample over a read/mpy/write loop 
     *  This is simple enough, we simply inject another multiply statement
     *  after we have read the new input
     **********************************************************************/
    W1 = dm(STATE,m6), GAIN     = pm(COEFF,m14);
    W2 = dm(STATE,m7), A2_COEFF = pm(COEFF,m14); /* step - 1 */
    X  =dm(SRC,m6),   A1_COEFF = pm(COEFF,m14);
    X  = X*GAIN,       B2_COEFF = pm(COEFF,m14);
    Y  = W1+X,         B1_COEFF =pm(COEFF,m14);
    W1 = B1_COEFF*X;

    /* main loop: 5 ops/sample */
    lcntr=SAMPLECOUNTER, do filter_loop1 until lce;
      P      = A1_COEFF*Y, W1 = W1+W2,     dm(DST,m6)= Y;
      Q      = B2_COEFF*X, W1 = W1-P,      X_NEXT     = dm(SRC,m6); /* w1 is up to date */
                                                                     /* next x[n] sample */
      X_NEXT = X_NEXT*GAIN;
      W2     = A2_COEFF*Y, Y  = W1+X_NEXT, X=X_NEXT;                 /* next y[n] */
filter_loop1:  W1     = B1_COEFF*X, W2 = Q-W2;                       /* w2 is up to date */

    /* drain the pipe to update the state and save it */
    P            = A1_COEFF*Y, W1           = W1+W2, dm(DST,m6) = Y; 
    Q            = B2_COEFF*X, W1           = W1-P;
    W2           = A2_COEFF*Y, dm(STATE,m6) = W1;
    W2           = Q-W2;                                   /* w2 is up to date */
    dm(STATE,m6) = W2;                                     /* save state */

    S = SRC;        /* save next input ptr */

    /**********************************************************************
    * 4 multiply loop over the remaining stages, in-place.
    **********************************************************************/
    NSTAGES =dm(CHA, STAGES); 
    NSTAGES = NSTAGES -1; 
    if EQ jump(pc,next_channel_prep);

    lcntr=NSTAGES, do stage_loop until lce;
      SRC =XBASE;
      W1  =dm(STATE,m6), A2_COEFF =pm(COEFF,m14);
      W2  =dm(STATE,m7) , A1_COEFF =pm(COEFF,m14); /* step - 1 */
      X   =dm(SRC,m5) ,   B2_COEFF =pm(COEFF,m14);
      Y   = W1+X,         B1_COEFF = pm(COEFF,m14);
      W1  = B1_COEFF*X; 
      
      /* main loop: 4 ops/sample */
  
             lcntr=SAMPLECOUNTER, do filter_loop until lce;
        P  = A1_COEFF*Y, W1 = W1+W2,dm(SRC,m6)= Y; 
        Q  = B2_COEFF*X, W1 = W1-P,      X_NEXT     =dm(SRC,m5); /* w1 is up to date */
                                                                  /* read next x[n] sample */
        W2 = A2_COEFF*Y, Y  = W1+X_NEXT, X=X_NEXT;                /* next y[n] */
                                                                  /* move x[n] to a reg. in the range r4-r7 */
filter_loop: W1 = B1_COEFF*X, W2 = Q-W2;                          /* w2 is up to date */

      /* drain the pipe to update the state and save it */
      P  = A1_COEFF*Y, W1 = W1+W2, dm(SRC,m6)= Y; 
      Q  = B2_COEFF*X, W1 = W1-P;
      W2 = A2_COEFF*Y, dm(STATE,m6)= W1;
      W2 = Q-W2;           /* w2 is up to date */
stage_loop: dm(STATE,m6) = W2;   /* save state */
  
next_channel_prep:  
    /* update all stuff for the next channel */
    r0  = CHA;
    r0  = r0+1;
    CHA = r0;
channel_loop: SRC = S;

     restore_reg_piir;   
leaf_exit;   /* leaf_exit macro in file asm_sprt restores stack, frame pointers and returns to calling function */

_pooliirSplitProcess.end:

.endseg;



