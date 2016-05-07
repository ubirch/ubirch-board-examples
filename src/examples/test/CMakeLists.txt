add_executable(ubirch-board main.c)
target_link_libraries(ubirch-board board timer)
prepare_flash(ubirch-board)
