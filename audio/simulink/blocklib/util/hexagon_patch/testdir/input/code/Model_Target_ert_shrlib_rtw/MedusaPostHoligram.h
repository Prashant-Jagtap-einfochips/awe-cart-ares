/*
 * File: MedusaPostHoligram.h
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

#ifndef RTW_HEADER_MedusaPostHoligram_h_
#define RTW_HEADER_MedusaPostHoligram_h_
#ifndef Model_Target_COMMON_INCLUDES_
#define Model_Target_COMMON_INCLUDES_
#include <stdlib.h>
#include <stdio.h>
#include "rtwtypes.h"
#include "Model_Target_RTC_IDs.h"
#include "Model_Target_TalariaAPI.h"
#include "Model_Target_TSP_Capture.h"
#include "Model_Target_StructStaticInit.h"
#include "Model_Target_TOP.h"
#endif                                 /* Model_Target_COMMON_INCLUDES_ */

/* Block signals for system '<S1138>/MedusaPostHoligram' */
typedef struct
{
    real32_T y_c[768];                 /* '<S1791>/Router' */
    real32_T ReconstructionFilter_e[704];/* '<S1791>/Reconstruction Filter' */
    real32_T MatrixConcatenate[176];   /* '<S1788>/Matrix Concatenate' */
    real32_T Merge[768];               /* '<S1788>/Merge' */
}
B_MedusaPostHoligram_Model_Target_T;

/* Block states (default storage) for system '<S1138>/MedusaPostHoligram' */
typedef struct
{
    real32_T ReconstructionFilter_TapDelayBuff[748];/* '<S1791>/Reconstruction Filter' */
    uint32_T MedusaPostHoligramEnable; /* '<S1788>/Data Store Memory' */
}
DW_MedusaPostHoligram_Model_Target_T;

/* Extern declarations of internal data for system '<S1138>/MedusaPostHoligram' */
extern B_MedusaPostHoligram_Model_Target_T Model_Target_MedusaPostHoligram_B;
extern DW_MedusaPostHoligram_Model_Target_T Model_Target_MedusaPostHoligram_DW;
extern void HandleReqPreAmpMedusaPostHoligramDisable(void);
extern void Model_Target_MedusaPostHoligram_Init(void);
extern void Model_Target_MedusaPostHoligram(void);

#endif                                 /* RTW_HEADER_MedusaPostHoligram_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
