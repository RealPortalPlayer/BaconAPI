// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/ArgumentHandler.h>
#include <BaconAPI/Internal/Compiler.h>
#include <BaconAPI/Logger.h>

int main(int argc, char** argv) {
    BA_ArgumentHandler_Initialize(argc, argv);
    BA_LOGGER_INFO("Compiler: %s %s\n"
                   "Standard Version: %i\n", BA_COMPILER_NAME, BA_COMPILER_VERSION, BA_COMPILER_STANDARD_VERSION);
}
