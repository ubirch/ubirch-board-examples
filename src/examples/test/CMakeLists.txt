# ADD_EXECUTABLE
ADD_EXECUTABLE(ubirch-board main.c)
target_link_libraries(ubirch-board board)


SET_TARGET_PROPERTIES(ubirch-board PROPERTIES OUTPUT_NAME "ubirch-board.elf")

TARGET_LINK_LIBRARIES(ubirch-board -Wl,--start-group)
TARGET_LINK_LIBRARIES(ubirch-board ksdk-device-lib)
TARGET_LINK_LIBRARIES(ubirch-board ksdk-mmcau)
#TARGET_LINK_LIBRARIES(ubirch-board ubirch-firmware)
TARGET_LINK_LIBRARIES(ubirch-board wolfcrypt)
# SYSTEM LIBRARIES
TARGET_LINK_LIBRARIES(ubirch-board m)
TARGET_LINK_LIBRARIES(ubirch-board c)
TARGET_LINK_LIBRARIES(ubirch-board gcc)
TARGET_LINK_LIBRARIES(ubirch-board nosys)
TARGET_LINK_LIBRARIES(ubirch-board -Wl,--end-group)

prepare_flash(ubirch-board)
