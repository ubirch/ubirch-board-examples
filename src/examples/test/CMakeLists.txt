add_executable(ubirch-board main.c)
target_link_libraries(ubirch-board board timer)
set_target_properties(ubirch-board PROPERTIES OUTPUT_NAME "ubirch-board.elf")
prepare_flash(ubirch-board)
