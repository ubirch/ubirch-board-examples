set(UBIRCH_FIRMWARE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/libs/firmware")
INCLUDE_DIRECTORIES(${UBIRCH_FIRMWARE_ROOT}/runtime/board)

ADD_LIBRARY(ubirch-firmware
        ${UBIRCH_FIRMWARE_ROOT}/runtime/board/atecc508a.c
        ${UBIRCH_FIRMWARE_ROOT}/runtime/board/board.c
        ${UBIRCH_FIRMWARE_ROOT}/runtime/board/clock_config.c
        ${UBIRCH_FIRMWARE_ROOT}/runtime/board/extpin.c
        )
