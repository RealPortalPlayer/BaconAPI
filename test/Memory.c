// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#define BA_MEMORY_INFORMATION_ENUM_NAME MemoryTypes
#define BA_MEMORY_INFORMATION_ENUM_SIZE MEMORY_TYPE_SIZE

#include <BaconAPI/Debugging/StaticAssert.h>
#include <BaconAPI/ArgumentHandler.h>
#include <BaconAPI/String.h>
#include <BaconAPI/Memory.h>

#include "BaconAPI/Debugging/Assert.h"

#define PRINT_MEMORY_INFORMATION() \
memoryInformation = BA_Memory_GetAllocatedInformation(""); \
BA_LOGGER_INFO("%s\n", memoryInformation); \
free(memoryInformation)

typedef enum {
    MEMORY_TYPE_TEST1,
    MEMORY_TYPE_TEST2,

    MEMORY_TYPE_SIZE
} MemoryTypes;

BA_Memory_TypeData baMemoryLookupTable[] = {
    {0, 0},
    {0, 0}
};

BA_STATIC_ASSERT_LOOKUP_TABLE_CHECK(baMemoryLookupTable, MEMORY_TYPE_SIZE);

size_t BA_Memory_GetEnumeratorSize(void) {
    return MEMORY_TYPE_SIZE;
}

char* BA_Memory_GetAllocatedInformation(const char* prefix) {
    char* finalString;
    
    BA_MEMORY_CREATE_INFORMATION_STRING(finalString, BA_MEMORY_DEFINE_INFORMATION_STRING_TEMPLATE("Test1") "\n"
                                                     BA_MEMORY_DEFINE_INFORMATION_STRING_TEMPLATE("Test2"));
    BA_MEMORY_FORMAT_INFORMATION_STRING(finalString,
                                        BA_MEMORY_GET_MEMORY_INFORMATION(MEMORY_TYPE_TEST1),
                                        BA_MEMORY_GET_MEMORY_INFORMATION(MEMORY_TYPE_TEST1));
    return finalString;
}

int main(int argc, char** argv) {
    BA_ArgumentHandler_Initialize(argc, argv);

    char* memoryInformation;

    PRINT_MEMORY_INFORMATION();

    void* test1 = BA_Memory_Allocate(sizeof(void*), MEMORY_TYPE_TEST1);

    PRINT_MEMORY_INFORMATION();
    BA_ASSERT(baMemoryLookupTable[MEMORY_TYPE_TEST1].allocatedBytes == sizeof(void*), "Invalid allocated bytes\n");
    BA_ASSERT(baMemoryLookupTable[MEMORY_TYPE_TEST1].allocatedAmount == 1, "Invalid allocated amount\n");

    test1 = BA_Memory_Reallocate(test1, sizeof(void*), sizeof(void*) * 2, MEMORY_TYPE_TEST1);

    PRINT_MEMORY_INFORMATION();
    BA_ASSERT(baMemoryLookupTable[MEMORY_TYPE_TEST1].allocatedBytes == sizeof(void*) * 2, "Invalid allocated bytes\n");

    BA_Memory_Deallocate(test1, sizeof(void*) * 2, MEMORY_TYPE_TEST1);

    PRINT_MEMORY_INFORMATION();
    BA_ASSERT(baMemoryLookupTable[MEMORY_TYPE_TEST1].allocatedBytes == 0, "Invalid allocated bytes\n");
    BA_ASSERT(baMemoryLookupTable[MEMORY_TYPE_TEST1].allocatedAmount == 0, "Invalid allocated amount\n");
}