/********************************************************************************************************************************************************
*
* Function:  FIR - Multi channel FIR filter with option for state buffer sharing across channels. 
*
* Prototype: void fir_vec_mc(	float*  input[],				- Array of input buffer start address for all the channels.
*																Some elements of this array can be duplicate in case of input sharing.
*                               float*  output,					- Pointer to array of consecutive output buffers.
*                               float*  coefficients[],			- Array of coefficient buffer start address for all the channels
*																  Some elements of this array can be duplicate in case of filter sharing.
*																  The coefficients should be stored in reverse order. 	
*								float*  state[],				- Array of input buffer start address for all the channels.The size of the state buffer 
*																  should be minimum "taps+samples". Some elements of this array can be duplicate
*																  in case of state sharing.	
*								int*    stateSharingEnabled,	- Set this element to 1 if the corresponding filter shares the same state buffer with
*																  any other output channel with lesser array index, else set to 0. This information
*																  is used by the function to perform state update only for the first time and 
*																  skip it for the subsequent channels sharing the same state buffer. The number of taps 
*																  for the channels sharing the same state buffer should be the same.     		 
*								int*  	taps,					- Array of tap length values for all the channels. 
*								int     samples,				- Block size - same for all the channels. 
*								int     nch );					- Total number of channels 
*
* Implementation: The function is implemented in the following way:
*   
* 	1. Iterate for all the output channels.
*	2. Read the updated state buffer index from the location state[taps+samples-1].  
*	3. If stateSharingEnabled = 1, skip step 4.
*	4. Copy the new block of input data to the state buffer and update the state buffer index to point to the oldest sample in the state buffer.
*	   Save this updated state buffer index in a temp register. 
*	5. If state[taps+samples-1] wasn't overwritten while input data copy, duplicate state[0] in state[taps+samples-1].  
*	   Else, duplicate state[taps+samples-1] to state[0]. This allows us to use SIMD with circ1ular buffering for the location taps+samples-2.
*	6. Start FIR MAC loop with SIMD enabled. Process four samples in parallel - two in PEx and two in PEy. Two instructions in the FIR MAC loop 
*	   are required to do pipelined two stage MAC operations without compute to compute stalls. 
*	7. Write output samples. 
*	8. Update the new state buffer index at the location state[taps+samples-1].     
*	9. Go to step 1 to process the next output channel.   
*
* (c) Copyright 2019 Analog Devices, Inc.  All rights reserved.
*
* Rev history:  
* 				Rev 1: Initial Version
*				Rev 2: Updated second parameter to a single output buffer address for all the channels instead of an array of output buffer addresses
*				Rev 3: Fixed issue caused due to state[taps+samples-1] getting overwritten instead of state[0] because of circular buffer wraparound during input data copy 
*				Rev 4: Added a patch to support single tap FIR filter implementation.  
***********************************************************************************************************************************************************/

#include <platform_include.h>
#include <sys/anomaly_macros_rtl.h>
#include "../lib/src/libcc_src/xlibdefs.h"

.SECTION/CODE seg_pmco;
.GLOBAL       _fir_vec_mc;

#if defined(__BA_SHARC__)
.SET fir_vec_mc.,_fir_vec_mc;
#endif

_fir_vec_mc:

	/*
	Register Usage
	i3  -> input buffer address array
	i4  -> output buffer address 
	i10 -> coeff buffer address array  
	i12 -> state buffer address array 
	i13 -> bStateSharingEnabled array
	i8  -> taps  array
	
	i0, b0, l0 -> state buffer -> circular
	i5   	   -> start of the state buffer
	i9, b9, l9 -> coeff buffer -> circular
	i1 -> input buffer
	m4 -> 2, for SIMD address increments 
	m6, m14-> 1, for non-SIMD address increments 
	m5 -> 0, for data access without address increment
	m1 -> samples+2, used to reset the state buffer index register i0 to the correst offset after each iteration of 4 sample processing loop.
	m2 -> taps+samples-1, used to access the state buffer location taps+samples-1
	
	r5 -> Number of taps - 1 
	r6 -> To store the updated state buffer index temporarily  
	r7 -> Number of samples
	
	r8, r9 -> To store multiplication results 
	r12, r13-> To store MAC results 
	
	r2 -> To store remaiming samples to process. 
	r0, r1-> General purpose
	 
	*/ 

   /* Preserve registers */
   r0 = i0; puts = r0;
   r0 = i1; puts = r0;
   r0 = i3; puts = r0;
   r0 = i5; puts = r0;
   r0 = m1; puts = r0;
   r0 = m2; puts = r0;
   r0 = b0; puts = r0;
   puts = i8;
   puts = i10;
   puts = i9;
   puts = b9;
   puts = r5;
   puts = r6;
   puts = r7;
   puts = r9;
   puts = r13;

   
    r7 = reads(4);    			// Get samples
    r7 = pass r7;
    if le jump .terminate;		// Abort if samples <=0 
  
	i3 = r4; 					//input buffer address array
	i4 = r8; 					//output buffer address 
	i10 = r12;					//coeff buffer address array
	i12 = reads(1); 			//state buffer address array 
	i13 = reads(2); 			//stateSharingEnabled array
	i8  = reads(3); 			//taps  array
	m4  = 2;					//for SIMD address increments 
	r1  = 2;						
	r0=r7+r1; m1 =r0; 			//m1 = samples+2	
	
	#ifdef __ADI_GENERATED_DEF_HEADERS__
	   bit set mode1 (BITM_REGF_MMASK_BDCST9);   // Enter BDCST9 mode
	#else
	   bit set mode1 (BDCST9);
	#endif
	
	/* Iterate for all output buffers */
	r0 = reads(5);
	lcntr = r0, do .outer_loop until lce;

		r2 = r7;  						// Number of samples	
		r5 = pm(i8,m14);				//Number of taps 
		r0=dm(i3,m6); i1=r0; 			//Input buffer
		r0=pm(i10,m14); b9=r0;			//Coefficient buffer base and index
		l9=r5;							//Coefficient buffer length = taps 
		r0 = r5+r7; r0=r0-1; m2=r0;		//m2 = taps+block_size-1
		r5=r5-1;						//r5 = taps-1 
		
		/* Skip to different implementation for single tap filter */
		if eq jump .single_tap_fir;
		
		b0=pm(i12,m14);					//State buffer start address
		l0=m2;							//State buffer length = taps+block_size-1
		i5=b0;							//Initialize i5 with state buffer start address
		r0=dm(m2,i0);					//Read the location state[taps+samples-1] to get the state buffer index
		r0=pass r0;						
		if ne i0=r0; 					//Update i0 with the new state buffer index if non-zero
		
		/* Update the location taps+block_size-1 with 0xFFFFFFFF (NAN) to check whether it was overwritten with a valid float data while copying the input buffer */
		r12 = 0xFFFFFFFF;
		dm(m2, i5)=r12;
		
		r0=pm(i13, m14);				//Check if state sharing is enabled, if yes, skip state buffer update 	
		r0=pass r0;
		if ne jump .skip2;
		
		
			/* Update state buffer with new input block */
			r0 = lshift r7 by -1;
			r1 = lshift r0 by 1;
			r1 = r7 - r1;
			if le jump .skip1;			//If number of samples is even, skip first copy
				r1 = dm(i1,m6);
				dm(i0,m6)=r1;
				
			.skip1:
			r0 = pass r0;
			if eq jump .skip2;			//If number of samples is one, skip SIMD copy
			 
			ENTER_SIMD              	// Enable SIMD
			nop;                    	// One cycle effect latency
			
			lcntr = r0, do .copy_data until lce;
			r1=dm(i1,m4);  
		   .copy_data: dm(i0,m4)=r1;
		
   		.skip2:
        r6 = i0;  						//Store updated state index in r6 register
       
        EXIT_SIMD         				// Disable SIMD - has one cycle effect latency
        nop;
       
        /* If state[taps+samples-1] wasn't overwritten while input data copy, duplicate state[0] in state[taps+samples-1], so we can use circular buffers in SIMD */
        /* Else duplicate state[taps+samples-1] to state[0] */
        r0 = dm(m2, i5); /* read state[taps+samples-1] */
        r1 = dm(m5, i5); /* read state[0] */
        r12 = r12 - r0;  /* Check if  state[taps+samples-1] was overwritten while data copy */
        
        /* If yes, duplicate state[taps+samples-1] to state[0] */
        if ne dm(m5, i5) = r0; 
        
        /* Else duplicate state[0] in state[taps+samples-1] */
        if eq dm(m2, i5) = r1;
		
		 /* Loop over samples; this loop runs ceil(samples/4) times, R2 contains the number of samples remaining. */
		.samples_loop:
		
			ENTER_SIMD              	// Enable SIMD
			nop;                    	// One cycle effect latency
			
			/* Clear accumulators */
			r12 = r12 - r12;
			r13 = r13 - r13;
			r8 = r12 + r13, r9 = r12 - r13;
		
		    /* FIR MAC loop, process four samples at a time using SIMD */
			f0 = pm(i9,m14), f4 = dm(i0,m4);
			lcntr = r5, do .taps_end until lce;
				f8 = f0 * f4, f12 = f8 + f12, f4 = dm(i0,m7);
			.taps_end:
			    f9 = f0 * f4, f13 = f9 + f13, f4 = dm(i0,m4), f0 = pm(i9,m14);
			
			f8 = f0 * f4, f12 = f8 + f12, f4 = dm(i0,m1);
			f9 = f0 * f4, f13 = f9 + f13;
			f12 = f8 + f12;
			f13 = f9 + f13; 
			
			EXIT_SIMD         			// Disable SIMD - has one cycle effect latency
			nop;
			
			/* Write outputs */
		    r2 = r2 - 1;
		    if eq jump .samples_end (db);
		       dm(i4,m6) = r12; nop;
		    r2 = r2 - 1;
		    if eq jump .samples_end (db);
		       dm(i4,m6) = s12; nop; 
		    r2 = r2 - 1;
		    if eq jump .samples_end (db);
		       dm(i4,m6) = r13; nop; 
		    r2 = r2 - 1;
		    if ne jump .samples_loop (db);
		       dm(i4,m6) = s13; nop;
		       
		.samples_end: dm(m2, i5)=r6;  // Update the new state buffer index at location state[taps+samples-1]
		
	.outer_loop: nop;
	
	#ifdef __ADI_GENERATED_DEF_HEADERS__
	   bit clr mode1 (BITM_REGF_MMASK_BDCST9);   // Leave BDCST9 mode
	#else
	   bit clr mode1 (BDCST9);
	#endif

	.terminate:
	   i12=dm(m7,i6);       // Load return address
	
	   /* Restore registers */
	   i0  = reads(-2);
	   i1  = reads(-3);
	   i3  = reads(-4);
	   i5  = reads(-5);
	   m1  = reads(-6);
	   m2  = reads(-7);
	   b0  = reads(-8);
	   i8  = reads(-9);
	   i10 = reads(-10);
	   i9  = reads(-11);
	   b9  = reads(-12);
	   r5  = reads(-13);
	   r6  = reads(-14);
	   r7  = reads(-15);
	   r9  = reads(-16);
	   r13 = reads(-17);
	   
	   l0  = 0;
	   /* Return */
	   jump (m14,i12) (db);
	      l9 = 0;
	      rframe;

._fir_vec_mc.end:

	
.single_tap_fir:

	f0 = pm(i9,m14), f4 = dm(i1,m6);
	f12 = f0*f4, f0 = pm(i9,m14), f4 = dm(i1,m6);
	
	lcntr = r2, do .samples_loop1 until lce;
	f12 = f0*f4, dm(i4,m6) = f12;
	.samples_loop1: f0 = pm(i9,m14), f4 = dm(i1,m6); 
	
	jump .outer_loop;

/* End of file. */
