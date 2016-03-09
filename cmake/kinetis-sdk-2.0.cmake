# ENABLE ASM
ENABLE_LANGUAGE(ASM)

set(MK82F25615_FLASH_LD "${KSDK_ROOT}/devices/MK82F25615/gcc/MK82FN256xxx15_flash.ld")

# DEBUG LINK FILE
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -u _printf_float -T\"${MK82F25615_FLASH_LD}\"  -static")

# RELEASE LINK FILE
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -T\"${MK82F25615_FLASH_LD}\"  -static")

# DEBUG ASM FLAGS
SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -g  -mcpu=cortex-m4  -mfloat-abi=hard  -mfpu=fpv4-sp-d16  -mthumb  -Wall  -fno-common  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin  -mapcs  -std=gnu99")

# DEBUG C FLAGS
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g  -O0  -mcpu=cortex-m4  -mfloat-abi=hard  -mfpu=fpv4-sp-d16  -mthumb  -MMD  -MP  -Wall  -fno-common  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin  -mapcs  -std=gnu99")

# DEBUG LD FLAGS
SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -g  -mcpu=cortex-m4  -mfloat-abi=hard  -mfpu=fpv4-sp-d16  --specs=nano.specs  -lm  -Wall  -fno-common  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin  -mthumb  -mapcs  -Xlinker --gc-sections  -Xlinker -static  -Xlinker -z  -Xlinker muldefs  -Xlinker --defsym=__stack_size__=0x2000  -Xlinker --defsym=__heap_size__=0x2000")

# RELEASE ASM FLAGS
SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mcpu=cortex-m4  -mfloat-abi=hard  -mfpu=fpv4-sp-d16  -mthumb  -Wall  -fno-common  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin  -mapcs  -std=gnu99")

# RELEASE C FLAGS
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os  -mcpu=cortex-m4  -mfloat-abi=hard  -mfpu=fpv4-sp-d16  -mthumb  -MMD  -MP  -Wall  -fno-common  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin  -mapcs  -std=gnu99")

# RELEASE LD FLAGS
SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -mcpu=cortex-m4  -mfloat-abi=hard  -mfpu=fpv4-sp-d16  --specs=nano.specs  -lm  -Wall  -fno-common  -ffunction-sections  -fdata-sections  -ffreestanding  -fno-builtin  -mthumb  -mapcs  -Xlinker --gc-sections  -Xlinker -static  -Xlinker -z  -Xlinker muldefs  -Xlinker --defsym=__stack_size__=0x2000  -Xlinker --defsym=__heap_size__=0x2000")

# ASM MACRO
SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG}  -DDEBUG")

# C MACRO
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}  -DDEBUG")
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}  -DCPU_MK82FN256VDC15")
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}  -DFRDM_K82F")
SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}  -DFREEDOM")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}  -DNDEBUG")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}  -DCPU_MK82FN256VDC15")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}  -DFRDM_K82F")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}  -DFREEDOM")

INCLUDE_DIRECTORIES(${KSDK_ROOT}/utilities)
INCLUDE_DIRECTORIES(${KSDK_ROOT}/CMSIS/Include)
INCLUDE_DIRECTORIES(${KSDK_ROOT}/devices/MK82F25615)
INCLUDE_DIRECTORIES(${KSDK_ROOT}/devices/MK82F25615/drivers)
INCLUDE_DIRECTORIES(${KSDK_ROOT}/devices/MK82F25615/utilities)

FILE(GLOB MK82F25615_DRIVERS_SRCS "${KSDK_ROOT}/devices/MK82F25615/drivers/*.c")
ADD_LIBRARY(ksdk-device-lib STATIC
  ${MK82F25615_DRIVERS_SRCS}
  "${KSDK_ROOT}/devices/MK82F25615/gcc/startup_MK82F25615.S"
  "${KSDK_ROOT}/devices/MK82F25615/utilities/fsl_debug_console.c"
  "${KSDK_ROOT}/devices/MK82F25615/utilities/fsl_notifier.c"
  "${KSDK_ROOT}/devices/MK82F25615/utilities/fsl_sbrk.c"
  "${KSDK_ROOT}/devices/MK82F25615/system_MK82F25615.c"
  )

# MMCAU libs
INCLUDE_DIRECTORIES(${KSDK_ROOT}/middleware/mmcau_2.0.0)
INCLUDE_DIRECTORIES(${KSDK_ROOT}/middleware/mmcau_2.0.0/asm-cm4-cm7/src)
add_library(ksdk-mmcau STATIC
  "${KSDK_ROOT}/middleware/mmcau_2.0.0/asm-cm4-cm7/src/cau2_defines.hdr"
  "${KSDK_ROOT}/middleware/mmcau_2.0.0/asm-cm4-cm7/src/mmcau_aes_functions.s"
  "${KSDK_ROOT}/middleware/mmcau_2.0.0/asm-cm4-cm7/src/mmcau_des_functions.s"
  "${KSDK_ROOT}/middleware/mmcau_2.0.0/asm-cm4-cm7/src/mmcau_md5_functions.s"
  "${KSDK_ROOT}/middleware/mmcau_2.0.0/asm-cm4-cm7/src/mmcau_sha1_functions.s"
  "${KSDK_ROOT}/middleware/mmcau_2.0.0/asm-cm4-cm7/src/mmcau_sha256_functions.s"
  )
