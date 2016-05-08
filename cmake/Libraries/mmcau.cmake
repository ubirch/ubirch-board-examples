# MMCAU library

if(MCU_MMCAU)
  message(STATUS "MCU has MMCAU support, adding library libmmcau.a")
  include_directories(${KSDK_ROOT}/middleware/mmcau_2.0.0)
  include_directories(${KSDK_ROOT}/middleware/mmcau_2.0.0/asm-cm4-cm7/src)
  add_library(mmcau STATIC
    "${KSDK_ROOT}/middleware/mmcau_2.0.0/asm-cm4-cm7/src/cau2_defines.hdr"
    "${KSDK_ROOT}/middleware/mmcau_2.0.0/asm-cm4-cm7/src/mmcau_aes_functions.s"
    "${KSDK_ROOT}/middleware/mmcau_2.0.0/asm-cm4-cm7/src/mmcau_des_functions.s"
    "${KSDK_ROOT}/middleware/mmcau_2.0.0/asm-cm4-cm7/src/mmcau_md5_functions.s"
    "${KSDK_ROOT}/middleware/mmcau_2.0.0/asm-cm4-cm7/src/mmcau_sha1_functions.s"
    "${KSDK_ROOT}/middleware/mmcau_2.0.0/asm-cm4-cm7/src/mmcau_sha256_functions.s"
    )
endif()