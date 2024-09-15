// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "BaconAPI/OperatingSystem.h"
#include "BaconAPI/User.h"

#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <unistd.h>
#   include <pwd.h>
#elif BA_OPERATINGSYSTEM_WINDOWS
#   include <stdio.h>
#   include <lmcons.h>

#   include "BaconAPI/Hacks/Windows.h"
#endif

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BA_Boolean BA_User_IsAdministrator(void) {
#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
    return getuid() == 0;
#elif BA_OPERATINGSYSTEM_WINDOWS
    SID_IDENTIFIER_AUTHORITY authority = SECURITY_NT_AUTHORITY;
    PSID adminGroup;
    BA_Boolean admin = BA_WindowsHacks_AllocateAndInitializeSid(&authority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup);
    
    if (admin) {
        if (!BA_WindowsHacks_CheckTokenMembership(NULL, adminGroup, &admin))
            admin = BA_BOOLEAN_FALSE;

        FreeSid(adminGroup);
    }

    return admin;
#endif
}

const char* BA_User_GetName(void) {
#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
    struct passwd* password = getpwuid(getuid());

    return password != NULL ? password->pw_name : "";
#elif BA_OPERATINGSYSTEM_WINDOWS
    static TCHAR buffer[UNLEN + 1] = {'\0'};
    DWORD size = UNLEN + 1;
    
    if (buffer[0] == '\0')
        GetUserName(buffer, &size);

    return buffer;
#endif
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
