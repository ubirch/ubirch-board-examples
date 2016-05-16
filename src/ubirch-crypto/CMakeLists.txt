add_executable(ubirch-crypto main.c)
target_link_libraries(ubirch-crypto board firmware wolfcrypt)
prepare_flash(ubirch-crypto)
