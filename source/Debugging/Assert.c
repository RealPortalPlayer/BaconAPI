// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "BaconAPI/ArgumentHandler.h"
#include "BaconAPI/BuiltInArguments.h"
#include "BaconAPI/Debugging/Assert.h"
#include "BaconAPI/Internal/Boolean.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BA_Boolean BA_Assert_CheckLogsEnabled(void) {
#ifdef BA_ALLOW_DEBUG_LOGS
    static int enabled = -1;

    if (enabled == -1)
        enabled = !BA_ArgumentHandler_ContainsArgumentOrShort(BA_BUILTINARGUMENTS_DONT_PRINT_ASSERT_CHECKS, BA_BUILTINARGUMENTS_DONT_PRINT_ASSERT_CHECKS_SHORT, BA_BOOLEAN_FALSE);

    return enabled;
#else
    return BA_BOOLEAN_FALSE;
#endif
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
