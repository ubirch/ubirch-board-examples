# Based on https://github.com/ARMmbed/target-mbed-gcc/blob/master/CMake/toolchain.cmake
# Apache 2.0 licensed
# Copyright (C) 2014-2015 ARM Limited. All rights reserved.
# Copyright (C) 2016 ubirch GmbH

# Configurables:
#  UBIRCH_CFG_GCC_PRINTF_FLOAT - set to include float into printf lib
#
if (UBIRCH_GCC_TOOLCHAIN_INCLUDED)
  return()
endif ()
set(UBIRCH_GCC_TOOLCHAIN_INCLUDED 1)

# if there is not build type set, we default to MinSizeRel
set(CMAKE_BUILD_TYPE "MinSizeRel" CACHE STRING "build type")
message(STATUS "build type: ${CMAKE_BUILD_TYPE}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")

set(CMAKE_SYSTEM_NAME ubirch)
set(CMAKE_SYSTEM_VERSION 1)
SET(CMAKE_SYSTEM_PROCESSOR arm)

find_program(ARM_NONE_EABI_GCC arm-none-eabi-gcc)
find_program(ARM_NONE_EABI_GPP arm-none-eabi-g++)
find_program(ARM_NONE_EABI_OBJCOPY arm-none-eabi-objcopy)
find_program(ARM_NONE_EABI_OBJDUMP arm-none-eabi-objdump)

# macro to print an info message in case we didn't find the compiler executables
macro(gcc_program_notfound progname)
  message("**************************************************************************\n")
  message(" ERROR: the arm gcc program ${progname} could not be found\n")
  if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows" OR CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    message(" you can install the ARM GCC embedded compiler tools from:")
    message(" https://launchpad.net/gcc-arm-embedded/+download ")
  elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
    message(" it is included in the arm-none-eabi-gcc package that you can install")
    message(" with homebrew:\n")
    message("   brew tap ARMmbed/homebrew-formulae")
    message("   brew install arm-none-eabi-gcc")
  endif()
  message("\n**************************************************************************")
  message(FATAL_ERROR "missing program prevents build")
  return()
endmacro(gcc_program_notfound)

if(NOT ARM_NONE_EABI_GCC)
  gcc_program_notfound("arm-none-eabi-gcc")
endif()
if(NOT ARM_NONE_EABI_GPP)
  gcc_program_notfound("arm-none-eabi-g++")
endif()
if(NOT ARM_NONE_EABI_OBJCOPY)
  gcc_program_notfound("arm-none-eabi-objcopy")
endif()
if(NOT ARM_NONE_EABI_OBJDUMP)
  gcc_program_notfound("arm-none-eabi-objdump")
endif()

# Set the compiler to ARM-GCC
if(CMAKE_VERSION VERSION_LESS "3.6.0")
  include(CMakeForceCompiler)
  cmake_force_c_compiler("${ARM_NONE_EABI_GCC}" GNU)
  cmake_force_cxx_compiler("${ARM_NONE_EABI_GPP}" GNU)
else()
  # from 3.5 the force_compiler macro is deprecated: CMake can detect
  # arm-none-eabi-gcc as being a GNU compiler automatically
  set(CMAKE_C_COMPILER "${ARM_NONE_EABI_GCC}")
  set(CMAKE_CXX_COMPILER "${ARM_NONE_EABI_GPP}")
endif()

# find the KSDK directory based on it's name
macro(find_ksdk KSDK_NAME)
  find_path(KSDK_ROOT_PARENT "${KSDK_NAME}" HINTS "libs" ".." ENV KSDK_ROOT)
  if (KSDK_ROOT_PARENT)
    get_filename_component(KSDK_ROOT "${KSDK_NAME}" ABSOLUTE BASE_DIR ${KSDK_ROOT_PARENT})
    message(STATUS "KSDK: ${KSDK_ROOT}")
  else ()
    message(FATAL_ERROR "Could not find Kinetis SDK: ${KSDK_NAME}, searched ../ and libs/")
  endif ()
endmacro()

# special settings for boards
if (BOARD MATCHES "ubirch#1|FRDM-K82F")
  set(MCU "K82F")

  if(NOT KSDK_ROOT)
    find_ksdk("SDK_2.0_MK82FN256xxx15")
  endif()

  set(MCU_SPEC "MK82F25615")
  set(MCU_SPEC_C_FLAGS "-DCPU_MK82FN256VDC15 -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16")
  set(MCU_SPEC_FLASH_LD "${KSDK_ROOT}/devices/${MCU_SPEC}/gcc/MK82FN256xxx15_flash.ld")
  set(MCU_SPEC_LINKER_FLAGS "-T\"${MCU_SPEC_FLASH_LD}\" -static --specs=nano.specs -Wl,--gc-sections  -Wl,-z,muldefs -Wl,--defsym=__stack_size__=0x2000  -Wl,--defsym=__heap_size__=0x2000")
  message(STATUS "MCU: ${MCU_SPEC}")

  # this MCU has MMCAU support
  set(MCU_MMCAU 1)
  include(cmake/Libraries/ksdk20.cmake)

elseif(BOARD MATCHES "FRDM-KL82Z")
  set(MCU "KL82Z")
  if(NOT KSDK_ROOT)
    find_ksdk("SDK_1.3_MKL82Z")
  endif()

  set(MCU_SPEC "MKL82Z7")
  set(MCU_SPEC_C_FLAGS "-DCPU_MKL82Z128VLK7 -mcpu=cortex-m0plus")
  set(MCU_SPEC_FLASH_LD "${KSDK_ROOT}/platform/devices/${MCU_SPEC}/linker/gcc/MKL82Z128xxx7_flash.ld")
  set(MCU_SPEC_LINKER_FLAGS "-T\"${MCU_SPEC_FLASH_LD}\" -static --specs=nano.specs -Wl,--gc-sections  -Wl,-z,muldefs -Wl,--defsym=__stack_size__=0x2000  -Wl,--defsym=__heap_size__=0x2000")
  message(STATUS "MCU: ${MCU_SPEC}")

  include(cmake/Libraries/ksdk13.cmake)
else ()
  message(FATAL_ERROR "Please define BOARD (ubirch#1, FRDM-K82F, FRDM-KL82Z)!")
endif ()
