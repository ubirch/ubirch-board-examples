# ADD_EXECUTABLE
add_executable(rgbsensor main.c)
target_link_libraries(rgbsensor ubirch::${BOARD}::firmware)
prepare_flash(rgbsensor)
