add_executable(ubirch-board main.c)
target_link_libraries(ubirch-board ubirch::${MCU}::firmware)
prepare_flash(ubirch-board)
