cmake_minimum_required(VERSION 3.4)

# ADD_EXECUTABLE
ADD_EXECUTABLE(lpuart
  board.c
#  clock_config.c
  lpuart_polling_transfer.c
  pin_mux.c
  )
target_link_libraries(lpuart board)

set_target_properties(lpuart PROPERTIES OUTPUT_NAME "lpuart.elf")

target_link_libraries(lpuart -Wl,--start-group)
target_link_libraries(lpuart ksdk-device-lib)
target_link_libraries(lpuart ksdk-mmcau)
# SYSTEM LIBRARIES
target_link_libraries(lpuart m)
target_link_libraries(lpuart c)
target_link_libraries(lpuart gcc)
target_link_libraries(lpuart nosys)
target_link_libraries(lpuart -Wl,--end-group)

prepare_flash(lpuart)
