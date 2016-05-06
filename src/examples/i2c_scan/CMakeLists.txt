# ADD_EXECUTABLE
add_executable(i2c_scan main.c)
target_link_libraries(i2c_scan board timer i2c)
set_target_properties(i2c_scan PROPERTIES OUTPUT_NAME "i2c_scan.elf")
prepare_flash(i2c_scan)
