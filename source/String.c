// Copyright (c) 2023, 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#include "BaconAPI/OperatingSystem.h"

#if BA_OPERATINGSYSTEM_WINDOWS
#   include <Windows.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <wchar.h>

#include "BaconAPI/String.h"
#include "BaconAPI/Debugging/Assert.h"
#include "BaconAPI/Storage/DynamicDictionary.h"
#include "BaconAPI/OperatingSystem.h"
#include "StringImplementation.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BA_STRINGIMPLEMENTATION_CREATE(String)

char* BA_String_ReadFile(FILE* file, size_t lengthLimit, size_t* lineLength) {
    fseek(file, 0, SEEK_END);
    
    size_t numberOfBytes = ftell(file);

    fseek(file, 0, SEEK_SET);

    if (lengthLimit != 0 && numberOfBytes > lengthLimit)
        numberOfBytes = lengthLimit;
    
    char* buffer = calloc(sizeof(char) * numberOfBytes, 1);
    
    if (buffer == NULL)
        return NULL;

    if (lineLength != NULL)
        *lineLength = numberOfBytes;

    buffer[0] = '\0';

    fread(buffer, sizeof(char), numberOfBytes, file);
    fseek(file, 0, SEEK_SET);

    buffer[numberOfBytes] = '\0';
    return buffer;
}

intmax_t BA_String_GetLine(FILE* file, char** line, const char* splitString) {
    const char* currentSplitString = splitString != NULL ? splitString : "\n";
    intmax_t length = 0;
    char* buffer = NULL;
    intmax_t splitStringLength = (int) strlen(currentSplitString);

    if (line != NULL) {
        buffer = BA_String_CreateEmpty();
                
        if (buffer == NULL)
            return -2;
    }
    
    while (!feof(file)) {
        char* contents = calloc(splitStringLength + 1, 1);

        contents[0] = '\0';
        contents[splitStringLength] = '\0';
        
        fread(contents, sizeof(char), splitStringLength, file);

        if (BA_String_Equals(contents, currentSplitString, BA_BOOLEAN_FALSE)) {
            if (line != NULL)
                *line = buffer;

            free(contents);
            return length;
        }
        
        length += splitStringLength;

        if (line == NULL) {
            free(contents);
            continue;
        }
        
        buffer = BA_String_Append(buffer, contents);
        
        free(contents);
    }

    if (line != NULL && buffer[0] != '\0') {
        *line = buffer;
        return length;
    }
    
    return -1;
}

intmax_t BA_String_GetLineCharacter(FILE* file, char** line, char splitCharacter) {
    const char temporaryString[2] = {splitCharacter, '\0'};
    
    return BA_String_GetLine(file, line, temporaryString);
}
BA_CPLUSPLUS_SUPPORT_GUARD_END()
