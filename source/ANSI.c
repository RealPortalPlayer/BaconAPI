// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "BaconAPI/Internal/CPlusPlusSupport.h"
#include "BaconAPI/ANSI.h"
#include "BaconAPI/ArgumentHandler.h"
#include "BaconAPI/BuiltInArguments.h"
#include "BaconAPI/Internal/Boolean.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
static const char* baAnsiCodesCharacterArray[BA_ANSI_CODE_SIZE] = {
    "\033[1m", "\033[4m", "\033[5m", "\033[7m",
    "\033[m",
    "\033[0;30m", "\033[0;31m", "\033[0;32m", "\033[0;33m",
    "\033[0;34m", "\033[0;35m", "\033[0;36m", "\033[0;37m",
    "\033[0;39m", "\033[1;30m", "\033[1;31m", "\033[1;32m",
    "\033[1;33m", "\033[1;34m", "\033[1;35m", "\033[1;36m",
    "\033[1;37m", "\033[1;39m", "\033[0;40m", "\033[0;41m",
    "\033[0;42m", "\033[0;43m", "\033[0;44m", "\033[0;45m",
    "\033[0;46m", "\033[0;47m", "\033[0;49m"
};

BA_Boolean BA_ANSI_IsEnabled(void) {
    static int enabled = -1;

    if (enabled == -1)
        enabled = BA_ArgumentHandler_ContainsArgumentOrShort(BA_BUILTINARGUMENTS_DISABLE_ANSI_COLORING, BA_BUILTINARGUMENTS_DISABLE_ANSI_COLORING_SHORT, BA_BOOLEAN_FALSE);

    return enabled;
}

const char* BA_ANSI_ConvertCodeToString(BA_ANSI_Codes ansi) {
    return BA_ANSI_IsEnabled() && ansi < BA_ANSI_CODE_SIZE ? baAnsiCodesCharacterArray[ansi] : "";
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
