/*******************************************************************************
*  BOSE CORPORATION
*  COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
*  This program may not be reproduced, in whole or in part in any
*  form or any means whatsoever without the written permission of:
*     BOSE CORPORATION
*     The Mountain
*     Framingham, MA 01701-9168
******************************************************************************/

/**
 * @file OJFeatureLib.cpp
 * @brief Feature Control defines for enabled features *
 * @details <OPTIONAL: Some details about file>
 *
 * @see <OPTIONAL: Links to reference/Another software component>
 *
 */




/***  Include Files ***********************************************************/
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include "OJPAL.h"
#include "OJFeatureLib.h"



/*******************************************************************************
 *  OJFeatureLib - Public
*******************************************************************************/


RET_CODE_TYPE OJFeatureLib::FL_Initialize_Features(void)
{
    RET_CODE_TYPE retCode = RET_SUCCESS;

    mModelRootInportConfigFeature.SetInstanceNumber(0);
    mModelRootInportConfigFeature.BaseInitialize((const char*)"ModelRootInportConfig");
    mModelRootOutportConfigFeature.SetInstanceNumber(0);
    mModelRootOutportConfigFeature.BaseInitialize((const char*)"ModelRootOutportConfig");

    return retCode;
}

