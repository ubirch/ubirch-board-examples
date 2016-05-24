add_executable(ubirch-crypto main.c)
target_link_libraries(ubirch-crypto
  ubirch::${BOARD}::firmware
  ubirch-crypto::${BOARD}::crypto
  )
prepare_flash(TARGET ubirch-crypto)
