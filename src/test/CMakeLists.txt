add_executable(ubirch-board main.c)
target_link_libraries(ubirch-board ubirch::${BOARD}::firmware)
prepare_flash(TARGET ubirch-board)
