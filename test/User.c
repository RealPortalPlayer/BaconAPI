// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include <BaconAPI/ArgumentHandler.h>
#include <BaconAPI/Logger.h>
#include <BaconAPI/User.h>

void Test(void) {
    BA_LOGGER_INFO("User information:\n"
                   "Username: %s\n"
                   "Is administrator: %i\n", BA_User_GetName(), BA_User_IsAdministrator());
}
