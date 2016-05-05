add_executable(lpuart board.c lpuart_polling_transfer.c pin_mux.c)
target_link_libraries(lpuart board)
set_target_properties(lpuart PROPERTIES OUTPUT_NAME "lpuart.elf")
prepare_flash(lpuart)
