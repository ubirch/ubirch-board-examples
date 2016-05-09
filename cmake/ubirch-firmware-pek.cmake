# This is the CMakeLists.txt for the ubirch firmware layer
# It uses the special code for the board, but not the MK82F25615 layer, as it is also available
# in the Kinetis SDK for our specfic MCU.

set(PEK_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/libs/firmware")

# select all .c files from the board directory and add them to the sources
file(GLOB PEK_FIRMWARE_SRCS ${PEK_ROOT}/runtime/board/*.c)
add_library(pek-firmware STATIC ${PEK_FIRMWARE_SRCS})
target_include_directories(pek-firmware PUBLIC ${PEK_ROOT}/runtime/board)

add_executable(pek-blink ${PEK_ROOT}/apps/blink/main.c)
target_link_libraries(pek-blink pek-firmware ksdk20)
prepare_flash(pek-blink)

add_executable(pek-blink-systick ${PEK_ROOT}/apps/blink-systick/main.c)
target_link_libraries(pek-blink-systick pek-firmware ksdk20)
prepare_flash(pek-blink-systick)

add_executable(pek-ecc-i2c ${PEK_ROOT}/apps/ecc-i2c/main.c)
target_link_libraries(pek-ecc-i2c pek-firmware ksdk20)
prepare_flash(pek-ecc-i2c)

add_executable(pek-gpio ${PEK_ROOT}/apps/gpio/main.c)
target_link_libraries(pek-gpio pek-firmware ksdk20)
prepare_flash(pek-gpio)

add_executable(pek-powerdomains ${PEK_ROOT}/apps/powerdomains/main.c)
target_link_libraries(pek-powerdomains pek-firmware ksdk20)
prepare_flash(pek-powerdomains)

add_executable(pek-sim800-uart ${PEK_ROOT}/apps/sim800-uart/main.c)
target_link_libraries(pek-sim800-uart pek-firmware ksdk20)
prepare_flash(pek-sim800-uart)

add_executable(pek-vbatsense ${PEK_ROOT}/apps/vbatsense/main.c)
target_link_libraries(pek-vbatsense pek-firmware ksdk20)
prepare_flash(pek-vbatsense)

