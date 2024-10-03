/*******************************************************************************
*  BOSE CORPORATION
*  COPYRIGHT (c) BOSE CORPORATION ALL RIGHTS RESERVED
*  This program may not be reproduced, in whole or in part in any
*  form or any means whatsoever without the written permission of:
*     BOSE CORPORATION
*     The Mountain
*     Framingham, MA 01701-9168
*******************************************************************************/
/****************************************************
 * @file CMB.c
 *
 * @brief Implementation of the CART Message Bus (CMB).
 *
 *****************************************************/

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>

#include "CART.h"
#include "CART_STATUS.h"
#include "CMB_TYPES.h"
#include "CART_Message.h"
#include "_CMB.h"
#include "CMB_AdaptorInterface.h"
#include "CMB_Resource.h"
#include "CMB_Message_helper.h"

#define LOG_STR "CMB: "


#ifdef __cplusplus
extern "C"
{
#endif

_CMB CMB_obj;

CMB* CMB_get_handle()
{
    return (CMB*) &CMB_obj;
}

CMB*  CMB_create(const CMB_Config* cfg)
{
    if (0 == CMB_obj.created)
    {
        CMB_obj.created = 1;

        CMB_obj.id = cfg->id;

        CMB_obj.resource.maxAdaptors = cfg->maxNumOfAdaptors;

        CMB_obj.resource.memorySize = 0;

        CMB_obj.resource.memoryPointer = 0;

        CMB_obj.adaptors = 0;

        CMB_obj.adaptorCount = 0;

        CMB_obj.context = 0;

        CMB_obj.adaptorTable = 0;
    }

    return (CMB*) &CMB_obj;
}

void CMB_destroy(CMB* ccb)
{
    _CMB* this_private = (_CMB*) ccb;

    this_private->created = 0;
}

CMB_Resource* CMB_resource(CMB* ccb)
{
    _CMB* this_private = (_CMB*) ccb;

    const uint32_t totalInternalSize = (this_private->resource.maxAdaptors *
                                            sizeof(CMB_AdaptorInterface)) + 
                                        (MAX_NUM_OF_DESTINATIONS * sizeof(CMB_AdaptorInterface*));

    this_private->resource.memorySize = totalInternalSize; 

    return (CMB_Resource*) &(this_private->resource);
}

CART_STATUS CMB_init(CMB* ccb, void* arg)
{
    CART_STATUS retVal = CART_STATUS_SUCCESS;

    _CMB* this_private = (_CMB*) ccb;

    if (0 == ccb)
    {
        retVal = CART_STATUS_ERROR;
    }
    else if (0 == this_private->resource.memoryPointer)
    {
        retVal = CART_STATUS_INVALID;
    }
    else
    {
        uint8_t* buffer = (uint8_t*)this_private->resource.memoryPointer;

        this_private->adaptors = (CMB_AdaptorInterface*)buffer;

        buffer += (this_private->resource.maxAdaptors * sizeof(CMB_AdaptorInterface));

        this_private->adaptorTable = (CMB_AdaptorInterface**)buffer;

        memset((void*)this_private->adaptorTable, 0, MAX_NUM_OF_DESTINATIONS * sizeof(CMB_AdaptorInterface*));

        this_private->context = (CMB_Context*)arg;
    }

    return retVal;
}

CART_STATUS CMB_add_adaptor(CMB* ccb, CMB_AdaptorInterface* adaptor, CMB_Adaptor_Id* adaptorId)
{
    CART_STATUS retVal = CART_STATUS_SUCCESS;

    _CMB* this_private = (_CMB*) ccb;

    if ((0 == adaptor) || (0 == ccb))
    {
        retVal = CART_STATUS_ERROR;
    }
    else if (0 == this_private->resource.memoryPointer)
    {
        retVal = CART_STATUS_INVALID;
    }
    else if (this_private->adaptorCount >= this_private->resource.maxAdaptors)
    {
        retVal = CART_STATUS_QUEUE_FULL;
    }
    else
    {
        *adaptorId = this_private->adaptorCount;

        // Shallow copy.
        this_private->adaptors[this_private->adaptorCount] = *adaptor;

        this_private->adaptorCount++;
    }

    return retVal;
}


CART_STATUS CMB_initializeAdaptor(CMB* ccb, void* context)
{
    CART_STATUS retVal = CART_STATUS_SUCCESS;

    CMB_Adaptor_config* ioparams = (CMB_Adaptor_config*)context;

    _CMB* this_private = (_CMB*) ccb;

    if ((0 == ccb) || (0 == context))
    {
        retVal = CART_STATUS_ERROR;
    }
    else if (ioparams->id >= this_private->adaptorCount)
    {
        // Adaptor to which the proxy is requested to be added is invalid.
        retVal = CART_STATUS_INVALID;
    }
    else
    {
        retVal = this_private->adaptors[ioparams->id].initialize(ccb, ioparams->configuration);
    }

    return retVal;
}

#if 0
// Do not initialize all at once.
CART_STATUS CMB_init_adaptor(CMB* ccb, void* context)
{
    CART_STATUS retVal = CART_STATUS_SUCCESS;

    if (0 == ccb)
    {
        retVal = CART_STATUS_ERROR;
    }
    else
    {
        _CMB* this_private = (_CMB*) ccb;

        // Call each adaptor's initialization routine.
        for (uint8_t i = 0; i < this_private->adaptorCount; i++)
        {
            if (CART_STATUS_SUCCESS != this_private->adaptors[i].initialize(ccb, context))
            {
                retVal = CART_STATUS_ERROR;
            }
        }
    }

    return retVal;
}
#endif

CART_Cmd_Ret CMB_routeMessage(CMB* ccb, CART_Message* msg)
{
    CART_Cmd_Ret ret = {CART_STATUS_SUCCESS};

    if (0 == ccb)
    {
        ret.status = CART_STATUS_ERROR;
    }
    else if (0 == msg)
    {
        ret.status = CART_STATUS_INVALID;
    }
    else
    {
        _CMB* this_private = (_CMB*) ccb;

        const CART_ID dstId = CART_Message_get_dst(msg);

        uint8_t found = 0;

        // Verify that CMB has a valid adaptor for this destination. 
        if (0 != this_private->adaptorTable[dstId])
        {
            ret = this_private->adaptorTable[dstId]->sendCartMessage(ccb, msg);

            found = 1;
        }

        if (0 == found)
        {
            printf("CMB_routeMessage: Error routing msg to dst 0x%x. \n", dstId);

            ret.status = CART_STATUS_ERROR;

            ret.res_size = 0;
        }
    }

    return ret;
}

CART_STATUS CMB_deiniliatize(CMB* ccb, void* reserved)
{
    // Call each adaptor's deinitialization routine.
    CART_STATUS retVal = CART_STATUS_SUCCESS;

    _CMB* this_private = (_CMB*) ccb;

    if (0 == ccb)
    {
        retVal = CART_STATUS_ERROR;
    }
    else
    {
        // Call each adaptor's initialization routine.
        for (uint8_t i =0; i < this_private->adaptorCount; i++)
        {
            if (CART_STATUS_SUCCESS != this_private->adaptors[i].deinitialize(ccb, reserved))
            {
                retVal = CART_STATUS_ERROR;
            }
        }
    }

    return retVal;
}

CART_STATUS CMB_addProxy(CMB* ccb, CMB_Adaptor_proxyConfig* ioparams)
{
    // Call each adaptor's deinitialization routine.
    CART_STATUS retVal = CART_STATUS_SUCCESS;

    _CMB* this_private = (_CMB*) ccb;

    if (0 == ccb)
    {
        retVal = CART_STATUS_ERROR;
    }
    else if (ioparams->id >= this_private->adaptorCount)
    {
        // Adaptor to which the proxy is requested to be added is invalid.
        retVal = CART_STATUS_INVALID;
    }
    else if (ioparams->dstId > MAX_NUM_OF_DESTINATIONS)
    {
        // The CART ID of the proxy is out of range.
        retVal = CART_STATUS_OUTOFRANGE;
    }
    else
    {
        CART_Cmd_Ret ret = this_private->adaptors[ioparams->id].addProxy(ccb, ioparams->configuration);

        retVal = ret.status;

        if (CART_STATUS_SUCCESS == retVal)
        {
            for (int32_t i = ioparams->dstId; i <= ioparams->dstId + ioparams->cartIdMask; i++ )
            {
                this_private->adaptorTable[i] = &(this_private->adaptors[ioparams->id]);
            }
        }
    }

    return retVal;
}

CART_STATUS CMB_registerRecv(CMB* ccb, CMB_Adaptor_recvCallbackConfig* ioparams)
{
    // Call each adaptor's deinitialization routine.
    CART_STATUS retVal = CART_STATUS_SUCCESS;

    _CMB* this_private = (_CMB*) ccb;

    if (0 == ccb)
    {
        retVal = CART_STATUS_ERROR;
    }
    else if (ioparams->id >= this_private->adaptorCount)
    {
        retVal = CART_STATUS_OUTOFRANGE;
    }
    else if (0 == this_private->adaptors[ioparams->id].registerRecvCallback)
    {
        retVal = CART_STATUS_INVALID;
    }
    else
    {
        CART_Cmd_Ret ret = this_private->adaptors[ioparams->id].registerRecvCallback(ioparams->func);

        retVal = ret.status;
    }

    return retVal;
}

CART_STATUS CMB_registerSend(CMB* ccb, CMB_Adaptor_sendCallbackConfig* ioparams)
{
    // Call each adaptor's deinitialization routine.
    CART_STATUS retVal = CART_STATUS_SUCCESS;

    _CMB* this_private = (_CMB*) ccb;

    if (0 == ccb)
    {
        retVal = CART_STATUS_ERROR;
    }
    else if (ioparams->id >= this_private->adaptorCount)
    {
        retVal = CART_STATUS_OUTOFRANGE;
    }
    else if (0 == this_private->adaptors[ioparams->id].registerSendCallback)
    {
        retVal = CART_STATUS_INVALID;
    }
    else
    {
        CART_Cmd_Ret ret = this_private->adaptors[ioparams->id].registerSendCallback(ioparams->func);

        retVal = ret.status;
    }

    return retVal;
}

CART_Cmd_Ret CMB_command(CMB* ccb, const CMB_COMMAND msgtype, void* inoutparams)
{
    CART_Cmd_Ret ret = {CART_STATUS_SUCCESS};

    ret.res_size = 0;

    if (0 == ccb)
    {
        ret.status = CART_STATUS_INVALID;
    }
    else if ((msgtype < CMB_CMD_LAST) && (msgtype > CMB_CMD_FIRST))
    {
        switch (msgtype)
        {
            case CMB_CMD_INITIALIZE_ADAPTOR:
            {
                ret.status = CMB_initializeAdaptor(ccb, inoutparams);
                break;
            }

            case CMB_CMD_SEND_MSG:
            {
                CART_Message* msg = (CART_Message*)inoutparams;

                if (0 != CMB_Message_isDestinationValid(msg))
                {
                    ret = CMB_routeMessage(ccb, msg);
                }
                else
                {
#ifdef VERBOSE_LOGGING
                    const CART_ID dstId = CART_Message_get_dst(msg);

                    printf("Error routing msg to invalid dst 0x%x. \n", dstId);
#endif
                }

                break;
            }

            case CMB_CMD_ADD_PROXY:
            {
                ret.status = CMB_addProxy(ccb, (CMB_Adaptor_proxyConfig*)inoutparams);
                break;
            }

            case CMB_CMB_REGISTER_RECV_CALLBACK:
            {
                ret.status = CMB_registerRecv(ccb, (CMB_Adaptor_recvCallbackConfig*)inoutparams);
                break;
            }

            case CMB_CMB_REGISTER_SEND_CALLBACK:
            {
                ret.status = CMB_registerSend(ccb, (CMB_Adaptor_sendCallbackConfig*)inoutparams);
                break;
            }

            case CMB_CMD_DEINITIALIZE_ADAPTORS:
            {
                ret.status = CMB_deiniliatize(ccb, inoutparams);
                break;
            }

            default:
            {
                ret.status = CART_STATUS_INVALID;
                break;
            }
        }
    }
    else
    {
        ret.status = CART_STATUS_OUTOFRANGE;
    }

    return ret;
}

#ifdef __cplusplus
}
#endif

