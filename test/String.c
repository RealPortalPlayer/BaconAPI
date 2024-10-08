// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/ArgumentHandler.h>
#include <BaconAPI/Debugging/Assert.h>
#include <BaconAPI/String.h>
#include <string.h>

#include "StringHelper.h"

int main(int argc, char** argv) {
    BA_ArgumentHandler_Initialize(argc, argv);
    STRING_HELPER_CREATE(String);
}
