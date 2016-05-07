# ADD_EXECUTABLE
add_executable(rgbsensor main.c)
target_link_libraries(rgbsensor board timer i2c isl29125)
prepare_flash(rgbsensor)
