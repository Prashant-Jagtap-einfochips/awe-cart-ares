/*
 * File: BasicAudio_InterpretationEngine64.c
 *
 * Code generated for Simulink model 'BasicAudio_InterpretationEngine64'.
 *
 * Model version                  : 7.2
 * Simulink Coder version         : 9.8 (R2022b) 13-May-2022
 * C/C++ source code generated on : Wed Jul 17 15:49:20 2024
 *
 * Target selection: ert_shrlib.tlc
 * Embedded hardware selection: Intel->x86-32 (Windows32)
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "BasicAudio_InterpretationEngine64.h"
#include "BasicAudio_InterpretationEngine64_private.h"
#include "rtwtypes.h"
#include "tsp_capture_settings.h"

/* Block signals (default storage) */
B_BasicAudio_InterpretationEngine64_T BasicAudio_InterpretationEngine64_B;

/* Real-time model */
static RT_MODEL_BasicAudio_InterpretationEngine64_T
    BasicAudio_InterpretationEngine64_M_;
RT_MODEL_BasicAudio_InterpretationEngine64_T *const
    BasicAudio_InterpretationEngine64_M = &BasicAudio_InterpretationEngine64_M_;
extern void BasicAudio_InterpretationEngine64_InitializeInterpretationRoot( void
    );

/* Output and update for function-call system: '<S3>/ifunc' */
void BasicAudio_InterpretationEngine64_ifunc(void)
{
    /* MATLAB Function: '<S2>/TrivialInterpretation' */
    /*  This function serves only to invoke a function trigger output */
    /* MATLAB Function 'InterpretTrigger/ifunc': '<S5>:1' */
    /* '<S5>:1:3' f(); */
    BasicAudio_InterpretationEngine64_B.State =
        BasicAudio_InterpretationEngine64_B.TSP;

    /*  Use one or more input Targe State Probes (TSP) or Captured State */
    /*  Constants to calculate an output state value that feeds into a StateVar */
    /*  block. Unlike tune translation code, this state interpretation code does */
    /*  not support variable size inputs - TSPs each have a fixed size. As a */
    /*  result, the input signals are just normal signal values that do *not* */
    /*  have .Value and .Size fields. No need for a custom bus! */
    /* MATLAB Function 'Empty/TrivialInterpretation': '<S4>:1' */
    /*  ----- To register an error, use the following method (works like printf): */
    /*    InterpretError( <format string>, <arguments ... ) */
    /*    ... this error will cause the build process to abort. */
    /*  A trivial pass-through interpretation looks like this: */
    /* '<S4>:1:16' State = TSP1; */
}

/* Model step function */
void BasicAudio_InterpretationEngine64_step(void)
{
    /* (no output/update code required) */
}

/* Model initialize function */
void BasicAudio_InterpretationEngine64_initialize(void)
{
    /* Registration code */

    /* initialize error status */
    rtmSetErrorStatus(BasicAudio_InterpretationEngine64_M, (NULL));

    /* block I/O */
    (void) memset(((void *) &BasicAudio_InterpretationEngine64_B), 0,
                  sizeof(B_BasicAudio_InterpretationEngine64_T));

    /* user code (Initialize function Body) */

    // Register interpretation engine among the model roots
    BasicAudio_InterpretationEngine64_InitializeInterpretationRoot();
}

/* Model terminate function */
void BasicAudio_InterpretationEngine64_terminate(void)
{
    /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
