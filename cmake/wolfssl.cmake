# CMake settings to create a library of wolfcrypt

set(WOLFSSL_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/libs/wolfssl")
set(WOLFSSL_DEFINITIONS
  -DWOLFSSL_KEY_GEN
  -DFREESCALE_KSDK_BM
  -DFREESCALE_MMCAU
  -DHAVE_CURVE25519
  )

if (NOT EXISTS "${WOLFSSL_ROOT}/wolfssl/version.h")
  message(FATAL_ERROR "wolfSSL library not found, checkout using 'git clone --recursive'")
else ()
  include_directories(${WOLFSSL_ROOT})

  file(GLOB WOLFSSL_SRCS ${WOLFSSL_ROOT}/wolfcrypt/src/*.c)

  add_library(wolfcrypt STATIC ${WOLFSSL_SRCS})
  target_compile_definitions(wolfcrypt PUBLIC ${WOLFSSL_DEFINITIONS})
endif ()
