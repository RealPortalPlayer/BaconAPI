// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <string.h>
#include <BaconAPI/ArgumentHandler.h>
#include <BaconAPI/ANSI.h>

#include "BaconAPI/Debugging/Assert.h"

void Test(void) {
    BA_ASSERT(BA_ANSI_ConvertCodeToString(BA_ANSI_CODE_SIZE)[0] == '\0', "Invalid code didn't return an empty string\n");
    
    if (!BA_ANSI_IsEnabled()) {
        BA_ASSERT(BA_ANSI_ConvertCodeToString(BA_ANSI_CODE_BOLD)[0] == '\0', "Valid code didn't return an empty string, despite ANSI being disabled\n");
        return;
    }

    BA_ASSERT(BA_ANSI_ConvertCodeToString(BA_ANSI_CODE_BOLD)[0] != '\0', "Returned an empty string\n");
}
