// Purpose: Defines for all the built-in arguments.
// Created on: 9/11/22 @ 12:40 AM

// Copyright (c) 2022, 2023, PortalPlayer <email@portalplayer.xyz>
// Licensed under MIT <https://opensource.org/licenses/MIT>

#pragma once

#define BA_BUILTINARGUMENTS_DONT_PARSE "--"
#define BA_BUILTINARGUMENTS_DISABLE_ANSI_COLORING "--disable-ansi-coloring"
#define BA_BUILTINARGUMENTS_DISABLE_ANSI_COLORING_SHORT "-dac"
#define BA_BUILTINARGUMENTS_LOG_LEVEL "--log-level"
#define BA_BUILTINARGUMENTS_LOG_LEVEL_SHORT "-ll"
#define BA_BUILTINARGUMENTS_DONT_CHANGE_LOG_LEVELS "--dont-change-log-levels"
#define BA_BUILTINARGUMENTS_DONT_CHANGE_LOG_LEVELS_SHORT "-dcll"
#define BA_BUILTINARGUMENTS_ALWAYS_USE_STDOUT "--always-use-stdout"
#define BA_BUILTINARGUMENTS_ALWAYS_USE_STDOUT_SHORT "-aus"
#ifdef BA_ALLOW_DEBUG_LOGS
#   define BA_BUILTINARGUMENTS_DONT_PRINT_ASSERT_CHECKS "--dont-print-assert-checks"
#   define BA_BUILTINARGUMENTS_DONT_PRINT_ASSERT_CHECKS_SHORT "-dpac"
#endif
#define BA_BUILTINARGUMENTS_DISABLE_LOG_HEADER "--dont-log-header"
#define BA_BUILTINARGUMENTS_DISABLE_LOG_HEADER_SHORT "-dlh"
#define BA_BUILTINARGUMENTS_SINGLE_THREADED "--single-threaded"
#define BA_BUILTINARGUMENTS_SINGLE_THREADED_SHORT "-st"
