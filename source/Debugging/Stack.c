// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "BaconAPI/OperatingSystem.h"

#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
#   include <execinfo.h>
#   define BA_STACK_BUFFER_SIZE 4024
#elif BA_OPERATINGSYSTEM_WINDOWS
#   include <Windows.h>
#   include <stdint.h>
#   include <dbghelp.h>
#   define BA_STACK_BUFFER_SIZE 0xFFFF
#   include "BaconAPI/Number.h"
#   include "BaconAPI/Internal/PlatformSpecific.h"
#endif

#include <stddef.h>

#include "BaconAPI/Debugging/Stack.h"

#include "BaconAPI/Logger.h"
#include "BaconAPI/String.h"
#include "BaconAPI/Internal/Compiler.h"

// FIXME: This code sucks. It's not async signal safe. The alternative, which is libunwind, doesn't play nicely with CMake.
//        The only option is to do this. Too bad


char* BA_Stack_GetCallTrace(void) {
    void* buffer[BA_STACK_BUFFER_SIZE];
    char* callTrace = BA_String_CreateEmpty();

#if BA_OPERATINGSYSTEM_POSIX_COMPLIANT
    int size = backtrace(buffer, BA_STACK_BUFFER_SIZE);
    char** symbols = backtrace_symbols(buffer, size);
    
    if (symbols != NULL) {
        for (int i = 1; i < size; i++) {
            callTrace = BA_String_Append(callTrace, symbols[i]);

            if (i == size - 1)
                continue;

            callTrace = BA_String_Append(callTrace, "\n");
        }

        free(symbols);
    }
#elif BA_OPERATINGSYSTEM_WINDOWS
    HANDLE process = GetCurrentProcess();
    HANDLE thread = GetCurrentThread();
    CONTEXT context = {};
    BA_Boolean firstOne = BA_BOOLEAN_TRUE;

    context.ContextFlags = CONTEXT_FULL;

    RtlCaptureContext(&context);

    if (!SymInitialize(process, NULL, BA_BOOLEAN_TRUE)) {
        char* errorMessage;
        
        BA_PLATFORMSPECIFIC_GET_ERROR(errorMessage);
        BA_LOGGER_ERROR("Failed to initialize symbol API: %s\n", errorMessage);
        return NULL;
    }

    SymSetOptions(SYMOPT_LOAD_LINES);

    STACKFRAME frame = {};

#if BA_COMPILER_BITS == BA_COMPILER_64BITS
#   define BA_STACK_IP context.Rip
#   define BA_STACK_BP context.Rbp
#   define BA_STACK_SP context.Rsp
#   define BA_STACK_MACHINE IMAGE_FILE_MACHINE_AMD64
#elif BA_COMPILER_BITS == BA_COMPILER_32BITS
#   define BA_STACK_IP context.Eip
#   define BA_STACK_BP context.Ebp
#   define BA_STACK_SP context.Esp
#   define BA_STACK_MACHINE IMAGE_FILE_MACHINE_I386
#endif

    frame.AddrPC.Offset = BA_STACK_IP;
    frame.AddrPC.Mode = AddrModeFlat;
    frame.AddrFrame.Offset = BA_STACK_BP;
    frame.AddrFrame.Mode = AddrModeFlat;
    frame.AddrStack.Offset = BA_STACK_SP;
    frame.AddrStack.Mode = AddrModeFlat;

    while (StackWalk(BA_STACK_MACHINE, process, thread, &frame, &context, NULL, SymFunctionTableAccess, SymGetModuleBase, NULL)) {
        if (firstOne) {
            firstOne = BA_BOOLEAN_FALSE;
            continue;
        }
        
        DWORD64 functionAddress = frame.AddrPC.Offset;
        char* fileName = NULL;
        char* functionName = NULL;
        DWORD lineNumber = 0;
        
        {
            char symbolBuffer[sizeof(IMAGEHLP_SYMBOL) + 255];
            PIMAGEHLP_SYMBOL symbol = (PIMAGEHLP_SYMBOL) symbolBuffer;

            symbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL) + 255;
            symbol->MaxNameLength = 254;

            if (SymGetSymFromAddr(process, frame.AddrPC.Offset, NULL, symbol))
                functionName = symbol->Name;
        }

        {
            DWORD offset = 0;
            IMAGEHLP_LINE line;

            line.SizeOfStruct = sizeof(IMAGEHLP_LINE);

            if (SymGetLineFromAddr(process, frame.AddrPC.Offset, &offset, &line)) {
                fileName = line.FileName;
                lineNumber = line.LineNumber;
            }
        }

        {
            BA_Boolean surround = fileName != NULL;
            
            if (surround)
                callTrace = BA_String_Format(BA_String_Append(callTrace, "%s"), fileName);
            
            if (functionName != NULL)
                callTrace = BA_String_Format(BA_String_Append(callTrace, "%s%s+0x%x%s"), surround ? "(" : "", functionName, lineNumber, surround ? ")" : "");

            surround = functionName != NULL;
            callTrace = BA_String_Format(BA_String_Append(callTrace, "%s0x%x%s\n"), surround ? " [" : "", functionAddress, surround ? "]" : "");
        }
    }

    SymCleanup(process);

    callTrace[strlen(callTrace) - 1] = '\0';
#endif

    return callTrace;
}
