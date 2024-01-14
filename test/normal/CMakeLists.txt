project(BaconAPITestNormal)

add_executable(BaconAPITestNormal ../TestHeader.h Main.c)

target_link_libraries(BaconAPITestNormal LINK_PRIVATE BaconAPI)

ba_strip(BaconAPITestNormal)