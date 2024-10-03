/****************************************************
 *  BTF_Dll.h                                         
 *  Created on: 03-Mar-2023 8:55:11 AM                      
 *  Implementation of the Class BTF_Dll       
 *  Original author: cb1037694                     
 ****************************************************/

#if !defined(BTF_DLL_H_)
#define BTF_DLL_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "CART.h"
#include "BTF.h"
#include "BTF_Callback.h"

typedef BTF* (*BTF_Create_Fxn)(BTF* btf);
typedef BTF_Resource* (*BTF_Resource_Fxn)(BTF* btf);
typedef CART_STATUS (*BTF_Init_Fxn)(BTF* btf, void* arg);
typedef uint32_t (*BTF_Command_Fxn)(BTF* btf, CART_Message* req, CART_Message* rsp);
typedef uint32_t (*BTF_Exec_Fxn)(BTF* btf, uint32_t thread_level, void* arg);
typedef CART_Cmd_Ret (*BTF_Receive_Fxn)(BTF* btf, CART_Message* rsp);

typedef void (*BTF_Callback_Func_Post)(CART_EXEC_ID exec_id, void* arg);
typedef CART_Cmd_Ret (*BTF_Callback_Func_Notify)(CART_Message* status);
typedef CART_Cmd_Ret (*BTF_Callback_Func_Send)(CART_Message* req);

typedef void (*BTF_Callback_Register_Post_Fxn)(BTF_Callback_Func_Post funcptr);
typedef void (*BTF_Callback_Register_Notify_Fxn)(BTF_Callback_Func_Notify funcptr);
typedef void (*BTF_Callback_Register_Send_Fxn)(BTF_Callback_Func_Send funcptr);


#ifdef __cplusplus
}
#endif


#endif /*!defined(BTF_DLL_H_)*/
 