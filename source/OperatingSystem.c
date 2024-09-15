// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "BaconAPI/OperatingSystem.h"

#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <sys/utsname.h>
#elif BA_OPERATINGSYSTEM_WINDOWS
#   include <Windows.h>

typedef NTSTATUS (WINAPI* BA_OperatingSystem_WindowsGetVersionFunction)(PRTL_OSVERSIONINFOW);
#endif

#include "BaconAPI/String.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
void BA_OperatingSystem_GetVersion(BA_OperatingSystem_Version* version) {
#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
    struct utsname name;

    uname(&name);

    version->systemName = BA_String_Copy(name.sysname);
    version->release = BA_String_Copy(name.release);
#elif BA_OPERATINGSYSTEM_WINDOWS
    HMODULE ntdll = GetModuleHandle("ntdll.dll");
    
    if (ntdll != NULL) {
        BA_OperatingSystem_WindowsGetVersionFunction getVersionFunction = (BA_OperatingSystem_WindowsGetVersionFunction) GetProcAddress(ntdll, "RtlGetVersion");

        if (getVersionFunction != NULL) {
            RTL_OSVERSIONINFOW versionInformation = {0};

            versionInformation.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOW);

            if (getVersionFunction(&versionInformation) == 0) {
                version->systemName = BA_String_Copy("Windows");
                version->release = BA_String_Format(BA_String_Copy("%lu.%lu.%lu"), versionInformation.dwMajorVersion, versionInformation.dwMinorVersion, versionInformation.dwBuildNumber);
                return;
            }
        }
    }

    version->systemName = BA_String_CreateEmpty();
    version->release = BA_String_CreateEmpty();
#endif
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
