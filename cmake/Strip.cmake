macro(ba_strip TARGET)
    if(EMSCRIPTEN)
        message(WARNING "TODO: Emscripten is not supported with ba_strip")
        return()
    endif()
    
    add_custom_command(TARGET "${TARGET}"
                       POST_BUILD
                       DEPENDS "${TARGET}"
                       COMMAND $<$<OR:$<CONFIG:release>,$<CONFIG:native>,$<CONFIG:minsizerel>>:${CMAKE_STRIP}>
                       ARGS --strip-all $<TARGET_FILE:${TARGET}>)
endmacro()

macro(ba_strip_static TARGET)
    if(NOT CMAKE_C_COMPILER_ID STREQUAL "MSVC")
        target_compile_options("${TARGET}" PUBLIC "$<$<OR:$<CONFIG:release>,$<CONFIG:native>,$<CONFIG:minsizerel>>:-fvisibility=hidden>")
    endif()
endmacro()