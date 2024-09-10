// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "BaconAPI/Memory.h"
#include "BaconAPI/Internal/Boolean.h"
#include "BaconAPI/Debugging/Assert.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
extern BA_Memory_TypeData baMemoryLookupTable[];

static BA_Memory_TypeData* BA_Memory_GetMemoryTypeInformation(BA_MEMORY_INFORMATION_ENUM_NAME memoryType) {
    BA_ASSERT(memoryType < BA_MEMORY_INFORMATION_ENUM_SIZE, "This shouldn't happen\n");
    return &baMemoryLookupTable[memoryType];
}

const BA_Memory_TypeData* BA_Memory_Get(void) {
    return baMemoryLookupTable;
}

size_t BA_Memory_GetAllocatedBytes(void) {
    size_t allocatedBytes = 0;
    
    for (int i = 0; i < BA_MEMORY_INFORMATION_ENUM_SIZE; i++)
        allocatedBytes += baMemoryLookupTable[i].allocatedBytes;
    
    return allocatedBytes;
}

size_t BA_Memory_GetAllocatedAmount(void) {
    size_t finalAmount = 0;

    for (int i = 0; i < BA_MEMORY_INFORMATION_ENUM_SIZE; i++)
        finalAmount += baMemoryLookupTable[i].allocatedAmount;

    return finalAmount;
}

void* BA_Memory_Allocate(size_t size, BA_MEMORY_INFORMATION_ENUM_NAME memoryType) {
    void* pointer;
    BA_Memory_TypeData* memoryTypeInformation = BA_Memory_GetMemoryTypeInformation(memoryType);

    BA_LOGGER_TRACE("Allocating memory\n"
                    "Size: %zu\n"
                    "Type: %i\n", size, memoryType);
    BA_ASSERT(size > 0, "Size cannot be zero or below\n");
    BA_ASSERT((pointer = malloc(size)) != NULL, "Failed to allocate %zu bytes of data\n", size);

    memoryTypeInformation->allocatedBytes += size;
    memoryTypeInformation->allocatedAmount++;
    return pointer;
}

void* BA_Memory_Reallocate(void* pointer, size_t oldSize, size_t newSize, BA_MEMORY_INFORMATION_ENUM_NAME memoryType) {
    void* newPointer;
    BA_Memory_TypeData* memoryTypeInformation = BA_Memory_GetMemoryTypeInformation(memoryType);

    BA_LOGGER_TRACE("Reallocating memory\n"
                    "Old Size: %zu\n"
                    "New Size: %zu\n"
                    "Type: %i\n", oldSize, newSize, memoryType);
    BA_ASSERT(pointer != NULL, "Pointer cannot be null\n");
    BA_ASSERT(oldSize > 0 && newSize > 0, "Size cannot be zero or below\n");
    BA_ASSERT((newPointer = realloc(pointer, newSize)) != NULL, "Failed to reallocate %zu bytes of data\n", newSize);

    memoryTypeInformation->allocatedBytes = memoryTypeInformation->allocatedBytes - oldSize + newSize;
    return newPointer;
}

void* BA_Memory_Deallocate(void* pointer, size_t oldSize, BA_MEMORY_INFORMATION_ENUM_NAME memoryType) {
    BA_Memory_TypeData* memoryTypeInformation = BA_Memory_GetMemoryTypeInformation(memoryType);

    BA_LOGGER_TRACE("Deallocating memory\n"
                    "Size: %zu\n"
                    "Type: %i\n", oldSize, memoryType);
    BA_ASSERT(pointer != NULL, "Pointer cannot be null\n");
    BA_ASSERT(oldSize > 0, "Size cannot be zero or below\n");
    free(pointer);

    memoryTypeInformation->allocatedBytes -= oldSize;
    memoryTypeInformation->allocatedAmount--;
}

void BA_Memory_AddSize(size_t size, BA_MEMORY_INFORMATION_ENUM_NAME memoryType) {
    BA_LOGGER_TRACE("Adding to allocated bytes\n"
                    "Size: %zu\n"
                    "Type: %i\n", size, memoryType);

    BA_Memory_GetMemoryTypeInformation(memoryType)->allocatedBytes += size;
}

void BA_Memory_RemoveSize(size_t size, BA_MEMORY_INFORMATION_ENUM_NAME memoryType) {
    BA_LOGGER_TRACE("Removing from allocated bytes\n"
                    "Size: %zu\n"
                    "Type: %i\n", size, memoryType);

    BA_Memory_GetMemoryTypeInformation(memoryType)->allocatedBytes -= size;
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
