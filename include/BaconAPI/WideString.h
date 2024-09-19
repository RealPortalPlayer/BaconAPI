// Purpose: Wide string wrapper
// Created on: 9/18/24 @ 1:08 PM

// Copyright (c) 2024, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <wchar.h>

#include "Internal/CPlusPlusSupport.h"
#include "Internal/Boolean.h"
#include "Storage/DynamicArray.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
BA_Boolean BA_WideString_Contains(const wchar_t* string, const wchar_t* compare, BA_Boolean caseless);
BA_Boolean BA_WideString_Equals(const wchar_t* string, const wchar_t* compare, BA_Boolean caseless);
BA_Boolean BA_WideString_StartsWith(const wchar_t* string, const wchar_t* compare, BA_Boolean caseless);
BA_Boolean BA_WideString_EndsWith(const wchar_t* string, const wchar_t* compare, BA_Boolean caseless);
BA_Boolean BA_WideString_ContainsCharacter(const wchar_t* string, wchar_t compare, BA_Boolean caseless);

/**
 * @note Returns NULL if it fails to allocate memory
 */
wchar_t* BA_WideString_Copy(const wchar_t* duplicateFrom);
wchar_t* BA_WideString_Append(wchar_t* target, const wchar_t* stringToAppend);
wchar_t* BA_WideString_Prepend(wchar_t* target, const wchar_t* stringToPrepend);
wchar_t* BA_WideString_ToLower(wchar_t* string);
wchar_t* BA_WideString_ToUpper(wchar_t* string);
wchar_t* BA_WideString_AppendCharacter(wchar_t* target, wchar_t character);
wchar_t* BA_WideString_PrependCharacter(wchar_t* target, wchar_t character);
wchar_t* BA_WideString_Format(wchar_t* target, ...);
wchar_t* BA_WideString_FormatPremadeList(wchar_t* target, va_list arguments);
BA_DynamicArray* BA_WideString_Split(const wchar_t* target, const wchar_t* splitBy);
BA_DynamicArray* BA_WideString_SplitCharacter(const wchar_t* target, wchar_t splitBy);
wchar_t* BA_WideString_FormatSafe(wchar_t* target, int amountOfFormatters, ...);
wchar_t* BA_WideString_FormatSafePremadeList(wchar_t* target, int amountOfFormatters, va_list arguments);
wchar_t* BA_WideString_CreateEmpty(void);
wchar_t* BA_WideString_Replace(wchar_t* target, const wchar_t* what, const wchar_t* to);
wchar_t* BA_WideString_ReplaceCharacter(wchar_t* target, wchar_t what, wchar_t to);
wchar_t* BA_WideString_Join(const BA_DynamicArray* dynamicArray, const wchar_t* joinString);
wchar_t* BA_WideString_JoinCharacter(const BA_DynamicArray* dynamicArray, wchar_t joinCharacter);
wchar_t* BA_WideString_Convert(const char* target);
BA_CPLUSPLUS_SUPPORT_GUARD_END()
