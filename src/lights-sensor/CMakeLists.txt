# ADD_EXECUTABLE
add_executable(lights-sensor main.c)
target_link_libraries(lights-sensor ubirch::${BOARD}::firmware)
prepare_flash(lights-sensor)

# copy the config.h.template to config.h in case it is not there; it is ignored by .git!
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/config.h")
  message(AUTHOR_WARNING "In directory '${CMAKE_CURRENT_SOURCE_DIR}': installing the template config.h, please edit!")
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.template ${CMAKE_CURRENT_SOURCE_DIR}/config.h COPYONLY)
endif()

