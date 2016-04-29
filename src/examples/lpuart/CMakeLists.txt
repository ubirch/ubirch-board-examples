cmake_minimum_required(VERSION 3.4)

# ADD_EXECUTABLE
ADD_EXECUTABLE(lpuart
  board.c
#  clock_config.c
  lpuart_polling_transfer.c
  pin_mux.c
  )

SET_TARGET_PROPERTIES(lpuart PROPERTIES OUTPUT_NAME "lpuart.elf")

TARGET_LINK_LIBRARIES(lpuart -Wl,--start-group)
TARGET_LINK_LIBRARIES(lpuart ksdk-device-lib)
TARGET_LINK_LIBRARIES(lpuart ksdk-mmcau)
TARGET_LINK_LIBRARIES(lpuart ubirch-firmware)
#TARGET_LINK_LIBRARIES(lpuart wolfcrypt)
# SYSTEM LIBRARIES
TARGET_LINK_LIBRARIES(lpuart m)
TARGET_LINK_LIBRARIES(lpuart c)
TARGET_LINK_LIBRARIES(lpuart gcc)
TARGET_LINK_LIBRARIES(lpuart nosys)
TARGET_LINK_LIBRARIES(lpuart -Wl,--end-group)

prepare_flash(lpuart)
