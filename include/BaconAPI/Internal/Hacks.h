// Purpose: Hack macros
// Created on: 10/24/24 @ 11:39 AM

#pragma once

/**
 * Do not use this one. Use BA_HACKS_STRINGIFY
 */
#define BA_HACKS_STRINGIFY_INTERNAL(thing) #thing

/**
 * There's no way to get the string version of a macros value without doing this. Yes, it is required for it to call
 * another macro. Yes, this is more complicated than it has to be
 */
#define BA_HACKS_STRINGIFY(thing) BA_HACKS_STRINGIFY_INTERNAL(thing)
