// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <string.h>
#include <BaconAPI/Debugging/StaticAssert.h>
#include <BaconAPI/ArgumentHandler.h>
#include <BaconAPI/Debugging/Assert.h>
#include <BaconAPI/BuiltInArguments.h>

#define FAKE_ARGUMENTS_AMOUNT 5

#define ARGUMENT1 "--test1"
#define ARGUMENT2 "--test2"

int main(void) {
    char* fakeArguments[] = {
        "throwaway", // argv[0] is the executable name/path. ArgumentHandler skips this
        ARGUMENT1, "value",
        BA_BUILTINARGUMENTS_DONT_PARSE, ARGUMENT2
    };

    BA_STATIC_ASSERT_LOOKUP_TABLE_CHECK(fakeArguments, FAKE_ARGUMENTS_AMOUNT);
    BA_ArgumentHandler_Initialize(FAKE_ARGUMENTS_AMOUNT, fakeArguments);
    BA_ASSERT(BA_ArgumentHandler_GetIndex("should not find this", BA_BOOLEAN_FALSE) == -1, "Argument found despite not being in the original argv\n");
    BA_ASSERT(BA_ArgumentHandler_GetIndex(ARGUMENT1, BA_BOOLEAN_FALSE) == 1, "Wrong index returned\n");
    BA_ASSERT(BA_ArgumentHandler_GetIndex(ARGUMENT2, BA_BOOLEAN_FALSE) == -1, "Argument found despite it being after the don't parse argument\n");
    BA_ASSERT(BA_ArgumentHandler_GetValue("should not find this", BA_BOOLEAN_FALSE) == NULL, "Found a value to a argument that doens't exist\n");
    BA_ASSERT(strcmp(BA_ArgumentHandler_GetValue(ARGUMENT1, BA_BOOLEAN_FALSE), "value") == 0, "Invalid value returned\n");
}
