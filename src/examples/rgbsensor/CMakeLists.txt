# ADD_EXECUTABLE
add_executable(rgbsensor main.c)
target_link_libraries(rgbsensor board timer i2c isl29125)
set_target_properties(rgbsensor PROPERTIES OUTPUT_NAME "rgbsensor.elf")
prepare_flash(rgbsensor)
