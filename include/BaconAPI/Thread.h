// Purpose: Wrapper around OS specific threads
// Created on: 11/25/22 @ 1:54 PM

// Copyright (c) 2022, 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include "Internal/CPlusPlusSupport.h"
#include "Internal/Boolean.h"
#include "OperatingSystem.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <pthread.h>

typedef pthread_t BA_Thread;
typedef pthread_mutex_t BA_Thread_Lock;
#elif BA_OPERATINGSYSTEM_WINDOWS
#   include <Windows.h>

typedef HANDLE BA_Thread;
typedef HANDLE BA_Thread_Lock;
#endif

typedef void (*BA_Thread_Function)(void*);

BA_Thread BA_Thread_GetCurrent(void);
BA_Boolean BA_Thread_IsSingleThreaded(void);

/**
 * @return -1 means unlimited
 */
int BA_Thread_GetLimit(void);
int BA_Thread_GetAmount(void);

BA_Boolean BA_Thread_Create(BA_Thread* thread, BA_Thread_Function threadFunction, const char* name, void* argument);
BA_Boolean BA_Thread_Join(BA_Thread thread, void* returnValue);
BA_Boolean BA_Thread_CreateLock(BA_Thread_Lock* lock);
BA_Boolean BA_Thread_UseLock(BA_Thread_Lock* lock);
BA_Boolean BA_Thread_Unlock(BA_Thread_Lock* lock);
BA_Boolean BA_Thread_DestroyLock(BA_Thread_Lock* lock);

/**
 * @param limit -1 means unlimited
 */
void BA_Thread_SetLimit(int limit);

BA_Boolean BA_Thread_Kill(BA_Thread thread);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
