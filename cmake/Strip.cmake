macro(ba_strip TARGET)
    if(EMSCRIPTEN)
        message(WARNING "TODO: Emscripten is not supported with ba_strip")
        return()
    endif()
    
    add_custom_command(TARGET "${TARGET}"
                       POST_BUILD
                       DEPENDS "${TARGET}"
                       COMMAND $<$<CONFIG:release>:${CMAKE_STRIP}>
                       ARGS --strip-all $<TARGET_FILE:${TARGET}>)
endmacro()