# Kinetis SDK 2.0 base library

if (NOT MCU_SPEC)
  message(FATAL_ERROR "KSDK 2.0 base library needs an MCU_SPEC setting (i.e. MK82F25615)")
endif ()

include_directories(${KSDK_ROOT}/CMSIS/Include)
include_directories(${KSDK_ROOT}/devices/${MCU_SPEC}/drivers)
include_directories(${KSDK_ROOT}/devices/${MCU_SPEC}/utilities)
include_directories(${KSDK_ROOT}/devices/${MCU_SPEC})

FILE(GLOB DRIVERS_SRCS "${KSDK_ROOT}/devices/${MCU_SPEC}/drivers/*.c")
add_library(ksdk20 STATIC
  ${DRIVERS_SRCS}
  "${KSDK_ROOT}/devices/${MCU_SPEC}/gcc/startup_${MCU_SPEC}.S"
  "${KSDK_ROOT}/devices/${MCU_SPEC}/utilities/fsl_debug_console.c"
  "${KSDK_ROOT}/devices/${MCU_SPEC}/utilities/fsl_notifier.c"
  "${KSDK_ROOT}/devices/${MCU_SPEC}/utilities/fsl_sbrk.c"
  "${KSDK_ROOT}/devices/${MCU_SPEC}/system_${MCU_SPEC}.c"
  )
add_definitions(-DKSDK20)
set(KSDK20 1)
