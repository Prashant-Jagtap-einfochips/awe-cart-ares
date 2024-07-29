/*
 * File: Model_Target_TSP_Capture.h
 *
 * Code generated for Simulink model 'Model_Target'.
 *
 * Model version                  : 1.12
 * Simulink Coder version         : 9.8 (R2022b) 13-May-2022
 * C/C++ source code generated on : Wed Dec 20 15:13:20 2023
 *
 * Target selection: ert_shrlib.tlc
 * Embedded hardware selection: Qualcomm->Hexagon
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#ifndef RTW_HEADER_Model_Target_TSP_Capture_h_
#define RTW_HEADER_Model_Target_TSP_Capture_h_
#include "ASDID.h"                     // for SimulinkDatatypeId
#include "Model_Target_TSP_IDs.h" // for TSP access message payload struct types

// ------------------------------------------------------------
// ACCESS ROUTINES TO GET TSP VALUES
// ------------------------------------------------------------
// "RTC" style TSP access handler
int Model_Target_HandleTspRequest(void* pReqBuf, uint32_t reqBufSize,
    void* pRspBuf, uint32_t rspBufSize);

// Capture TSP during step operations
void TSP_StepCapture(void* tablePtr, uint32_t tableIndex, const void
                     * sourceBuffer);

#endif                              /* RTW_HEADER_Model_Target_TSP_Capture_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
