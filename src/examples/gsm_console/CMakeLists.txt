# ADD_EXECUTABLE
ADD_EXECUTABLE(gsm_console main.c)
target_link_libraries(gsm_console sim800h)

SET_TARGET_PROPERTIES(gsm_console PROPERTIES OUTPUT_NAME "gsm_console.elf")

TARGET_LINK_LIBRARIES(gsm_console -Wl,--start-group)
TARGET_LINK_LIBRARIES(gsm_console ksdk-device-lib)
TARGET_LINK_LIBRARIES(gsm_console ksdk-mmcau)
TARGET_LINK_LIBRARIES(gsm_console ubirch-firmware)
TARGET_LINK_LIBRARIES(gsm_console wolfcrypt)
# SYSTEM LIBRARIES
TARGET_LINK_LIBRARIES(gsm_console m)
TARGET_LINK_LIBRARIES(gsm_console c)
TARGET_LINK_LIBRARIES(gsm_console gcc)
TARGET_LINK_LIBRARIES(gsm_console nosys)
TARGET_LINK_LIBRARIES(gsm_console -Wl,--end-group)

prepare_flash(gsm_console)
