// Copyright (c) 2022, 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "BaconAPI/OperatingSystem.h"

#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <pthread.h>
#   define BA_THREAD_INTERNAL_RETURN_TYPE void*
#elif BA_OPERATINGSYSTEM_WINDOWS
#   include <Windows.h>
#   define BA_THREAD_INTERNAL_RETURN_TYPE DWORD
#endif

#include "BaconAPI/Thread.h"
#include "BaconAPI/ArgumentHandler.h"
#include "BaconAPI/BuiltInArguments.h"

// TODO: Error handling sucks right now
// TODO: Should we disable creation of locks if multi-threading is disabled?

BA_CPLUSPLUS_SUPPORT_GUARD_START()
static int baThreadLimit = -1;
static int baThreadCreated = 0;

BA_Thread BA_Thread_GetCurrent(void) {
#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
    return pthread_self();
#elif BA_OPERATINGSYSTEM_WINDOWS
    return GetCurrentThread();
#endif
}

BA_Boolean BA_Thread_Create(BA_Thread* thread, BA_Thread_Function threadFunction, void* argument) {
#ifndef BA_SINGLE_THREADED
    if (baThreadLimit == baThreadCreated || BA_Thread_IsSingleThreaded())
        return BA_BOOLEAN_FALSE;

#   if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
    if (pthread_create(thread, NULL, threadFunction, &argument) != 0)
        return BA_BOOLEAN_FALSE;
#   elif BA_OPERATINGSYSTEM_WINDOWS
    *thread = (BA_Thread) CreateThread(NULL, 0, threadFunction, &argument, 0, NULL);
    
    if (*thread == NULL)
        return BA_BOOLEAN_FALSE;
#   endif

    baThreadCreated++;
    return BA_BOOLEAN_TRUE;
#else
    return BA_BOOLEAN_FALSE;
#endif
}

BA_Boolean BA_Thread_Join(BA_Thread thread, void* returnValue) {
#ifndef BA_SINGLE_THREADED
    if (BA_Thread_IsSingleThreaded())
        return BA_BOOLEAN_FALSE;
    
#   if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
    if (pthread_join(thread, returnValue) != 0)
        return BA_BOOLEAN_FALSE;
#   elif BA_OPERATINGSYSTEM_WINDOWS
    if (WaitForSingleObject(thread, INFINITE) != WAIT_OBJECT_0)
        return BA_BOOLEAN_FALSE;

    GetExitCodeThread(thread, returnValue);
#   endif

    baThreadCreated--;
    return BA_BOOLEAN_TRUE;
#else
    return BA_BOOLEAN_FALSE;
#endif
}

BA_Boolean BA_Thread_CreateLock(BA_Thread_Lock* lock) {
#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
    return pthread_mutex_init(lock, NULL) == 0;
#elif BA_OPERATINGSYSTEM_WINDOWS
    *lock = CreateMutex(NULL, FALSE, NULL);
    return *lock != NULL;
#endif
}

BA_Boolean BA_Thread_UseLock(BA_Thread_Lock* lock) {
#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
    return pthread_mutex_lock(lock) == 0;
#elif BA_OPERATINGSYSTEM_WINDOWS
    return WaitForSingleObject(*lock, INFINITE) == WAIT_OBJECT_0;
#endif
}

BA_Boolean BA_Thread_Unlock(BA_Thread_Lock* lock) {
#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
    return pthread_mutex_unlock((pthread_mutex_t*) lock) == 0;
#elif BA_OPERATINGSYSTEM_WINDOWS
    return ReleaseMutex(*lock) != 0;
#endif
}

BA_Boolean BA_Thread_DestroyLock(BA_Thread_Lock* lock) {
#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
    return pthread_mutex_destroy((pthread_mutex_t*) lock) == 0;
#elif BA_OPERATINGSYSTEM_WINDOWS
    return CloseHandle(*lock) != 0;
#endif
}

int BA_Thread_GetLimit(void) {
    return baThreadLimit;
}

void BA_Thread_SetLimit(int limit) {
    baThreadLimit = limit;
}

int BA_Thread_GetAmount(void) {
    return baThreadCreated;
}

BA_Boolean BA_Thread_Kill(BA_Thread thread) {
#ifndef BA_SINGLE_THREADED
    if (BA_Thread_IsSingleThreaded())
        return BA_BOOLEAN_FALSE;
    
#   if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
    return pthread_cancel(thread) == 0;
#   else
    return TerminateThread(thread, 0) != 0;
#   endif
#else
    return BA_BOOLEAN_FALSE;
#endif
}

BA_Boolean BA_Thread_IsSingleThreaded(void) {
#ifndef BA_SINGLE_THREADED
    static int enabled = -1;

    if (enabled == -1)
        enabled = BA_ArgumentHandler_ContainsArgumentOrShort(BA_BUILTINARGUMENTS_SINGLE_THREADED, BA_BUILTINARGUMENTS_SINGLE_THREADED_SHORT, BA_BOOLEAN_FALSE);

    return enabled;
#else
    return BA_BOOLEAN_TRUE;
#endif
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
