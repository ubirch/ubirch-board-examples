# ISL29125 driver
add_library(isl29125 isl29125.c)
target_link_libraries(isl29125 i2c)
