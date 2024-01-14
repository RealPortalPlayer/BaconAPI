// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/Thread.h>

#include "../TestHeader.h"

// FIXME: Test logs are not thread safe, so the position of logs will differ

#define NUMBER_OF_THREADS 5

static volatile BA_Boolean threadsWaiting = BA_BOOLEAN_TRUE;
static BA_Thread_Lock lock;
static int lockNumber;
static BA_Thread* addedThreads[NUMBER_OF_THREADS];
static int createdThreads = 0;

void LockProtectedFunction() {
    ASSERT(BA_Thread_UseLock(&lock));
    
    lockNumber++;

    ASSERT(lockNumber <= createdThreads);
    ASSERT(BA_Thread_Unlock(&lock));
}

BA_THREAD_FUNCTION ThreadFunction(void* argument) {
    while (threadsWaiting) continue;

    LockProtectedFunction();
    return (BA_THREAD_RETURN_VALUE) NULL;
}

int main(int argc, char** argv) {
    ASSERT(BA_Thread_CreateLock(&lock));
    printf("--- Testing API\n");
    
    {
        printf("--- BA_Thread_Create\n");

        for (int i = 0; i < NUMBER_OF_THREADS; i++) {
            BA_Thread* thread = malloc(sizeof(BA_Thread));

            createdThreads++;
            
            ASSERT(thread != NULL);
            ASSERT(BA_Thread_Create(thread, &ThreadFunction, NULL));
            
            addedThreads[i] = thread;
        }
    }

    {
        printf("--- BA_Thread_GetAmount\n");
        ASSERT(BA_Thread_GetAmount() == createdThreads);
    }
    
    {
        printf("--- BA_Thread_SetLimit\n");
        BA_Thread_SetLimit(NUMBER_OF_THREADS);
        ASSERT(!BA_Thread_Create(NULL, &ThreadFunction, NULL));
        ASSERT(BA_Thread_GetAmount() == NUMBER_OF_THREADS);
    }

    threadsWaiting = BA_BOOLEAN_FALSE;
    
    {
        printf("--- BA_Thread_Join\n");
        
        for (int i = 0; i < createdThreads; i++)
            ASSERT(BA_Thread_Join(*(addedThreads[i]), NULL));
    }

    ASSERT(BA_Thread_DestroyLock(&lock));
    printf("--- Success\n");
}