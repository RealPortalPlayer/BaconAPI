// Purpose: Memory management
// Created on: 9/9/24 @ 11:44 PM

// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <stddef.h>

#include "Internal/CPlusPlusSupport.h"
#include "Internal/Boolean.h"
#include "String.h"
#include "Debugging/Assert.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
typedef struct {
    size_t allocatedBytes;
    size_t allocatedAmount;
} BA_Memory_TypeData;

const BA_Memory_TypeData* BA_Memory_Get(void);
size_t BA_Memory_GetAllocatedBytes(void);
size_t BA_Memory_GetAllocatedAmount(void);
size_t BA_Memory_GetEnumeratorSize(void);

char* BA_Memory_GetAllocatedInformation(const char* prefix);
void* BA_Memory_Allocate(size_t size, int memoryType);
void* BA_Memory_Reallocate(void* pointer, size_t oldSize, size_t newSize, int memoryType);
void BA_Memory_Deallocate(void* pointer, size_t oldSize, int memoryType);
void BA_Memory_AddSize(size_t size, int memoryType);
void BA_Memory_RemoveSize(size_t size, int memoryType);
BA_CPLUSPLUS_SUPPORT_GUARD_END()

#define BA_MEMORY_GET_MEMORY_INFORMATION(memoryType) prefix, baMemoryLookupTable[memoryType].allocatedAmount, baMemoryLookupTable[memoryType].allocatedBytes

#define BA_MEMORY_DEFINE_INFORMATION_STRING_TEMPLATE(name) "%s" name ": %zu allocated, %zu bytes"
#define BA_MEMORY_CREATE_INFORMATION_STRING(variable, value) \
do {                                                         \
    variable = BA_String_Copy(value);                        \
    BA_ASSERT(variable != NULL, "Failed to allocate memory for a string\n"); \
} while (BA_BOOLEAN_FALSE)
#define BA_MEMORY_FORMAT_INFORMATION_STRING(variable, ...) BA_ASSERT(BA_String_Format(&variable, __VA_ARGS__), "Failed to format string\n")
