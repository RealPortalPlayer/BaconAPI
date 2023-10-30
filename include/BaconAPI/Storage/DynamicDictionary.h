// Purpose: Stores a variable amount of keys and values.
// Created on: 8/15/22 @ 8:18 PM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#include <stddef.h>

#include "BaconAPI/Internal/CPlusPlusSupport.h"
#include "DynamicArray.h"

BA_CPLUSPLUS_SUPPORT_GUARD_START()
typedef struct {
    BA_DynamicArray keys;
    BA_DynamicArray values;
    BA_Boolean frozen;
} BA_DynamicDictionary;

int BA_DynamicDictionary_GetElementIndexFromKey(const BA_DynamicDictionary* dictionary, void* key, size_t elementSize);
int BA_DynamicDictionary_GetElementIndexFromValue(const BA_DynamicDictionary* dictionary, void* value, size_t elementSize);
void* BA_DynamicDictionary_GetElementKeyViaValue(const BA_DynamicDictionary* dictionary, void* value, size_t elementSize);
void* BA_DynamicDictionary_GetElementValueViaKey(const BA_DynamicDictionary* dictionary, void* key, size_t elementSize);

BA_Boolean BA_DynamicDictionary_Create(BA_DynamicDictionary* dictionary, size_t size);
BA_Boolean BA_DynamicDictionary_AddElementToStart(BA_DynamicDictionary* dictionary, void* key, void* value);
BA_Boolean BA_DynamicDictionary_AddElementToLast(BA_DynamicDictionary* dictionary, void* key, void* value);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
BA_Boolean BA_DynamicDictionary_RemoveFirstElement(BA_DynamicDictionary* dictionary);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
BA_Boolean BA_DynamicDictionary_RemoveLastElement(BA_DynamicDictionary* dictionary);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
BA_Boolean BA_DynamicDictionary_RemoveElementAt(BA_DynamicDictionary* dictionary, unsigned int index);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
BA_Boolean BA_DynamicDictionary_RemoveElementViaKey(BA_DynamicDictionary* dictionary, void* key, size_t elementSize);

/**
  * @note This doesn't free any memory, you have to do that yourself to prevent memory leaks.
  */
BA_Boolean BA_DynamicDictionary_RemoveElementViaValue(BA_DynamicDictionary* dictionary, void* value, size_t elementSize);
void BA_DynamicDictionary_GetElementsKeyViaValue(const BA_DynamicDictionary* dictionary, BA_DynamicDictionary* results, void* value, size_t elementSize);
void BA_DynamicDictionary_GetElementsValueViaKey(const BA_DynamicDictionary* dictionary, BA_DynamicDictionary* results, void* key, size_t elementSize);
BA_Boolean BA_DynamicDictionary_Shrink(BA_DynamicDictionary* dictionary);
BA_CPLUSPLUS_SUPPORT_GUARD_END()

#define BA_DYNAMICDICTIONARY_GET_KEY(type, dictionary, value, elementSize) ((type*) BA_DynamicDictionary_GetElementKeyViaValue((dictionary), (value), (elementSize)))
#define BA_DYNAMICDICTIONARY_GET_VALUE(type, dictionary, key, elementSize) ((type*) BA_DynamicDictionary_GetElementValueViaKey((dictionary), (key), (elementSize)))
