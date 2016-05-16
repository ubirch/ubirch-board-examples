# ADD_EXECUTABLE
add_executable(i2c_scan main.c)
target_link_libraries(i2c_scan board firmware)
prepare_flash(i2c_scan)
