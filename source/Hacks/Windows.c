// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "BaconAPI/Hacks/Windows.h"
#include "BaconAPI/Internal/Boolean.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
static FARPROC BA_WindowsHacks_GetFunction_advapi32(const char* functionName) {
    static HANDLE requiredLibrary = NULL;
    static BA_Boolean initialized = BA_BOOLEAN_FALSE;

    if (requiredLibrary == NULL && !initialized) {
        initialized = BA_BOOLEAN_TRUE;
        requiredLibrary = LoadLibrary("advapi32.dll");
    }

    return requiredLibrary != NULL ? GetProcAddress(requiredLibrary, functionName) : NULL;
}

BOOL WINAPI BA_WindowsHacks_CheckTokenMembership(HANDLE tokenHandle, PSID securityIdentifier, PBOOL isMember) {
    static BOOL (WINAPI *function)(HANDLE, PSID, PBOOL) = NULL;
    static BA_Boolean initialized = BA_BOOLEAN_FALSE;

    if (function == NULL && !initialized) {
        initialized = BA_BOOLEAN_TRUE;
        function = BA_WindowsHacks_GetFunction_advapi32("CheckTokenMembership");
    }

    return function != NULL && function(tokenHandle, securityIdentifier, isMember);
}

BOOL WINAPI BA_WindowsHacks_AllocateAndInitializeSid(PSID_IDENTIFIER_AUTHORITY identifierAuthority, BYTE subAuthorityCount, DWORD subAuthority0, DWORD subAuthority1, DWORD subAuthority2, DWORD subAuthority3, DWORD subAuthority4, DWORD subAuthority5, DWORD subAuthority6, DWORD subAuthority7, PSID* securityIdentifier) {
    static BOOL (WINAPI *function)(PSID_IDENTIFIER_AUTHORITY, BYTE, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, PSID*) = NULL;
    static BA_Boolean initialized = BA_BOOLEAN_FALSE;

    if (function == NULL && !initialized) {
        initialized = BA_BOOLEAN_TRUE;
        function = (BOOL (WINAPI *)(PSID_IDENTIFIER_AUTHORITY, BYTE, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, PSID*)) BA_WindowsHacks_GetFunction_advapi32("AllocateAndInitializeSid");
    }

    return function != NULL && function(identifierAuthority, subAuthorityCount, subAuthority0, subAuthority1, subAuthority2, subAuthority3, subAuthority4, subAuthority5, subAuthority6, subAuthority7, securityIdentifier);
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
