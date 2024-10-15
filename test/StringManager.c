// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/ArgumentHandler.h>
#include <BaconAPI/StringManager.h>
#include <BaconAPI/Logger.h>
#include <BaconAPI/Debugging/Assert.h>

int main(int argc, char** argv) {
    BA_ArgumentHandler_Initialize(argc, argv);

    int string1 = BA_StringManager_Allocate("Hello, World!",  BA_BOOLEAN_FALSE);
    int string2 = BA_StringManager_Allocate(L"Hello, World!", BA_BOOLEAN_TRUE);

    BA_ASSERT(string1 != -1 && string2 != -1, "Failed to allocate strings\n");
    BA_LOGGER_INFO("Strings:\n"
                   "%i: %s\n"
                   "%i: %ls\n", string1, (const char*) BA_StringManager_Get(string1),
                                string2, (const wchar_t*) BA_StringManager_Get(string2));
    BA_StringManager_Replace(string2, "Goodbye, Moon!", BA_BOOLEAN_FALSE);
    BA_LOGGER_INFO("Strings:\n"
                   "%i: %s\n"
                   "%i: %s\n", string1, (const char*) BA_StringManager_Get(string1),
                               string2, (const char*) BA_StringManager_Get(string2));
    BA_StringManager_Deallocate(-1);
}
