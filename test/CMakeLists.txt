file(GLOB files "*.c")

foreach(file ${files})
    get_filename_component(filename "${file}" NAME_WE)

    project("${filename}Test")

    add_executable("${filename}Test" "${file}")

    target_link_libraries("${filename}Test" PRIVATE BaconAPI)

    ba_strip("${filename}Test")
endforeach()