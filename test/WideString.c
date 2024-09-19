// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <locale.h>
#include <BaconAPI/ArgumentHandler.h>
#include <BaconAPI/WideString.h>
#include <stdio.h>
#include <stdlib.h>
#include <BaconAPI/StringSafeFormat.h>

#include "StringHelper.h"

int main(int argc, char** argv) {
    BA_ArgumentHandler_Initialize(argc, argv);
    STRING_HELPER_CREATE(WideString);
}
