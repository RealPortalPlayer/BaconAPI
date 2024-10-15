// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/ArgumentHandler.h>
#include <BaconAPI/OperatingSystem.h>
#include <BaconAPI/Logger.h>
#include <stdlib.h>

void Test(void) {
    BA_OperatingSystem_Version version;

    BA_OperatingSystem_GetVersion(&version);
    BA_LOGGER_INFO("OS: %s %s %s (internally %s)\n", version.systemName, version.release, version.is64bits ? "x64" : "x86", BA_OPERATINGSYSTEM_NAME);
    BA_OperatingSystem_Free(&version);
}
