/*****************************************************************************************

  BOSE CORPORATION
  COPYRIGHT 2006 BOSE CORPORATION ALL RIGHTS RESERVED
  This program may not be reproduced, in whole or in part in any
  form or any means whatsoever without the written permission of:

      BOSE CORPORATION
      The Mountain
      Framingham, MA 01701-9168
********************************************************************************
*  Title:       IFirProcess.asm
*  Main Entry Function : IFirProcess.
*
*  $Header$
*
*  Description: 
*     Optimized SHARC assembly version of IFirProcess.c.
*     Performs FIR filtering on one block of data for each of
*     a set of input channels
*     See bdspIFir.m for algorithm details.
*     This code works for byte addressed platform only. 
*     Assembler options for this file - "char size for imported headers" is -char-size-8
*
*  References:  Based on IFirProcess.c and bdsdIFir.m
*               Testing can be performed by TestBed_IFirProcess.c
*
********************************************************************************

   $Log$
   Revision 1.3  2010/04/16 17:14:11  hc9330
   Changed processor related header file to "processor.h".

   Revision 1.2  2006/08/24 21:17:37  sc10652
   added headers and log; fixed indexing bug in reset


*****************************************************************************************/

//------------------------------------------------------------------------------
//assembler directives 
//-------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//includes
//-------------------------------------------------------------------------------

#include <platform_include.h>   // processor related definition file
#include "asm_sprt.h"    //useful macros for doinng Mixed Programming



/***  External Variable References  *******************************************/

//.IMPORT "IFir.h";


//--------------------------------------------------------------------------------
//Export Functions
//--------------------------------------------------------------------------------
.global IFirProcess.;  


//--------------------------------------------------------------------------------
//Import Functions
//--------------------------------------------------------------------------------



//--------------------------------------------------------------------------------
//Local Defines & Macros
//--------------------------------------------------------------------------------

//the following are offests from the base of the BDSP structure - IFir_t 
// The offsetof are in bytes,convert it words by dividing it by 4
#define numChannels    reads(1);
#define state          reads(2);
#define L              reads(3);
#define stateIndex     reads(4);
#define coeffs         reads(5);
#define activeCoeffSet reads(6);
#define I              reads(7);

//macro to save registers used by this routine (non scratch) onto stack 
#define save_reg_FE     r0=m10;\
                        puts=r0;\
                        r0=m0;\
                        puts=r0;\
                        r0=m1;\
                        puts=r0;\
                        r0=m8;\
                        puts=r0;\
                        r0=i14;\
                        puts=r0;\
                        r0=b14;\
                        puts=r0;\
                        r0=i1;\
                        puts=r0;\
                        puts=r10;\
                        r0=m0;\
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
                        puts=r0;\
                        r0=b1;\
                        puts=r0
                                                                         
//macro to restore registers used by this routine and reset stack pointer
#define restore_reg_FE  b1=gets(1);\
                        b13=gets(2);\
                        m9=gets(3);\
                        i9=gets(4);\
                        i5=gets(5);\
                        i3=gets(6);\
                        i2=gets(7);\
                        i0 =gets(8);\
                        r15 =gets(9);\
                        r14 =gets(10);\
                        r9 =gets(11);\
                        r5 =gets(12);\
                        r3 =gets(13);\
                        m0=gets(14);\
                        r10=gets(15);\
                        i1=gets(16);\
                        b14=gets(17);\
                        i14=gets(18);\
                        m8=gets(19);\
                        m1=gets(20);\
                        m0=gets(21);\
                        m10=gets(22);\
                        alter(22)

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
 
Function Name : IFirProcess.

C Callable : Yes, leaf function

C ProtoType :void IFirProcess(block_t *p0, float *x0[], int n);

Arguemts:    R4 = float* - DM Memory Pointer to output
             R8 = float* - DM Memory Pointer to input
             R12 = frame_size, frame size
             
Registers Used: 
           Core:   r0,r1,r2,r3,r4,r5,r8,r9,r10,r12,r14,r15
           DAG :   i0,i1,i2,i3,i5,i9,i13,i14 
                   m0,m1,m4,m8,m9,m10,m12
                   l1,l13,l14,b1,b13,b14
           Other: mode1: set/clear circular buffering and SIMD mode
                  mode1: set/clear broadcast mode for i1
                  (at exit mode1 identical to mode1 at entry so don't 
                   push on stack)
           
Max Clock Cycles : TBD

Processor Type :ADI SHARC 215xx;
                     
Key Assumptions : 1.)States arrays in PM space
                  2.)Coeffs arrays in DM space
                  3.)C Calling Convention
                  4.)L = # filter taps is EVEN
                  5.)I = interpolation factor must be EVEN
                     !!If I is not even function does not
                       give correct results!!
                  
                 
                  
                                                        
Special Coding Tricks : 
Special tricks were used to meet 2 optimization goals:
1.)Execute entire sample loop in SIMD mode. Main benefit of this goal is that we 
   get rid of the 4 instructions needed to enable/disable SIMD each sample.
2.)Efficiently solve the SIMD circular buffer read problem. Previous method for
   solving this problem was to copy states[0] to states[polyLen] every sample.
   Goal here is to eliminate this copy.
Tricks used to meet 2 goals above:
1.)SIMD scheme is: Entire sample loop is done in SIMD mode.  Each sample loop 
   iteration: 2 consecutive input samples are read via SIMD and 2 consecutive 
   output samples are written via SIMD to states. Each sample loop iteration processes 
   2 input samples. Coefficients are read using broadcast mode so we get same coeff 
   value in primary and shadow registers. States are read using standard SIMD reads 
   and so each filter loop produces 2 ouputs: sample 'n' and sample 'n+1'.  
   This scheme solves SIMD circular buffer read problem since circular buffer boundary 
   is never crossed with a SIMD read.   
2.)States array is accessed with 2 different pointers: a read pointer and a 
   write pointer. i13 used for reading and i14 used for writing.                        
3.)Writes to states array are all done in SIMD. Each write adds 2 new values.
4.)Relation of read and write pointers ar fixed such that states array is 
   read and written correctly using minimal cycles.
5.)States buffer read pointer i13 is incremented by I+2 instead of I after last 
   read for each filterloop. This positions read pointer correctly for next 
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
                            
                            
                            

Mathematical Equations:   see IFirProcess.c, bdspIFir.m       
***********************************************************************************/
IFirProcess..start:
IFirProcess.:

   leaf_entry;                          //call leaf entry macro from asm_sprt
   
   //At entry we have:
   //r4  = pointer to base of IFir_t
   //r8  = pointer to x0 = an array of pointers
   //      which point to the input and output
   //      data blocks for each channel
   //r12 = number of samples per data block 

   save_reg_FE;                         //save context on stack 
   
   BIT SET MODE1 BITM_REGF_MODE1_CBUFEN;//enable circular buffering 
   
   i2=r4;                               //i2-->base of IFir_t;
   
   
   m0=1;                                //DAG1 +1 increment
   m10=2;                               //DAG2 +2 increment
   r15=I;                               //r15=I 
   m12=r15;                             //m12=I = increment for reading
                                        //states array
                                        
   r15=r15+1;                           //r15=I+1
   r15=r15+1;                           //r15=I+2
   m8=r15;                              //m8=I+2 = increment for states array reads
                                        //used ONLY in filter post loop to position read
                                        //pointer for reading immediately after a write
                                        //has been done to states array
      
   m4=numChannels;                      //m4 = numChannels
   r0=L;                                //r0  = L = size of coefficient array
   r10=I;                              //r10=I
   r10=r10*r0(uui);                    //r10=stateLen = length of states array = L*I
     
   r15=r0;                              //r15 = L = size of coefficient array
   r15=r15-1,i3=r8;                     //i3-->x0[0]
   r15=r15-1,l1=r0;                     //l1 = L = circular buffer length for coeffs                                                          
   r15=r15-1,l13=r10;                   //l13 = stateLen = circular buffer length for states
                                        //r15=L-3.  r15 is used as loop counter for filter loop.
                                        // Each iteration of our filter loop we read 2 consecutive 
                                        //data samples and multiply them by the SAME filter 
                                        //coefficient. Thus each filter loop produces TWO output 
                                        //samples. We have 2 pre-loop instructions and one post-loop
                                        //instruction and so we do L-3 loop iterations
                                        
   r1=r0;                               //r1=L = size of coefficient array
   r1=r1-1;                             //r1=L-1
   m1=r1;                               //m1=L-1
   
   l14=r10;                             //l14 = stateLen = circular buffer length for states
   i4 = stateIndex;                     // get stateIndex for the current channel
   m9=dm(0x0,i4);						// store new value to the state index
   //m9=stateIndex;                     //m9=starting point in states array for 
                                        //all channels, (can't assume starting point
                                        //is first element of the channels states array,
                                        //BUT starting point is same for all channels).                                       
   
   i0=activeCoeffSet;                   //i0-->coeff set index for channel 0  
   r1=coeffs;                           //r1 = start addr of coefficient table
   
   
   r12=lshift r12 by -1;                   //r12 at input = number of samples for this block. 
   r12=r12-1;                              //We use r12 as numIterations for sample loop. 
                                           //We decrement divide by 2 since we process 2 samples
                                           //per loop iteration.  We decrement by 1 since 
                                           //we process the last 2 samples outside of the loop
                                           //for efficiency.
                                           
   bit set Mode1 BITM_REGF_MODE1_BDCST1;  //Set i1 for broadcast loads. We use i1 to access
                                          //coefficients array. 
                                           
                                           
   r4=state;                        //r4 = start states array for channel 0
   
   
   
   //////////////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////
   /*
     If the address calculation is done in compute(data) registers,
     for instance rx = rx + offset , then it should be changed to
     rx = rx + 4 * offset. 
     
     "rx = rx+offset" works in a word addressed scheme, for which this 
     code was initially written. For this code to work in a byte addressed
     scheme and since 1 word = 4 bytes, we need to multiply the address 
     modifier offset by 4.   
     
     If the address calculation is done in DAG rather than Data registers,
     the following instructions is not neccessary.
   */
   
   r3 = 4; 
   r0 = r0*r3(uui);              // line 385: r3=r3*r0(uui) which computes the offset into
                                 // coefficient's array for each channel, since this is in 
                                 // the compute registers, we multiply it by 4 for the  
                                 // above explained reason.
   r10 = r10*r3(uui);            // line 599: Channel_Loop: r4=r4+r10 computes the start of state's
                                 // array for each channel, where r10 is the offset, so we multiply
                                 // it by 4 for the above explained reason.
   
   ////////////////////////////////////////////////////////////////////////
   
   i9 = i2;
   i5 = i3;   
   lcntr=m4, do Channel_Loop until lce;  //Loop over all channels, (channel=0-->numChannels-1)
      
      //i9=dm(m4,i3);                      //i9-->out[channel]
      
      //r3=dm(i3,1);                       //i5--->in[channel]
      //i5=r3;                             //i3--->pointer to in[channel+1] for next 
                                         //loop iteration
        
       
      b13=r4;                            //b13=i13=start of states array for channel
                                         //i13 functions as read pointer for states array                                  
      b14=r4;                            //b14=i14=start of states array for channel
                                         //i14 functions as write pointer for states array
                                         
                                       
                                                                  
      // (NW) qualifer on the modify instruction specifies that the offset is in words
      modify(i14,m9)(NW);                 //i14-->current starting point for writing 
                                         //in states array for channel
                                         
      i13=i14;                           //read ptr = write ptr 
      
      
      r3=dm(i0,1);                       //r3=coeff set index for channel
                                         //i0-->coeff set index for channel+1
      
                                                                     
      r3=r3*r0(uui);                     //r3=offset into coefficients table for channel
                                         
                                      
                                         
      r3=r3+r1;                          //r3=addr of start of coeff set for channel
                                         
                                        
     
      b1=r3;                             //b1=i1=start of coeff set for channel.  We use
                                         //i1 to index the coeff set as a circular buffer
      
      modify(i1,m1)(NW);                 //i1 points to last coefficient in coeffs array                     
      
      
                                         
      bit set Mode1 BITM_REGF_MODE1_PEYEN;//enable SIMD
      nop;                                //nop due to mode change latency
      //Read 2 input samples from input
      //data array
      r5=dm(i5,2);                         //r5=input sample s
                                           //s5=input sample s+1
                                           
      //Write the 2 input samples to states
      //array
      pm(i14,m10)=r5;                      //states(i14)=sample s
                                           //states(i14+1)=sample s+1
      
      
      
      
      
                
   lcntr=r12, do Sample_Loop until lce;    //Loop over all samples in the block
                                           //We read 2 input data points and generate 
                                           //2 output data points each loop iteration.
                                           //We process the last 2 samples outside of the
                                           //loop for efficiency and so we need: (n/2)-1
                                           //loop iterations.  We will refer to the loop
                                           //counter as s. We have s=0 to n-4, AND s is 
                                           //incremented by 2 each iteration
                                           
                                                                                 
                                           
      //i13 points to starting point for reading states
      //array for computation of this sample pair 
      //For ease of notation in
      //comments below we will call this
      //starting point states(0).  This is not always
      //accurate since states is a circular buffer
      //and starting point will be different for 
      //each sample. 
                                                                         
      //Notation: 
      //p1(0) = coeffs(L-1)*states(0)
      //p2(0) = coeffs(L-1)*states(1)
      
      
      //Filter Pre-Loop
      r8=r8-r8,r2=dm(i1,m0),r5=pm(i13,m12);   //Clear 'accumulator': r8,s8=0
                                              //r2=coeffs(L-1), s2=coeffs(L-1)
                                              //r5=states(0), s5=states(1)
                                           
      f14=f2*f5,r2=dm(i1,m0),r5=pm(i13,m12);  //r14=coeffs(L-1)*states(0) = p1(0)
                                              //s14=coeffs(L-1)*states(1) = p2(0)
                                              //r2=coeffs(0), s2=coeffs(0)
                                              //r5=states(I), s5=states(I+1)
                                           
      
                                           
   //Filter Loop: L-3 iterations.  Let 
   //loop index = i, i=0-->(L-4)
   //let k=i+1
   //let m=i+2
   //let p1(i+1) = coeffs(i)*states(k*I)
   //let p2(i+1) = coeffs(i)*states((k*I)+1)
   //let S1(i) = sum from 0 to i of p1(i)
   //let S2(i) = sum from 0 to i of p2(i)
   lcntr=r15, do Filter_Loop until lce; 
Filter_Loop:  
f14=f2*f5,f8=f8+f14,r2=dm(i1,m0),r5=pm(i13,m12);  //r14=p1(i+1)
                                                  //s14=p2(i+1)
                                                  //r8=S1(i), s8=S2(i)
                                                  //r2=coeffs(i+1), s2=coeffs(i+1)
                                                  //r5=states(m*I), s2=states((m*I)+1)

                                                  
//Filter post loop:                                                  
f14=f2*f5,f8=f8+f14,r2=dm(i1,m0),r5=pm(i13,m8);  //r14=p1(L-2), s14=p2(L-2)
                                                 //r8=S1(L-3), s8=S2(L-3)
                                                 //r2=coeffs(L-2), s2=coeffs(L-2)
                                                 //r5=states((L-1)*I)
                                                 //s5=states(((L-1)*I)+1)
                                                 //m8 increments i13 by I+2 so that 
                                                 //i13 is positioned correctly for
                                                 //reading after next write to 
                                                 //states array


                                                                                                 
                                               
   f14=f2*f5,f8=f8+f14,r5=dm(i5,2);              //r14=p1(L-1), s14=p2(L-1)
                                                 //r8=S1(L-2), s8=S2(L-2)
                                                 //Read next 2 input samples from input
                                                 //data array:
                                                 //r5=input sample s
                                                 //s5=input sample s+1
   
   
   
                                               
                                               
   f8=f8+f14,pm(i14,m10)=r5;                   //r8 = S1(L-1) = ouput sample 2s
                                               //s8 = S2(L-1) = ouput sample 2s+1
                                               //Write the 2 new input samples to 
                                               //states array:
                                               //states(i14)=sample s
                                               //states(i14+1)=sample s+1
                                               
                                               
                                                         
               
Sample_Loop:  pm(i9,2)=r8;                     //Write our 2 ouput samples to output data array


//*********************** START CODE FOR LAST SAMPLE *******************************************

      //i13 points to starting point for reading states
      //array for computation of this sample pair 
      //For ease of notation in
      //comments below we will call this
      //starting point states(0).  This is not always
      //accurate since states is a circular buffer
      //and starting point will be different for 
      //each sample. 
                                                                         
      //Notation: 
      //p1(0) = coeffs(L-1)*states(0)
      //p2(0) = coeffs(L-1)*states(1)
      
      
      //Filter Pre-Loop
      r8=r8-r8,r2=dm(i1,m0),r5=pm(i13,m12);   //Clear 'accumulator': r8,s8=0
                                              //r2=coeffs(L-1), s2=coeffs(L-1)
                                              //r5=states(0), s5=states(1)
                                           
      f14=f2*f5,r2=dm(i1,m0),r5=pm(i13,m12);  //r14=coeffs(L-1)*states(0) = p1(0)
                                              //s14=coeffs(L-1)*states(1) = p2(0)
                                              //r2=coeffs(0), s2=coeffs(0)
                                              //r5=states(I), s5=states(I+1)
                                           
      
                                           
   //Filter Loop: L-3 iterations.  Let 
   //loop index = i, i=0-->(L-4)
   //let k=i+1
   //let m=i+2
   //let p1(i+1) = coeffs(i)*states(k*I)
   //let p2(i+1) = coeffs(i)*states((k*I)+1)
   //let S1(i) = sum from 0 to i of p1(i)
   //let S2(i) = sum from 0 to i of p2(i)
   lcntr=r15, do Filter_Loop2 until lce; 
Filter_Loop2:  
f14=f2*f5,f8=f8+f14,r2=dm(i1,m0),r5=pm(i13,m12);  //r14=p1(i+1)
                                                  //s14=p2(i+1)
                                                  //r8=S1(i), s8=S2(i)
                                                  //r2=coeffs(i+1), s2=coeffs(i+1)
                                                  //r5=states(m*I), s2=states((m*I)+1)

                                                  
//Filter post loop:                                                  
f14=f2*f5,f8=f8+f14,r2=dm(i1,m0),r5=pm(i13,m8);  //r14=p1(L-2), s14=p2(L-2)
                                                 //r8=S1(L-3), s8=S2(L-3)
                                                 //r2=coeffs(L-2), s2=coeffs(L-2)
                                                 //r5=states((L-1)*I)
                                                 //s5=states(((L-1)*I)+1)
                                                 //m8 increments i13 by I+2 so that 
                                                 //i13 is positioned correctly for
                                                 //reading after next write to 
                                                 //states array


                                                 
                                               
   f14=f2*f5,f8=f8+f14;                          //r14=p1(L-1), s14=p2(L-1)
                                                 //r8=S1(L-2), s8=S2(L-2)
   
   
   
                                               
                                               
   f8=f8+f14;                                  //r8 = S1(L-1) = ouput sample s
                                               //s8 = S2(L-1) = ouput sample s+1
                                               
                                               
   pm(i9,2)=r8;                                //Write last 2 ouput samples to output data array


//*********************** END CODE FOR LAST SAMPLE *********************************************








                                        
bit clr Mode1 BITM_REGF_MODE1_PEYEN;           //disable SIMD
nop;										   //nop due to mode change latency

Channel_Loop: r4=r4+r10;                       //r4 = start of states array for next channel
      
       

      //Store new value of p->stateIndex                               
      r4=b14;                        //r4 = start of states array for last channel
      r9=i14;                        //r9 = (start states array for last channel) +
                                     //     (offset into states arry for next write)
      r9=r9-r4;                      //r9 = new value for p->stateIndex, (=offset into
                                     //     states array for next write for all channels).
      r9 = lshift r9 by -2;          // convert the r9 offset into words from bytes
      i4 = stateIndex				 // get state index for the current channel
	  dm(0x0,i4) =  r9;				 // store new value to the state index
	  //stateIndex=r9;       		 //store new value
    
      //------------------------------------
      // Clean Up for exit
      //------------------------------------
      
      //Disable broadcast mode for i1
      bit clr Mode1 BITM_REGF_MODE1_BDCST1; 
      
      
      // reset length registers
      l1=0;
      l13=0;
      l14=0;
      //disable circular buffering
      //bit clr Mode1 CBUFEN; 
      //pop context off stack                         
      restore_reg_FE;                    
  

      
   leaf_exit;                             //call leaf exit macro from asm_sprt,
                                          //restores stack, frame pointers and 
                                          //returns to calling function
   
IFirProcess..end:
