// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/ArgumentHandler.h>
#include <BaconAPI/Number.h>
#include <BaconAPI/Debugging/Assert.h>
#include <string.h>

#define NUMBER_FUNCTION(to, string, result, error, endPointerResult) \
do {                                                                 \
    char* endPointer;                                                \
    BA_Boolean isError;                                              \
    BA_ASSERT(BA_Number_StringTo ## to(string, &endPointer, &isError, NULL, 0) == result, "Results did not match expected"); \
    BA_ASSERT(isError == error, error ? "Didn't error" : "Errored"); \
    BA_ASSERT(strcmp(endPointer, endPointerResult) == 0, "End pointer does not match expected\n"); \
} while (BA_BOOLEAN_FALSE)
#define NUMBER_FUNCTION_TEST(to) \
do {                             \
        NUMBER_FUNCTION(to, "1", 1, BA_BOOLEAN_FALSE, ""); \
        NUMBER_FUNCTION(to, "test", 0, BA_BOOLEAN_TRUE, "test"); \
        NUMBER_FUNCTION(to, "1test", 0, BA_BOOLEAN_TRUE, "test"); \
} while (0)

void Test(void) {
    NUMBER_FUNCTION_TEST(Character);
    NUMBER_FUNCTION_TEST(UnsignedCharacter);
    NUMBER_FUNCTION_TEST(Short);
    NUMBER_FUNCTION_TEST(UnsignedShort);
    NUMBER_FUNCTION_TEST(Integer);
    NUMBER_FUNCTION_TEST(Unsigned);
    NUMBER_FUNCTION_TEST(Long);
    NUMBER_FUNCTION_TEST(UnsignedLong);
    NUMBER_FUNCTION_TEST(LongLong);
    NUMBER_FUNCTION_TEST(UnsignedLongLong);
}
