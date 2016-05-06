# Kinetis SDK 1.3 base library

if (NOT MCU_SPEC)
  message(FATAL_ERROR "KSDK 1.3 base library needs an MCU_SPEC setting (i.e. MK82F25615 or MKL82Z7)")
endif ()

include_directories(${KSDK_ROOT}/platform/osa/inc)
include_directories(${KSDK_ROOT}/platform/CMSIS/Include)
include_directories(${KSDK_ROOT}/platform/utilities/inc)
include_directories(${KSDK_ROOT}/platform/devices)
include_directories(${KSDK_ROOT}/platform/devices/${MCU_SPEC}/include)
include_directories(${KSDK_ROOT}/platform/devices/${MCU_SPEC}/startup)
include_directories(${KSDK_ROOT}/platform/hal/inc)
include_directories(${KSDK_ROOT}/platform/drivers/inc)
include_directories(${KSDK_ROOT}/platform/system/inc)

FILE(GLOB_RECURSE DRIVERS_SRCS "${KSDK_ROOT}/platform/drivers/src/*.c")
add_library(ksdk13 STATIC
  ${DRIVERS_SRCS}
  "${KSDK_ROOT}/platform/system/src/clock/fsl_clock_manager.c"
  "${KSDK_ROOT}/platform/system/src/clock/${MCU_SPEC}/fsl_clock_${MCU_SPEC}.c"
  "${KSDK_ROOT}/platform/devices/${MCU_SPEC}/startup/gcc/startup_${MCU_SPEC}.S"
  "${KSDK_ROOT}/platform/utilities/src/fsl_debug_console.c"
  "${KSDK_ROOT}/platform/utilities/src/fsl_misc_utilities.c"
  "${KSDK_ROOT}/platform/utilities/src/print_scan.c"
  "${KSDK_ROOT}/platform/devices/${MCU_SPEC}/startup/system_${MCU_SPEC}.c"
  "${KSDK_ROOT}/platform/devices/startup.c"
  )
add_definitions(-DKSDK13)
set(KSDK13 1)
