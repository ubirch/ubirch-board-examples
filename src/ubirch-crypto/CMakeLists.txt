add_executable(ubirch-crypto main.c)
target_link_libraries(ubirch-crypto ubirch::${MCU}::firmware)
prepare_flash(ubirch-crypto)
