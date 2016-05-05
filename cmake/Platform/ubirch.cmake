# Based on https://github.com/ARMmbed/target-mbed-gcc/blob/master/CMake/Platform/mbedOS.cmake
# Apache 2.0 licensed
# Copyright (C) 2014-2015 ARM Limited. All rights reserved.
# Copyright (C) 2016 ubirch GmbH
#
#include(Compiler/GNU)

set(CMAKE_EXECUTABLE_SUFFIX ".elf" CACHE STRING FORCE "")

set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS FALSE)

set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
set(CMAKE_EXECUTABLE_SUFFIX "")
set(CMAKE_C_OUTPUT_EXTENSION ".o")
set(CMAKE_ASM_OUTPUT_EXTENSION ".o")
set(CMAKE_CXX_OUTPUT_EXTENSION ".o")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

if(NOT KSDK_ROOT)
  message(FATAL_ERROR "Please define KSDK_ROOT!")
endif()

if (MCU STREQUAL "K82F")
  set(MCU_SPEC "MK82F25615")
  set(MCU_SPEC_C_FLAGS "-DCPU_MK82FN256VDC15 -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16")
  set(MCU_SPEC_FLASH_LD "${KSDK_ROOT}/devices/${MCU_SPEC}/gcc/MK82FN256xxx15_flash.ld")
  set(MCU_SPEC_LINKER_FLAGS "-T\"${MCU_SPEC_FLASH_LD}\" -static --specs=nano.specs -Wl,--gc-sections  -Wl,-z,muldefs -Wl,--defsym=__stack_size__=0x2000  -Wl,--defsym=__heap_size__=0x2000")
  message(STATUS "MCU: ${MCU_SPEC}\n")
else ()
  message(FATAL_ERROR "Please define MCU (K82F)!")
endif ()

# set default compilation flags
set(_C_FAMILY_FLAGS "${MCU_SPEC_C_FLAGS} -mthumb -MMD -MP -fno-common -fno-exceptions -fno-unwind-tables -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mapcs-frame -Wall")

#set(CMAKE_MODULE_LINKER_FLAGS_INIT "${_C_FAMILY_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_INIT "${_C_FAMILY_FLAGS} ${MCU_SPEC_LINKER_FLAGS}")

# if GCC_PRINTF_FLOAT is set or not present, include float printing
if((NOT DEFINED UBIRCH_CFG_GCC_PRINTF_FLOAT) OR (UBIRCH_CFG_GCC_PRINTF_FLOAT))
  set(CMAKE_EXE_LINKER_FLAGS_INIT "${CMAKE_EXE_LINKER_FLAGS_INIT} -Wl,-u,_printf_float")
endif()
