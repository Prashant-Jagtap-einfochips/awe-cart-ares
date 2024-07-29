/*
 * File: BasicAudio_Target_TOP_Access.h
 *
 * Code generated for Simulink model 'BasicAudio_Target'.
 *
 * Model version                  : 7.2
 * Simulink Coder version         : 9.8 (R2022b) 13-May-2022
 * C/C++ source code generated on : Wed Jul 17 15:46:55 2024
 *
 * Target selection: ert_shrlib.tlc
 * Embedded hardware selection: Intel->x86-32 (Windows32)
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_BasicAudio_Target_TOP_Access_h_
#define RTW_HEADER_BasicAudio_Target_TOP_Access_h_

// ------------------------------------------------------------
// ACCESS ROUTINE TO GET AND SET TOP VALUES
// ------------------------------------------------------------
// See ASDID.h for returned ASDSTATUS codes.
int BasicAudio_Target_HandleTopRequest(void* pReqBuf, uint32_t reqBufSize,
    void* pRspBuf, uint32_t rspBufSize);

#endif                          /* RTW_HEADER_BasicAudio_Target_TOP_Access_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
