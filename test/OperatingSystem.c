// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/ArgumentHandler.h>
#include <BaconAPI/OperatingSystem.h>
#include <BaconAPI/Logger.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    BA_ArgumentHandler_Initialize(argc, argv);

    BA_OperatingSystem_Version version;

    BA_OperatingSystem_GetVersion(&version);
    
    BA_LOGGER_INFO("OS: %s %s (internally %s)\n", version.systemName, version.release, BA_OPERATINGSYSTEM_NAME);
    free(version.systemName);
    free(version.release);
}
