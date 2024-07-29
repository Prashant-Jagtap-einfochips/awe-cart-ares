/*****************************************************************************************

  BOSE CORPORATION
  COPYRIGHT 2006 BOSE CORPORATION ALL RIGHTS RESERVED
  This program may not be reproduced, in whole or in part in any
  form or any means whatsoever without the written permission of:

      BOSE CORPORATION
      The Mountain
      Framingham, MA 01701-9168

********************************************************************************
*  Title:       FirEvenSplitProcessASM.asm
*  Revision:    $Header$
*  Main Entry Function : _FirEvenSplitProcessASM
*
*  Description: 
*     Optimized SHARC assembly version of FirEvenSplitProcess.c.
*     Performs FIR filtering on one block of data for each of
*     a set of input channels
*     See bdspFirEvenSplit.m for algorithm details.
*  
*
*  References:  Based on FirEvenSplitProcess.c and bdsdFirEvenSplit.m
*               Testing can be performed by TestBed_FirEvenSplitProcess.c
*
********************************************************************************
*  $Log$

*****************************************************************************************/

//------------------------------------------------------------------------------
//assembler directives 
//-------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//includes
//-------------------------------------------------------------------------------

// Define for CCES to include old VDSP register names
#define _INCLUDE_LEGACY_SYSREG_NAMES 1
   
#include <platform_include.h>   // processor related definition file
#include "asm_sprt.h"    //useful macros for doinng Mixed Programming

/***  External Variable References  *******************************************/

// .IMPORT "FirEvenSplit.h";



//--------------------------------------------------------------------------------
//Export Functions
//--------------------------------------------------------------------------------
.global _FirEvenSplitProcess;  

//--------------------------------------------------------------------------------
//Import Functions
//--------------------------------------------------------------------------------



//--------------------------------------------------------------------------------
//Local Defines & Macros
//--------------------------------------------------------------------------------

//the following are offests from the base of the BDSP structure - FirEvenySplit_t 
#define numChannels    dm(1,i6)
#define state          dm(2,i6)
#define LEN              dm(3,i6) // OFFSETOF(FirEvenSplit_t,len)
#define stateIndex     OFFSETOF(FirEvenSplit_t,stateIndex)
#define pStateIndex     dm(4,i6) //OFFSETOF(FirEvenSplit_t,stateIndex)
#define coeffs         dm(6,i6) // OFFSETOF(FirEvenSplit_t,coeffs)
#define activeCoeffSet dm(5,i6) //OFFSETOF(FirEvenSplit_t,activeCoeffSet)


//macro to save registers used by this routine (non scratch) onto stack 
#define save_reg_FE     r0=m8;\
                        puts=r0;\
                        r0=i14;\
                        puts=r0;\
                        r0=b14;\
                        puts=r0;\
                        r0=m11;\
                        puts=r0;\
                        puts=r10;\
                        r0=m10;\
                        puts=r0;\
                        r0=i11;\
                        puts=r0;\
                        r0=m3;\
                        puts=r0;\
                        puts=r3;\
                        puts=r5;\
                        puts=r9;\
                        puts=r14;\
                        puts=r15;\
                        r0=i0;\
                        puts=r0;\
                        r0=i2;\
                        puts=r0;\
                        r0=i3;\
                        puts=r0;\
                        r0=i5;\
                        puts=r0;\
                        r0=i9;\
                        puts=r0;\
                        r0=m9;\
                        puts=r0;\
                        r0=b13;\
                        puts=r0

                                                                         
//macro to restore registers used by this routine and reset stack pointer
#define restore_reg_FE  b13=gets(1);\
                        m9=gets(2);\
                        i9=gets(3);\
                        i5=gets(4);\
                        i3=gets(5);\
                        i2=gets(6);\
                        i0 =gets(7);\
                        r15 =gets(8);\
                        r14 =gets(9);\
                        r9 =gets(10);\
                        r5 =gets(11);\
                        r3 =gets(12);\
                        m3=gets(13);\
                        i11=gets(14);\
                        m10=gets(15);\
                        r10=gets(16);\
                        m11=gets(17);\
                        b14=gets(18);\
                        i14=gets(19);\
                        m8=gets(20);\
                        alter(20)

//-------------------------------------------------------------------------------
//Local Variable (Not supported for BDSP modules) 
//--------------------------------------------------------------------------------



.section/pm seg_pmda;




//--------------------------------------------------------------------------------
//  Functions
//--------------------------------------------------------------------------------

.section/pm/DOUBLE32 seg_pmco;

/**********************************************************************************
                             Main Entry Function:
 
Function Name : _FirEvenSplitProcessASM

C Callable : Yes, leaf function

C ProtoType :void FirEvenSplitProcess(float *coeffs, float *x0[], int frame_size,
    int num_channels, float *state, int filter_length, int *stateIndex,
    int *activeCoeffSet)

Arguemts:   R4=block_t * -DM Memory Pointer to FirEvenSplit_t  (overlaid on block_t)
            R8=float **  -  (BDSP parameter passing)
                             This is an array of pointers (DM Memory).  
                             First numChannels point to input channels in DM Memory
                             next  numChannels point to output channels in DM Memory
            R12= int,  block size (Number of samples to process)
            
   
Registers Used: 
           Core:   r0,r1,r2,r3,r4,r5,r8,r9,r10,r12,r14,r15
           DAG :   i0,i2,i3,i4,i5,i9,i11,i12,i13,i14 
                   m4,m3,m9,m11,m12,m8
                   l4,l13,l14,b4,b13,b14
           Other: mode1: set/clear circular buffering and SIMD mode
                  (at exit mode1 identical to mode1 at entry so don't 
                   pust on stack)
           
Max Clock Cycles : TBD

Processor Type :ADI SHARC 21365;
                     
Key Assumptions : States arrays in PM space
                  Coeffs arrays in DM space
                  C Calling Convention
                  L = # filter taps is EVEN
                  (L= length of states/coeffs arrays)
                  
                             
Special Coding Tricks : Use 2 SIMD Pre-loop instructions and 2 SIMD
                        Post-loop instructions.  This allow us to:
                         1.)Loop on an instruction with max parallel operations
                         2.)End SIMD computation with pointers positioned 
                            correctly for next sample.
                            
                            
Special Coding Tricks : 
Special tricks were used to meet 2 optimization goals:
1.)Execute entire sample loop in SIMD mode. Main benefit of this goal is that we 
   get rid of the 4 instructions needed to enable/disable SIMD each sample.
2.)Efficiently solve the SIMD circular buffer read problem. Previous method for
   solving this problem was to copy states[0] to states[polyLen] every sample.
   Goal here is to eliminate this copy.
Tricks used to meet 2 goals above:
1.)States array contains 2 extra dummy locations at end. i.e. states array has
   size = L+2, (indexed as states[0] to states[L+1]  states array is accessed as 
   a circular buffer of length L. states[L] is kept filled such that SIMD reads 
   at circular buffer boundary give correct data.  states[L+1] is simply used 
   as a persistent variable between calls to FirEvenSplitProcess.  states[L+1] holds
   the last input data sample of the previous block.  We need to read this value
   once at start of block to correctly do 1st SIMD write to states. 
2.)States array is accessed with 2 different pointers: a read pointer and a 
   write pointer. i13 used for reading and i14 used for writing.                        
3.)Writes to states array are all done in SIMD. Each write adds ONE new value 
   only. Other half of SIMD write re-writes a previously existing entry.
4.)Relation of read and write pointers is fixed such that the dummy location
   at end of states array always has the data needed each time a SIMD read 
   crosses the circular buffer boundary.
5.)States buffer read pointer i13 is incremented by 3 instead of 2 after 
   last read for filterloop. This positions read pointer correctly for next 
   sample's filter loop where a new sample has been written to states array.
   This is implemented by executing code for last filter loop iteration
   outsided of filter loop and using a different increment. 
Other tricks used
1.)Operations for last iteration of sample loop are unrolled and done
   outside of loop. Thus number of sample loop iterations becomes n-1.
   Reason: allows us to fill parallel move slots in later instructions
   with moves needed for next sample.  These moves are done prior to 
   sample loop for first sample and are not needed when last sample 
   is processed.
2.)2 SIMD pre-loop instructions for filter loop.  Allows us to loop
   on an instruction with all parallel move/compute slots filled.
                            
                            
                            

Mathematical Equations:   see FirEvenSplitProcess.c, bdspFirEvenSplit.m       
***********************************************************************************/
_FirEvenSplitProcess.start:
_FirEvenSplitProcess:

   leaf_entry;                          //call leaf entry macro from asm_sprt
   
   //At entry we have:
   //r4  = pointer to base of FirEvenSplit_t
   //r8  = pointer to x0 = an array of pointers
   //      which point to the input and output
   //      data blocks for each channel
   //r12 = number of samples per data block 

   save_reg_FE;                        //save context on stack 
   
    //    bit set Mode1 CIRCULAR_BUF_EN_BIT;                //enable circular buffering
   bit set Mode1 CBUFEN;                //enable circular buffering 

   m10=0;                               //DAG2 0 increment
   m0=0;                                //DAG1 0 increment
                                        //necessary to set these???
   
   m3=2;                                //DAG1 +2 increment
   m12=2;                               //DAG2 +2 increment
      
    // i2=r4;         				        //i2-->base of FirEvenSplit_t;
   i2=r4;         				        //i2-->output buffer;
   m4=numChannels;               //m4 = numChannels
   r0=LEN;                         //r0  = L, (# filter taps)
   r15=2;
   r10=r0+r15,m11=r0;                   //r10 = L + 2 = increment from states array of channel i
                                        //to states array of channel i+1, (includes the 2 dummy locations
                                        //at end of each channel's states array)
                                        //m11 = L, (# filter taps)
   
   r15=r0;                              //r15 = L, (# filter taps)
   r15=lshift r15 by -1;                //r15 = L/2 - 2 for SIMD computation
   r15=r15-1,i3=r8;                     //i3-->x0[0]
   r15=r15-1,l4=r0;                     //l4 = L = circular buffer length for coeffs                                                          
   r15=r15-1,l13=r0;                    //r15= L/2 -3
                                        //l13 = L = circular buffer length for states
   l14=r0;                              //l14 = L = circular buffer length for states
   
    s9 = 0;                              // initialize s9 to avoid floating-point invalid operation for SIMD
   i5=pStateIndex;
   m9=dm(m0,i5);                //m9=starting point in states array for 
                                        //all channels, (can't assume starting point
                                        //is first element of the channels states array,
                                        //BUT starting point is same for all channels).                                       
   
   i0=activeCoeffSet;            //i0-->coeff set index for channel 0  
   r1=coeffs;                    //r1 = start addr of coefficient table
   
   m8=3;                               //increment for i13 states pointer used
                                       //in filter post loop
   
   
   r12=r12-1,r4=state;           //r12 at input = number of samples for this block. 
                                        //We use r12 as numIterations for sample loop. 
                                        //We decrement by one since we do last sample outside
                                        //of sample loop and dont do write to states
                                        //array
                                        //r4 = start states array for channel 0
   //******
   //r8=1;

    // ** NOTE: The two lines below are for Simulink-style (float *) I/O vs (float **)
    i9=i2;
    i5=i3;  
   lcntr=m4, do Channel_Loop until lce;  //Loop over all channels, (channel=0-->numChannels-1)

      // ** NOTE: The three lines below are for BDSP-style (float **) I/O vs (float *)
      //i9=dm(m0,i2);                      //i9-->out[channel]
      //r3=dm(i3,1);                       //i5--->in[channel]
      //i5=r3;                             //i3--->pointer to in[channel+1] for next 
                                         //loop iteration
        
       
      b13=r4;                            //b13=i13=start of states array for channel
                                         //i13 functions as read pointer for states array                                  
      b14=r4;                            //b14=i14=start of states array for channel
                                         //i14 functions as write pointer for states array
                                         
      i12=r4;                            //i12-->start of states array for channel
      modify(i12,m11);                   //i12-->states[L]
      modify(i12,1);                     //i12-->states[L+1]                                   
      
      //r5=pm(i12,m10);                    //r5 = last sample of previous data buffer, 
                                         //(set to zero at startup)
      //s5=dm(i5,m0);                      //s5=1st input sample, leave i5 pointing to 1st 
                                         //sample                                   
                                                                  
   
      modify(i13,m9);                    //i13-->current starting point for reading 
                                         //in states array for channel
                                         
      i14=i13;                           //write pointer,(i14) always starts 2 behind 
      modify(i14,-2);                    //read pointer, (i13);
      
      
      
      
                                         
      r3=dm(i0,1);                       //r3=coeff set index for channel
                                         //i0-->coeff set index for channel+1
                                                                     
      r3=r3*r0(uui),r5=pm(i12,m10);      //r3=offset into coefficients table for channel
                                         //r5 = last sample of previous data buffer, 
                                         //(set to zero at startup)
                                      
                                         
      r3=r3+r1,s5=dm(i5,m0);             //r3=addr of start of coeff set for channel
                                         //s5=1st input sample, leave i5 pointing to 1st 
                                         //sample 
                                        
     
      b4=r3;                             //b4=i4=start of coeff set for channel.  We use
                                         //i4 to index the coeff set as a circular buffer
      
                                         
      bit set Mode1 PEYEN;                //enable SIMD

      modify(i2,1);                       //i2-->out[channel+1] for next loop iteration

     
      //Do a SIMD write to states         //states(i14) = last sample of last in buffer
                                          //states(i14+1) = first sample of new input buffer
      pm(i14,1) = r5;                    
      
      //nop;
      //nop;
                
   lcntr=r12, do Sample_Loop until lce;    //Loop over all samples in the block
                                           //(sample=0-->n-1)
      
                                           
                                           
      //i13 points to starting point in states
      //array for this sample For ease of notation in
      //comments below we will call this
      //starting point states(0).  This is not
      //accurate since states is a circular buffer
      //and starting point will be different for 
      //each sample.
                                                                         
      //Notation: 
      //p(i)=coefs(i)*states(i)
      //sumE(i) = sum from 0 to i of 
      //p(i) for all even values of i
      //sumO(i) = sum from 1 to i of
      //p(i) for all odd values of i
      
      //Filter Pre-Loop
      r8=r8-r8,r2=dm(i4,m3),r5=pm(i13,m12);   //Clear 'accumulator': r8,s8=0
                                              //r2=coeffs(0), s2=coeffs(1)
                                              //r5=states(0), s5=states(1)
                                           
      f14=f2*f5,r2=dm(i4,m3),r5=pm(i13,m12);  //r14=p(0), s14=p(1)
                                              //r2=coeffs(2), s2=coeffs(3)
                                              //r5=states(2), s5=states(3)
                                           
      
                                           
   //Filter Loop: L/2 - 3 iterations.  Let 
   //loop index = i, i=0-->(L/2 - 4)
   lcntr=r15, do Filter_Loop until lce; 
Filter_Loop:  
f14=f2*f5,f8=f8+f14,r2=dm(i4,m3),r5=pm(i13,m12);  //r14=p(2i+2), s14=p(2i+3)
                                                  //r8=sumE(2i), s8=sumO(2i+1)
                                                  //r2=coeffs(2i+4), s2=coeffs(2i+5)
                                                  //r5=states(2i+4), s5=states(2i+5) 
//Filter post loop:                                                  
f14=f2*f5,f8=f8+f14,r2=dm(i4,m3),r5=pm(i13,m8);   //r14=p(L-4), s13=p(L-3)
                                                  //r8=sumE(L-6), s8=sumO(L-5)
                                                  //r2=coefs(L-2), s2=coefs(L-1)
                                                  //r5=states(L-2), s5=states(L-1)
                                                  //m8=3 thus positioning i13 at 
                                                  //correct starting point when 
                                                  //states array is updated. 
                                               
   f14=f2*f5,f8=f8+f14;                        //r14=p(L-2), s14=p(L-1)
                                               //r8=sumE(L-4), s8=sumO(L-3)
                                               
   f8=f8+f14,f5=dm(i5,1);                      //r8=sumE(L-2), s8=sumO(L-1)
                                               //r5 = current input sample
                                               //s5 = next input sample
                                                         
   
   f9<->s8;                             //r9=sumO(L-1), s8=junk but we don't care
                                        
                                 
   
   f8=f8+f9,pm(i14,1)=f5;               //r8=sumE(L-2)+sumO(L-1) = filter output
                                        //s8=junk but we dont care
                                        //SIMD write storing current and next samples
                                        //into states array, (current sample overwrites
                                        //current sample and next sample is new entry).
                                         
   
               
Sample_Loop:  pm(i9,1)=r8;             //store filter ouput in output data array
                                       //Note: we write r8=filter output to next
                                       //location in output array. s8 is written to
                                       //the following location in output array.
                                       //s8 contains junk. This is ok because in the
                                       //next sample loop iteration we overwrite
                                       //this with valid data.  In our last write
                                       //to output array we do a SISD write with r8 
                                       //thus avoiding writing beyond the end of the
                                       //output array..


//******** START CODE FOR LAST SAMPLE  ********************************************


      //i13 points to starting point in states
      //array for this sample For ease of notation in
      //comments below we will call this
      //starting point states(0).  This is not
      //accurate since states is a circular buffer
      //and starting point will be different for 
      //each sample.
                                                                         
      //Notation: 
      //p(i)=coefs(i)*states(i)
      //sumE(i) = sum from 0 to i of 
      //p(i) for all even values of i
      //sumO(i) = sum from 1 to i of
      //p(i) for all odd values of i



 //Filter Pre-Loop
 
      r3 = r5;                                //r3 = in(n-2), s3 = in(n-1)      
 
 
      r8=r8-r8,r2=dm(i4,m3),r5=pm(i13,m12);   //Clear 'accumulator': r8,s8=0
                                              //r2=coeffs(0), s2=coeffs(1)
                                              //r5=states(0), s5=states(1)
                                           
      f14=f2*f5,r2=dm(i4,m3),r5=pm(i13,m12);  //r14=p(0), s14=p(1)
                                              //r2=coeffs(2), s2=coeffs(3)
                                              //r5=states(2), s5=states(3)
                                           
      
                                           
   //Filter Loop: L/2 - 3 iterations.  Let 
   //loop index = i, i=0-->(L/2 - 4)
   lcntr=r15, do Filter_Loop2 until lce; 
Filter_Loop2:  
f14=f2*f5,f8=f8+f14,r2=dm(i4,m3),r5=pm(i13,m12);  //r14=p(2i+2), s14=p(2i+3)
                                                  //r8=sumE(2i), s8=sumO(2i+1)
                                                  //r2=coeffs(2i+4), s2=coeffs(2i+5)
                                                  //r5=states(2i+4), s5=states(2i+5) 
//Filter Post Loop:
f14=f2*f5,f8=f8+f14,r2=dm(i4,m3),r5=pm(i13,m8);   //r14=p(L-4), s13=p(L-3)
                                                  //r8=sumE(L-6), s8=sumO(L-5)
                                                  //r2=coefs(L-2), s2=coefs(L-1)
                                                  //r5=states(L-2), s5=states(L-1)
                                                  //m8=3 thus positioning i13 at 
                                                  //correct starting point when 
                                                  //states array is updated.
                                               

   f14=f2*f5,f8=f8+f14;                        //r14=p(L-2), s14=p(L-1)
                                               //r8=sumE(L-4), s8=sumO(L-3)
                                               
   f8=f8+f14;                                  //r8=sumE(L-2), s8=sumO(L-1)
                                               
                                                         
   f9<->s8;                                   //r9=sumO(L-1), s8=junk but we don't care
                                        
                                 
   
   f8=f8+f9;                            //r8=sumE(L-2)+sumO(L-1) = filter output
                                        //s8=junk but we dont care
   
   
//******** END CODE FOR LAST SAMPLE   ********************************************




 
bit clr Mode1 PEYEN      ;             //disable SIMD
nop;
pm(i12,m10)=s3;                        //states[L+1] = in[n-1] 

pm(i9,1)=r8;                           //store filter ouput in output data array
                                       //Note: we write last sample of block in SISD
                                       //mode.  If we did this in SIMD we would write
                                       //junk to memory location after end of output
                                       //data array.


Channel_Loop: r4=r4+r10;                //r4 = start of states array for next channel
      
       

      //Store new value of p->stateIndex                               
      r4=b13;                        //r4 = start of states array for last channel
      r9=i13;                        //r9 = (start states array for last channel) +
                                     //     (offset into states arry for next write)
      r9=r9-r4;                      //r9 = new value for p->stateIndex, (=offset into
                                     //     states array for next write for all channels).
      i5=pStateIndex;
      dm(m0,i5)=r9;          //store new value
        
    
      //------------------------------------
      // Clean Up for exit
      //------------------------------------
      // reset length registers
      l4=0;
      l13=0;
      l14=0;
      //disable circular buffering
      
      //bit clr Mode1 CBUFEN; 
      //pop context off stack                         
      restore_reg_FE;                    
  

      
   leaf_exit;                             //call leaf exit macro from asm_sprt,
                                          //restores stack, frame pointers and 
                                          //returns to calling function
   
_FirEvenSplitProcess.end:
