// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/Translations.h>
#include <BaconAPI/Logger.h>
#include <BaconAPI/StringManager.h>
#include <BaconAPI/Debugging/Assert.h>
#include <BaconAPI/String.h>

BA_TRANSLATIONS_PRELOAD(GetTest1String, "test.hello")
BA_TRANSLATIONS_PRELOAD(GetTest2String, "test.hello2")

#define TEST(u) \
BA_Translations_LoadLanguage("test", "test.hello=Colo" u "r me surprised!\ntest.hello2=Colo" u "ring?"); \
BA_ASSERT(BA_String_Equals(GetTest1String(), "Colo" u "r me surprised!", BA_BOOLEAN_FALSE), "Wrong translation\n"); \
BA_ASSERT(BA_String_Equals(GetTest2String(), "Colo" u "ring?", BA_BOOLEAN_FALSE), "Wrong translation\n")

void Test(void) {
    TEST("");
    TEST("u");
    BA_StringManager_Deallocate(-1);
    BA_Translations_Destroy();
}