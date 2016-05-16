# ADD_EXECUTABLE
add_executable(rgbsensor main.c)
target_link_libraries(rgbsensor board firmware)
prepare_flash(rgbsensor)
