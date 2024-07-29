#include <semaphore.h>
#include <pthread.h>

#include "CART_Message.h"

#define MAX_MSG_SIZE      2048
#define SHARED_MEM_NAME "/cartControl"

typedef struct
{
    sem_t initiateRequest;

    sem_t requestComplete;

    // CART_Phy_Message message will be appended contiguously. 
    // But it must be mapped separately in each process.
} shmem_t; // Rename to Module_Ipc_info


// Total size of shmem_t. Must be used in ftruncate() to size the mapped memory.
const uint32_t moduleIpcInfoSize = sizeof(shmem_t) + MAX_MSG_SIZE;
