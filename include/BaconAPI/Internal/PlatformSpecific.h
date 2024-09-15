// Purpose: Commonly used platform specific macros
// Created on: 4/22/2023 @ 7:10 PM

// Copyright (c) 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include "../OperatingSystem.h"
#include "Boolean.h"

#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <dlfcn.h>
#   include <unistd.h>
#   define BA_PLATFORMSPECIFIC_CHANGE_DIRECTORY(directory) chdir(directory)
#   define BA_PLATFORMSPECIFIC_GET_BINARY(name, options) dlopen(name, options)
#   define BA_PLATFORMSPECIFIC_GET_ADDRESS(binary, name) dlsym(binary, name)
#   define BA_PLATFORMSPECIFIC_GET_ERROR(variable) variable = dlerror()
#   if BA_OPERATINGSYSTEM_APPLE
#       define BA_PLATFORMSPECIFIC_BINARY_EXTENSION ".dylib"
#   elif BA_OPERATINGSYSTEM_LINUX || BA_OPERATINGSYSTEM_UNIX || BA_OPERATINGSYSTEM_SERENITY
#       define BA_PLATFORMSPECIFIC_BINARY_EXTENSION ".so"
 #   elif BA_OPERATINGSYSTEM_EMSCRIPTEN
#       define BA_PLATFORMSPECIFIC_BINARY_EXTENSION ".wasm"
#   endif
#   define BA_PLATFORMSPECIFIC_CLOSE_BINARY(binary) dlclose(binary)
#   define BA_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(type, variable, value) *(void**) &variable = value
#elif BA_OPERATINGSYSTEM_WINDOWS
#   include <Windows.h>
#   include <direct.h>
#   define BA_PLATFORMSPECIFIC_CHANGE_DIRECTORY(directory) _chdir(directory)
#   define BA_PLATFORMSPECIFIC_GET_BINARY(name, options) LoadLibrary(name)
#   define BA_PLATFORMSPECIFIC_GET_ADDRESS(binary, name) GetProcAddress(binary, name)
#   define BA_PLATFORMSPECIFIC_GET_ERROR(variable) do { \
        DWORD id = GetLastError();                       \
        LPSTR message = NULL;                            \
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, id, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR) &message, 0, NULL); \
        variable = message;                              \
    } while (BA_BOOLEAN_FALSE)
#   define BA_PLATFORMSPECIFIC_BINARY_EXTENSION ".dll"
#   define BA_PLATFORMSPECIFIC_CLOSE_BINARY(binary) FreeLibrary(binary)
#   define BA_PLATFORMSPECIFIC_FUNCTION_VARIABLE_SETTER(type, variable, value) variable = (type) value
#endif
