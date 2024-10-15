// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <string.h>
#include <BaconAPI/ArgumentHandler.h>
#include <BaconAPI/Debugging/Assert.h>
#include <BaconAPI/Thread.h>

#define NUMBER_OF_THREADS 5

static int lockNumber;
static BA_Thread_Lock lock;
static int createdThreads = 0;
static volatile BA_Boolean threadsWaiting = BA_BOOLEAN_TRUE;
static BA_Thread addedThreads[NUMBER_OF_THREADS];

void LockProtectedFunction() {
    BA_ASSERT(BA_Thread_UseLock(&lock), "Failed to use lock\n");
    
    lockNumber++;

    BA_ASSERT(lockNumber <= createdThreads, "Number is bigger than created threads. Locks are not working\n");
    BA_ASSERT(BA_Thread_Unlock(&lock), "Failed to unlock lock\n");
}

BA_THREAD_FUNCTION ThreadFunction(void* argument) {
    while (threadsWaiting) continue;

    LockProtectedFunction();
    return (BA_THREAD_RETURN_VALUE) NULL;
}

void Test(void) {
    if (BA_Thread_IsSingleThreaded()) {
        BA_Thread throwaway;
        
        BA_ASSERT(!BA_Thread_Create(&throwaway, (BA_THREAD_FUNCTION) &ThreadFunction, NULL), "Created a thread despite it being single-threaded\n");
        return;
    }

    BA_ASSERT(BA_Thread_CreateLock(&lock), "Failed to create lock\n");
    
    for (int i = 0; i < NUMBER_OF_THREADS; i++) {
        BA_Thread thread = 0;

        createdThreads++;

        BA_ASSERT(BA_Thread_Create(&thread, (BA_THREAD_FUNCTION) &ThreadFunction, NULL), "Failed to create thread %i", createdThreads);

        addedThreads[i] = thread;
    }

    BA_ASSERT(BA_Thread_GetAmount() == createdThreads, "Created threads amount is invalid\n");
    BA_Thread_SetLimit(NUMBER_OF_THREADS);

    {
        BA_Thread throwaway;

        BA_ASSERT(!BA_Thread_Create(&throwaway, (BA_THREAD_FUNCTION) &ThreadFunction, NULL), "Created a thread despite hitting the limit\n");
    }

    BA_ASSERT(BA_Thread_GetAmount() == createdThreads, "Still incremented the thread count\n");

    threadsWaiting = BA_BOOLEAN_FALSE;

    for (int i = 0; i < createdThreads; i++)
        BA_ASSERT(BA_Thread_Join(addedThreads[i], NULL), "Failed to join thread %i\n", i);

    BA_ASSERT(BA_Thread_DestroyLock(&lock), "Failed to destroy lock\n");
}
