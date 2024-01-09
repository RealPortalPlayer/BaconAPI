project(BaconAPITest)

add_executable(BaconAPITest Main.c)

target_link_libraries(BaconAPITest LINK_PRIVATE BaconAPI)

ba_strip(BaconAPITest)