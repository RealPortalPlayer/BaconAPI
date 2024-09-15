macro(ba_apply_compiler_options TARGET)
    target_compile_definitions("${TARGET}" PUBLIC "$<$<CONFIG:Debug>:DEBUG>"
                                                  "$<$<CONFIG:Debug>:BA_ALLOW_DEBUG_LOGS>"
                                                  "$<$<BOOL:${BA_ALLOW_DEBUG_LOGS}>:BA_ALLOW_DEBUG_LOGS>"
                                                  "$<$<BOOL:${BA_SINGLE_THREADED}>:BA_SINGLE_THREADED>"
                                                  "$<$<BOOL:${X11_FOUND}>:BA_X11_FOUND>"
                                                  BA_C_COMPILER_VERSION="${CMAKE_C_COMPILER_VERSION}"
                                                  BA_CXX_COMPILER_VERSION="${CMAKE_CXX_COMPILER_VERSION}")

    if(NOT BA_DISABLE_X11 AND X11_FOUND)
        target_compile_definitions("${TARGET}" PUBLIC BA_X11_FOUND)
        target_link_libraries("${TARGET}" PUBLIC ${X11_LIBRARIES} Xpm)
        target_include_directories("${TARGET}" PUBLIC ${X11_INCLUDE_DIR})

        if(NOT BA_DISABLE_FREETYPE AND FREETYPE_FOUND)
            target_compile_definitions("${TARGET}" PUBLIC BA_FREETYPE_FOUND)
            target_link_libraries("${TARGET}" PUBLIC ${FREETYPE_LIBRARIES} Xft)
            target_include_directories("${TARGET}" PUBLIC ${FREETYPE_INCLUDE_DIRS})
        endif()
    endif()
    
    target_link_libraries("${TARGET}" PUBLIC "$<$<PLATFORM_ID:Windows>:dbghelp>")
    set(CMAKE_EXECUTABLE_ENABLE_EXPORTS TRUE)
    
    if(NOT "${BA_ASAN_SANITIZER}" STREQUAL "")
        if(MSVC)
            target_compile_options("${TARGET}" PUBLIC "/fsanitize=${BA_ASAN_SANITIZER}")
            target_link_options("${TARGET}" PUBLIC "/fsanitize=${BA_ASAN_SANITIZER}")
        else()
            target_compile_options("${TARGET}" PUBLIC "-fsanitize=${BA_ASAN_SANITIZER}")
            target_link_options("${TARGET}" PUBLIC "-fsanitize=${BA_ASAN_SANITIZER}")
        endif()
        
        target_compile_definitions("${TARGET}" PUBLIC BA_ASAN_ENABLED)
    endif()
endmacro()