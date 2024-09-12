macro(ba_apply_compiler_options TARGET)
    target_compile_definitions("${TARGET}" PUBLIC "$<$<CONFIG:Debug>:DEBUG>"
                                                  "$<$<CONFIG:Debug>:BA_ALLOW_DEBUG_LOGS>"
                                                  "$<$<BOOL:${BA_ALLOW_DEBUG_LOGS}>:BA_ALLOW_DEBUG_LOGS>"
                                                  "$<$<BOOL:${BA_SINGLE_THREADED}>:BA_SINGLE_THREADED>")
    target_link_libraries("${TARGET}" PUBLIC "$<$<PLATFORM_ID:Windows>:dbghelp>")
    set(CMAKE_EXECUTABLE_ENABLE_EXPORTS TRUE)
endmacro()