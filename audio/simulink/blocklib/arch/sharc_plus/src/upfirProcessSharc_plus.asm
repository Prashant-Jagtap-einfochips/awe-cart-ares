/*****************************************************************************************

  BOSE CORPORATION
  COPYRIGHT 2006 BOSE CORPORATION ALL RIGHTS RESERVED
  This program may not be reproduced, in whole or in part in any
  form or any means whatsoever without the written permission of:

      BOSE CORPORATION
      The Mountain
      Framingham, MA 01701-9168
********************************************************************************
*  Title:       upfirProcessASM.asm
*  Main Entry Function : _upfirProcessASM
*
*  Description: 
*     Optimized SHARC assembly version of upfirProcess.c.
*     Performs FIR filtering on one block of data for each of
*     a set of input channels
*     See bdspupfir.m for algorithm details.
*     This code works for byte addressed platform only. 
*     Assembler options for this file - "char size for imported headers" is -char-size-8
*
*  References:  Based on upfirProcess.c and bdsdupfir.m
*               Testing can be performed by TestBed_upfirProcess.c
*
********************************************************************************


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

.IMPORT "upfir.h";



//--------------------------------------------------------------------------------
//Export Functions
//--------------------------------------------------------------------------------
.global upfirProcess.;  

.global upfirReset.;

//--------------------------------------------------------------------------------
//Import Functions
//--------------------------------------------------------------------------------



//--------------------------------------------------------------------------------
//Local Defines & Macros
//--------------------------------------------------------------------------------
// Add comment to describe defines
#define numChannels    reads(1)
#define state          reads(2)
#define L              reads(3)
#define stateIndex     reads(4)
#define coeffs         reads(5)
#define activeCoeffSet reads(6)
#define I              reads(7)
#define polyLen        reads(8)


//macro to save registers used by this routine (non scratch) onto stack 
#define save_reg_FE     r0=b4; \
                        puts=r0;\
                        r0=b13; \
                        puts=r0;\
                        puts=r11;\
                        puts=r10;\
                        r0=m11;\
                        puts=r0;\
                        r0=i11;\
                        puts=r0;\
                        r0=i12;\
                        puts=r0;\
                        r0=m12;\
                        puts=r0;\
                        r0=m3;\
                        puts=r0;\
                        puts=r7;\
                        puts=r6;\
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
                        puts=r0
                                                

                                                                         
//macro to restore registers used by this routine and reset stack pointer
#define restore_reg_FE  m9=gets(1);\
                        i9=gets(2);\
                        i5=gets(3);\
                        i3=gets(4);\
                        i2=gets(5);\
                        i0 =gets(6);\
                        r15 =gets(7);\
                        r14 =gets(8);\
                        r9 =gets(9);\
                        r5 =gets(10);\
                        r3 =gets(11);\
                        r6 =gets(12);\
                        r7 =gets(13);\
                        m3 =gets(14);\
                        m12=gets(15);\
                        i12=gets(16);\
                        i11=gets(17);\
                        m11=gets(18);\
                        r10=gets(19);\
                        r11=gets(20);\
                        b13=gets(21);\
                        b4=gets(22);\
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
 
Function Name : upfirProcess

C Callable : Yes, leaf function

C ProtoType :void upfirProcess(block_t *p0, float *x0[], int n);

Arguemts:   R4=block_t * -DM Memory Pointer to upfir_t  (overlaid on block_t)
            R8=float **  -  (BDSP parameter passing)
                             This is an array of pointers (DM Memory).  
                             First numChannels point to input channels in DM Memory
                             next  numChannels point to output channels in DM Memory
            R12= int,  block size (Number of samples to process)
            
   
Registers Used: 
           Core:   r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r14,r15
           DAG :   i0,i2,i3,i4,i5,i9,i11,i12,i13, 
                   m4,m3,m9,m11,m12
                   l4,l13,b4,b13
           Other: mode1: set/clear circular buffering and SIMD mode
                  (at exit mode1 identical to mode1 at entry so don't 
                   pust on stack)
           
Max Clock Cycles : TBD

Processor Type :ADI SHARC 21365;
                     
Key Assumptions : States arrays in PM space
                  Coeffs arrays in DM space
                  C Calling Convention
                  polyLen, (length of each polyphase component) is EVEN
                  (polyLen = length of states arrays)
                             
Special Coding Tricks : Use 2 SIMD Pre-loop instructions and 2 SIMD
                        Post-loop instructions.  This allow us to:
                         1.)Loop on an instruction with max parallel operations
                         2.)End SIMD computation with pointers positioned 
                            correctly for next sample.

Mathematical Equations:   see upfirProcess.c, bdspupfir.m       
***********************************************************************************/
upfirProcess..start:
upfirProcess.:

   leaf_entry;                          //call leaf entry macro from asm_sprt
   
   //At entry we have:
   //r4  = pointer to base of upfir_t
   //r8  = pointer to x0 = an array of pointers
   //      which point to the input and output
   //      data blocks for each channel
   //r12 = number of samples per data block 

   save_reg_FE;                        //save context on stack 
   
   bit set Mode1 CBUFEN;                //enable circular buffering
   m3=2;                                //DAG1 +2 increment
   m12=2;                               //DAG2 +2 increment
      
   i2=r4;                               //i2-->output buffer;
   m4=numChannels;               //m4 = numChannels
   r0=L;                         //r0  = L, (# filter taps)
   r6=polyLen;                   //r6  = polyLen, (length of each states array)
   r15=2;
   r11=r6+r15;                          //r11 = polyLen + 2 = increment from states array of channel i
                                        //to states array of channel i+1, (includes the 2 dummy locations
                                        //at end of each channel's states array)  
   m11=r6;                              //m11 = polyLen, (length of each states array);
   r15=r6;                              //r15 = polyLen, (length of states array)
   r15=lshift r15 by -1;                //r15 = polyLen/2 - 2 for SIMD computation
   r15=r15-1;                           //(we have 2 SIMD post loop computations
   r15=r15-1;                           // so that states and coeffs pointers end
                                        // in the correct place for next sample)
   i3=r8;                               //i3-->x0[0]
   l4=r0;                               //l4 = circular buffer length for coeffs
   l13=r6;                              //l13 = polyLen = circular buffer length 
                                        //for states
   r4=state;                     //r4 = start states array for channel 0
   i4 = stateIndex;
   m9=dm(0x0,i4);                //m9=starting point in states array for 
                                        //all channels, (can't assume starting point
                                        //is first element of the channels states array,
                                        //BUT starting point is same for all channels).
  
   //In the sample loop, we only
   //update the states array every
   //I samples. We use r7,r10 for this
   r7=I;                         //r7 = I = upsample factor
   r10=r7;                              //r10 = I
                                        
                                                                            
   i0=activeCoeffSet;            //i0-->coeff set index for channel 0  
   r1=coeffs;                    //r1 = start addr of coefficient table
   
   
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
   r0 = r0*r3(uui);                     // line 311: r3=r3*r0(uui) which computes the offset into
                                        // coefficient's array for each channel, since this is in 
                                        // the compute registers, we multiply it by 4 for the  
                                        // above explained reason.
   r11 = r11*r3(uui);                   // line 405: Channel_Loop: r4=r4+r11 computes the start of state's
                                        // array for each channel, where r11 is the offset, so we multiply
                                        // it by 4 for the above explained reason.
   /////////////////////////////////////////////////////////////
   
      i5=i3;   // load output
      i9=i2;   // load input 
   lcntr=m4, do Channel_Loop until lce;  //Loop over all channels, (channel=0-->numChannels-1)
                                         
      b13=r4;                            //b13=i13=start of states array for channel
      i11=r4;                            //***i11-->start of states array for channel
      i12=r4;                            //***i12-->start of states array for channel
      // (NW) qualifer on the modify instruction specifies that the offset is in words
	  modify(i12,m11)(NW);                   //***i12-->one past end of states array for channel
                                         //***(i12-->dummy location at end of states
                                         //array used to fix SIMD problem reading circular 
                                         //buffers
      
      
      // (NW) qualifer on the modify instruction specifies that the offset is in words
      modify(i13,m9)(NW);                    //i13-->current starting point in states array
                                         //for channel
                                         
      r3=dm(i0,1);                       //r3=coeff set index for channel
                                         //i0-->coeff set index for channel+1
      
                                                                     
      r3=r3*r0(uui);                     //r3=offset into coefficients table for channel
                                         
      r3=r3+r1;                          //r3=addr of start of coeff set for channel
     
      b4=r3;                             //b4=start of coeff set for channel.  We use
                                         //i4 to index the coeff set as a circular buffer
      
      
      f2=pm(i11,0);                      //f2 = first sample of states array for channel
      
      pm(i12,0)=f2;                      //We write first sample of states array to 
                                         //end + 1 of states array to handle the case
                                         //when a SIMD read crosses the circular buffer
                                         //boundary                                  
      
           
   lcntr=r12, do Sample_Loop until lce;    //Loop over all samples in the block
                                           //(sample=0-->n-1)                                                                   
      //Test if sample % I = 0                                    
      compu(r10,r7);                       //test if r10 == I
                                                                                
      if NE jump noNewVal(db);             //if r10 % I !=0 we don't update states array
      r10=r10+1;                           //and we don't reset r10. We fill the delayed
      nop;                                 //slots with r10 increment and a nop.  If we 
                                           //don't do the jump then reset of r10 overwrites
                                           //the increment.
                                 
      r10=1;                               //if r10 % I == 0, we set r10 to 1 and
      f9=dm(i5,1);                         //get sample.  f9 = sample, and then  
      pm(i13,1)=f9;                        //update states array with sample
                                           //i13-->starting point of states
                                           //array for filter loop. For the purpose
                                           //of comments below we will call this
                                           //starting point states(0), (even though
                                           //states is circular).
                                           
                                           
      f2=pm(i11,0);                      //f2 = first sample of states array for channel
      
      pm(i12,0)=f2;                      //We write first sample of states array to 
                                         //end + 1 of states array to handle the case
                                         //when a SIMD read crosses the circular buffer
                                         //boundary
      
      //Notation: 
      //p(i)=coefs(i)*states(i)
      //sumE(i) = sum from 0 to i of 
      //p(i) for all even values of i
      //sumO(i) = sum from 1 to i of
      //p(i) for all odd values of i
      
      
noNewVal:  bit set Mode1 PEYEN;            //Switch to SIMD mode                   
           nop;                            //nop due to mode change latency
           //#if __VISUALDSPVERSION__ == 0x05010200
           //   nop; // For Anomaly 09000022/15000004
           //#endif

      //SIMD Pre-Loop
      r8=r8-r8,r2=dm(i4,m3),r5=pm(i13,m12);   //Clear 'accumulator': r8,s8=0
                                              //r2=coeffs(0), s2=coeffs(1)
                                              //r5=states(0), s5=states(1)
                                           
      f14=f2*f5,r2=dm(i4,m3),r5=pm(i13,m12);  //r14=p(0), s14=p(1)
                                           //r2=coeffs(2), s2=coeffs(3)
                                           //r5=states(2), s5=states(3)
                                           
      
                                           
   //SIMD Loop: polyLen/2 - 2 iterations. Let 
   //loop index = i, i=0-->polyLen/2 - 3
   lcntr=r15, do Filter_Loop until lce; 
Filter_Loop:  
f14=f2*f5,f8=f8+f14,r2=dm(i4,m3),r5=pm(i13,m12);  //r14=p(2i+2), s14=p(2i+3)
                                                  //r8=sumE(2i), s8=sumO(2i+1)
                                                  //r2=coeffs(2i+4), s2=coeffs(2i+5)
                                                  //r5=states(2i+4), s5=states(2i+5) 

   //SIMD Post Loop:
   f14=f2*f5,f8=f8+f14;                        //r14=p(polyLen-2), s14=p(polyLen-1)
                                               //r8=sumE(polyLen-4), s8=sumO(polyLen-3)
                                               
   f8=f8+f14;                                  //r8=sumE(polyLen-2), s8=sumO(polyLen-1)               
   
                                

   bit clr Mode1 PEYEN      ;           //disable SIMD
   nop;                                 //nop due to mode change latency
   f9<->s8;                             //r9=sumO(polyLen-1), s8=junk but we don't care
   f8=f8+f9;                            //f8=sumE(L-2)+sumO(polyLen-1) = filter output
                                
               
Sample_Loop:  pm(i9,1)=r8;              //store filter ouput in output data array
//modify(i5,24)(NW);
Channel_Loop: r4=r4+r11;                 //r4 = start states array for next channel
       


      //Store new value of p->stateIndex 
      r4=b13;                        //r4 = start of states array for last channel
      r9=i13;                        //r9 = (start states array for last channel) +
                                     //     (offset into states arry for next write)
      r9=r9-r4;                      //r9 = new value for p->stateIndex, (=offset into
                                     //     states array for next write for all channels).
                                     
      r9 = lshift r9 by -2;
      i4 = stateIndex;          //store new value
      dm(0x0,i4) =  r9;
      
      //------------------------------------
      // Clean Up for exit
      //------------------------------------
      // reset length registers
      l4=0;
      l13=0;
      //disable circular buffering
    //  bit clr Mode1 CBUFEN; 
      //pop context off stack                         
      restore_reg_FE;                    
  

      
   leaf_exit;                             //call leaf exit macro from asm_sprt,
                                          //restores stack, frame pointers and 
                                          //returns to calling function
   
upfirProcess..end:

/**********************************************************************************
                             Reset Function:
 
Function Name : upfirReset.

C Callable : Yes, leaf function

C ProtoType :void upfirReset(block_t *);

Arguments:   R4=block_t * -DM Memory Pointer to upfirReset_t  (overlaid on block_t)

Return Value: None
                             
Registers Used: 
           Core:   r0,r1,r2
           DAG :   i4 
           Other: None
           
Max Clock Cycles : TBD

Processor Type :ADI SHARC 21365;

Key Assumptions : 
                             
Special Coding Tricks : 

Mathematical Equations:   Zeros out the state vector (delay line)  
***********************************************************************************/

upfirReset.:
  
  /* implement the following c funvtion
   {
    upfir_t *p;
    float *state;
    int i, L;
    
    p = (upfir_t *)p0;
    state = p->state;
    L=p->polyLen * p0->numIn;
    for(i=0;i<L;i++)
      state[i]=0.0f;
   } */


   leaf_entry;                        // call leaf entry macro from asm_sprt
  
   i4=r4;
   r1=polyLen;                   //r1  = polyLen, (length of each states array)
   r2=numChannels;               //r2 = numChannels
   r0=2;                                //extra 2 locations
   r1=r1+r0;
   r2=r1*r2(uui);                       //PolyLen*NumChannels
   
   r4=state;                     //r4 = start states array for channel 0
   
   i4=r4;
   
    f1=0.0;
   //zero out the state arrau
   lcntr=r2, do thestate_zero until lce;
    dm(i4,1)=f1;
thestate_zero: nop;    
   
        
   leaf_exit;                             //call leaf exit macro from asm_sprt,
                                          //...restores stack, frame pointers and returns to calling function
   
upfirReset..end:


