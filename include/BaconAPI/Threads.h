// Purpose: Wrapper around OS specific threads
// Created on: 11/25/22 @ 1:54 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include "Internal/CPlusPlusSupport.h"
#include "Internal/Boolean.h"
#include "Internal/OperatingSystem.h"

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

BA_Thread BA_Thread_GetCurrent(void);

BA_Boolean BA_Thread_Create(BA_Thread* thread, void (*threadFunction)(void));
BA_Boolean BA_Thread_Join(BA_Thread thread);
BA_Boolean BA_Thread_CreateLock(BA_Thread_Lock* lock);
BA_Boolean BA_Thread_UseLock(BA_Thread_Lock* lock);
BA_Boolean BA_Thread_Unlock(BA_Thread_Lock* lock);
BA_Boolean BA_Thread_DestroyLock(BA_Thread_Lock* lock);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
