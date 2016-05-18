add_executable(lpuart board.c lpuart_polling_transfer.c pin_mux.c)
target_link_libraries(lpuart ubirch::${MCU}::board)
prepare_flash(lpuart)
