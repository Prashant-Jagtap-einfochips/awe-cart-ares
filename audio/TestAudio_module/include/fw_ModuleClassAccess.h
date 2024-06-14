/*******************************************************************************
*
*               Audio Framework
*               ---------------
*
********************************************************************************
*     fw_ModuleClassAccess.h
********************************************************************************
*
*     Description:  AudioWeaver Framework heap header file
*
*     Copyright:    (c) 2007-2022 DSP Concepts, Inc. All rights reserved.
*                   3235 Kifer Road
*                   Santa Clara, CA 95054
*
*******************************************************************************/
#ifndef _FW_MODULECLASS_ACCESS_H
#define _FW_MODULECLASS_ACCESS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Call a module's set function if it has one.
 * @param [in] pModule				module pointer
 * @param [in] mask					mask to pass to set call
 * @return							0 or error code
 */
/*private*/ INT32 awe_fwSetCall(ModuleInstanceDescriptor *pModule, UINT32 mask);

/**
 * Call a module's get function if it has one.
 * @param [in] pModule				module pointer
 * @param [in] mask					mask to pass to set call
 * @return							0 or error code
 */
/*private*/ INT32 awe_fwGetCall(ModuleInstanceDescriptor *pModule, UINT32 mask);

/**
 * Call a module's set function if it has one.
 * @param [in] pModule				module pointer
 * @param [in] mask					mask to pass to set call
 * @return							0 or error code
 */
/*private*/ INT32 awe_fwSetCallModID(const AWEInstance *pAWE, UINT32 moduleID, UINT32 mask);

/**
 * Call a module's get function if it has one.
 * @param [in] pModule				module pointer
 * @param [in] mask					mask to pass to set call
 * @return							0 or error code
 */
/*private*/ INT32 awe_fwGetCallModID(const AWEInstance *pAWE, UINT32 moduleID, UINT32 mask);

/**
 * @brief Given an address in {objectID,index} form, compute the
 * address of the module member. Also return the module pointer.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [in] address				the member index
 * @param [out] pMod				pointer to receive module pointer
 * @param [out] retVal				0 or error code
 * @return							pointer to member
 */
/*private*/ UINT32 *awe_fwAddressOfMember(const AWEInstance *pAWE, UINT32 address, ModuleInstanceDescriptor **pMod, INT32 *retVal);

/**
 * @brief Set the value at the given address, then do set call.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [in] address				address in 20.12 {objectID,index} form
 * @param [in] offset				offset from address for object
 * @param [in] mask					get call mask (only used if the getCallFlag is 1)
 * @param [in] argsize				number of arguments that you are getting.  Set to 0 for a single scalar
 * @param [out] args 				pointer to array that will be filled with values
 * @param [in] floatFlg				flag that specifies if value(s) is\are float
 * @param [in] getCallFlag		    flag that sets if you want to call the module's get call function
 * @return							0 or error code
 */
INT32 awe_fwModuleClassGetCommon(const AWEInstance *pAWE, UINT32 address, UINT32 offset, UINT32 mask, UINT32 valSize, Sample *values, UINT32 floatFlag, UINT32 getCallFlag);

/**
 * @brief Set the value at the given address, then do set call.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [in] address				address in 20.12 {objectID,index} form
 * @param [in] offset				offset from address for object
 * @param [in] mask					set call mask
 * @param [in] argsize				number of arguments that you are setting.  Set to 0 for a single scalar
 * @param [in] args 				pointer to array (or single value) that has values for setting
 * @param [in] floatFlg				flag that specifies if value(s) is\are float
 * @param [in] setCallFlag		    flag that sets if you want to call the module's set call function
 * @return							0 or error code
 */
INT32 awe_fwModuleClassSetCommon(const AWEInstance *pAWE, UINT32 address, UINT32 offset, UINT32 mask, UINT32 valSize, const Sample *values, UINT32 floatFlag, UINT32 setCallFlag);

/**
 * @brief Set the given module to active=0, bypass=1, mute=2, inactive=other.
 * @param [in] pModule				nmodule to set state to
 * @param [in] state				state to set
 * @return							0 or error code
 */
/*private*/ INT32 awe_fwSetModuleState(ModuleInstanceDescriptor *pModule, INT32 status);

/**
 * @brief Get the given modules state: active=0, bypass=1, mute=2, inactive=3.
 * @param [in] pModule				module to get state from
 * @return							state, or -ve error code
 */
/*private*/ INT32 awe_fwGetModuleState(const ModuleInstanceDescriptor *pModule);

/**
 * @brief Assign a real pointer to a pointer type left hand side.
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [in] addressLhs			LHS address in 20.12 {objectID,index} form
 * @param [in] addressRhs			RHS address in 20.12 {objectID,index} form
 * @param [in] offsRhs				offset of right hand side
 * @return							0 or error code
 */
/*private*/ INT32 awe_fwSetPointer(const AWEInstance *pAWE, INT32 addressLhs, INT32 addressRhs, UINT32 offsRhs);

/**
 * @brief Dereference a pointer to a nested module
 * @param [in] pAWE					AWE instance pointer (this)
 * @param [in] address				address in 20.12 {objectID,index} form
 * @param [out] retVal				0 or error code
 * @return							ID of object pointed to, 0 on error
 */
/*private*/ INT32 awe_fwDerefPointer(const AWEInstance *pAWE, UINT32 address, INT32 *retVal);

#ifdef __cplusplus
}
#endif

#endif // _FW_MODULECLASS_ACCESS_H
