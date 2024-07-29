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
 * @file OJModelRootOutportConfigFeature.h
 * @brief Include list of available features
 *
 * @details <OPTIONAL: Some details about file>
 *
 * @see <OPTIONAL: Links to reference/Another software component>
 *
 */




#ifndef __OJMODELROOTOUTPORTCONFIGFEATURE_H__
#define __OJMODELROOTOUTPORTCONFIGFEATURE_H__


/***  Include Files ***********************************************************/
#include "Feature.h"







/***  Module Class  ***********************************************/
/**
 * @brief   : OJModelRootOutportConfigFeature class
 */
class OJModelRootOutportConfigFeature : public Feature
{
public:
        /**
        * @brief              : TBD
        */
        virtual RET_CODE_TYPE OJInitialize();


protected:

private:
        /**
        * @brief         : Number of Channel In
        */
        int num_in;

        /**
        * @brief         : Number of Channel out
        */
        int num_out;


};




#endif // OJMODELROOTOUTPORTCONFIGFEATURE