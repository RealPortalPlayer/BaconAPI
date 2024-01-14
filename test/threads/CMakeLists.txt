project(BaconAPITestThread)

add_executable(BaconAPITestThread ../TestHeader.h Main.c)

target_link_libraries(BaconAPITestThread LINK_PRIVATE BaconAPI)

ba_strip(BaconAPITestThread)