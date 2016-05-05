# Kinetis SDK 2.0 base library

include_directories(${KSDK_ROOT}/CMSIS/Include)
include_directories(${KSDK_ROOT}/devices/${MCU_SPEC})
include_directories(${KSDK_ROOT}/devices/${MCU_SPEC}/drivers)
include_directories(${KSDK_ROOT}/devices/${MCU_SPEC}/utilities)

FILE(GLOB DRIVERS_SRCS "${KSDK_ROOT}/devices/${MCU_SPEC}/drivers/*.c")
add_library(ksdk20 STATIC
  ${DRIVERS_SRCS}
  "${KSDK_ROOT}/devices/${MCU_SPEC}/gcc/startup_${MCU_SPEC}.S"
  "${KSDK_ROOT}/devices/${MCU_SPEC}/utilities/fsl_debug_console.c"
  "${KSDK_ROOT}/devices/${MCU_SPEC}/utilities/fsl_notifier.c"
  "${KSDK_ROOT}/devices/${MCU_SPEC}/utilities/fsl_sbrk.c"
  "${KSDK_ROOT}/devices/${MCU_SPEC}/system_${MCU_SPEC}.c"
  )
target_compile_definitions(ksdk20 PRIVATE "-DFRDM_K82F -DFREEDOM")
