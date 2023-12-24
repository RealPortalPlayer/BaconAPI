// Purpose: Windows specific hacks
// Created on: 9/25/23 @ 4:19 PM

// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include "BaconAPI/Internal/OperatingSystem.h"

#if !BA_OPERATINGSYSTEM_WINDOWS
#   error You cannot include Windows hacks on non-Windows Operating Systems.
#endif

#include <Windows.h>

#include "BaconAPI/Internal/CPlusPlusSupport.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
#ifndef BA_DISABLE_HACKS
WINBOOL WINAPI BA_WindowsHacks_CheckTokenMembership(HANDLE tokenHandle, PSID securityIdentifier, PBOOL isMember);
WINBOOL WINAPI BA_WindowsHacks_AllocateAndInitializeSid(PSID_IDENTIFIER_AUTHORITY identifierAuthority, BYTE subAuthorityCount, DWORD subAuthority0, DWORD subAuthority1, DWORD subAuthority2, DWORD subAuthority3, DWORD subAuthority4, DWORD subAuthority5, DWORD subAuthority6, DWORD subAuthority7, PSID* securityIdentifier);
#else
#   define BA_WindowsHacks_CheckTokenMembership CheckTokenMembership
#   define BA_WindowsHacks_AllocateAndInitializeSid AllocateAndInitializeSid
#endif
BA_CPLUSPLUS_SUPPORT_GUARD_END()
