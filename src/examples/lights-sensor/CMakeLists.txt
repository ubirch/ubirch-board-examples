# ADD_EXECUTABLE
add_executable(lights-sensor main.c)
target_link_libraries(lights-sensor board timer isl29125 sim800h)
prepare_flash(lights-sensor)
