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
 * @file OJFeatureLib.h
 * @brief Include list of available features
 *
 * @details <OPTIONAL: Some details about file>
 *
 * @see <OPTIONAL: Links to reference/Another software component>
 *
 */




#ifndef __OJFEATURELIB_H__
#define __OJFEATURELIB_H__


/***  Include Files ***********************************************************/
#include "ModelRootInportConfigFeature.h"
#include "ModelRootOutportConfigFeature.h"




#define RTCID_COUNT             0
#define TOPID_COUNT             0


/***  Module Class  ***********************************************/
/**
 * @brief   : OJFeatureLib class
 */
class OJFeatureLib
{
public:
        /**
        * @brief              : TBD
        */
        RET_CODE_TYPE FL_Initialize_Features(void);

        /**
        * @brief              : TBD
        */
        ModelRootInportConfigFeature *GetModelRootInportConfigPtr() { return &mModelRootInportConfigFeature;}

        /**
        * @brief              : TBD
        */
        ModelRootOutportConfigFeature *GetModelRootOutportConfigPtr() { return &mModelRootOutportConfigFeature;}


protected:

private:
        /**
        * @brief         : ModelRootInportConfig Feature Instance
        */
        ModelRootInportConfigFeature mModelRootInportConfigFeature;

        /**
        * @brief         : ModelRootOutportConfig Feature Instance
        */
        ModelRootOutportConfigFeature mModelRootOutportConfigFeature;


};
/**
* @brief              : TBD
*/




#endif // OJFEATURELIB