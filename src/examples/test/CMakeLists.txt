add_executable(ubirch-board main.c)
set_target_properties(ubirch-board PROPERTIES OUTPUT_NAME "ubirch-board.elf")
target_link_libraries(ubirch-board board)
prepare_flash(ubirch-board)
