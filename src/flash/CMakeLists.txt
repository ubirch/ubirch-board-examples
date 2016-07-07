add_executable(flash flash.c)
target_link_libraries(flash ubirch::${BOARD}::board)
prepare_flash(TARGET flash)
