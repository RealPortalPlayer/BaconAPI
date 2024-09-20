// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/ArgumentHandler.h>
#include <BaconAPI/Logger.h>
#include <BaconAPI/Internal/Architecture.h>

int main(int argc, char** argv) {
    BA_ArgumentHandler_Initialize(argc, argv);
    BA_LOGGER_INFO("%s-%i (%i)\n", BA_ARCHITECTURE_TYPE_NAME, BA_ARCHITECTURE, BA_ARCHITECTURE_TYPE);
}
  