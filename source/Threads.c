// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <errno.h>

#include "BaconAPI/Internal/OperatingSystem.h"

#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <pthread.h>
#elif BA_OPERATINGSYSTEM_WINDOWS
#   include <Windows.h>
#endif

#include "BaconAPI/Threads.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BA_Thread BA_Thread_GetCurrent(void) {
#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
    return pthread_self();
#elif BA_OPERATINGSYSTEM_WINDOWS
    return GetCurrentThread();
#endif
}

BA_Boolean BA_Thread_Create(BA_Thread* thread, void (*threadFunction)(void)) {
#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
    int result = pthread_create(thread, NULL, (void* (*)(void*)) threadFunction, NULL);

    if (result != 0)
        errno = result;

    return result == 0;
#elif BA_OPERATINGSYSTEM_WINDOWS
    *thread = (BA_Thread) CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) threadFunction, NULL, 0, NULL);
    return *thread != NULL;
#endif
}

BA_Boolean BA_Thread_Join(BA_Thread thread) {
#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
    int result = pthread_join(thread, NULL);

    if (result != 0)
        errno = result;

    return result == 0;
#elif BA_OPERATINGSYSTEM_WINDOWS
    while (WaitForSingleObject(thread, INFINITE) != WAIT_OBJECT_0)
        continue;

    return BA_BOOLEAN_TRUE;
#endif
}

BA_Boolean BA_Thread_CreateLock(BA_Thread_Lock* lock) {
#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
    int result = pthread_mutex_init(lock, NULL);

    if (result != 0)
        errno = result;

    return result == 0;
#elif BA_OPERATINGSYSTEM_WINDOWS
    *lock = CreateMutex(NULL, FALSE, NULL);
    return *lock != NULL;
#endif
}

BA_Boolean BA_Thread_UseLock(BA_Thread_Lock* lock) {
#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
    int result = pthread_mutex_lock(lock);

    if (result != 0)
        errno = result;

    return result == 0;
#elif BA_OPERATINGSYSTEM_WINDOWS
    while (WaitForSingleObject(*lock, INFINITE) != WAIT_OBJECT_0)
        continue;

    return BA_BOOLEAN_TRUE;
#endif
}

BA_Boolean BA_Thread_Unlock(BA_Thread_Lock* lock) {
#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
    int result = pthread_mutex_unlock((pthread_mutex_t*) lock);

    if (result != 0)
        errno = result;

    return result == 0;
#elif BA_OPERATINGSYSTEM_WINDOWS
    return ReleaseMutex(*lock);
#endif
}

BA_Boolean BA_Thread_DestroyLock(BA_Thread_Lock* lock) {
#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
    int result = pthread_mutex_destroy((pthread_mutex_t*) lock);

    if (result != 0)
        errno = result;

    return result == 0;
#elif BA_OPERATINGSYSTEM_WINDOWS
    return CloseHandle(*lock);
#endif
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
