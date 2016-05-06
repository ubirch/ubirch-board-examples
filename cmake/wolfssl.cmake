# wolfCrypt library

if(KSDK20)
  set(WOLFSSL_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/libs/wolfssl")
  set(WOLFSSL_DEFINITIONS
    -DWOLFSSL_KEY_GEN
    -DFREESCALE_KSDK_BM
    -DWOLFSSL_SHA512
    -DHAVE_CURVE25519
    -DHAVE_ED25519
    )

  if(MCU STREQUAL "K82F")
    set(WOLFSSL_DEFINITIONS "${WOLFSSL_DEFINITIONS} -DFREESCALE_MMCAU")
  endif()

  if (NOT EXISTS "${WOLFSSL_ROOT}/wolfssl/version.h")
    message(FATAL_ERROR "wolfSSL library not found, checkout using 'git clone --recursive'")
  else ()
    include_directories(${WOLFSSL_ROOT})

    file(GLOB WOLFSSL_SRCS ${WOLFSSL_ROOT}/wolfcrypt/src/*.c)
    list(REMOVE_ITEM WOLFSSL_SRCS ${WOLFSSL_ROOT}/wolfcrypt/src/misc.c)

    add_library(wolfcrypt STATIC ${WOLFSSL_SRCS})
    target_link_libraries(wolfcrypt mmcau)
    target_compile_definitions(wolfcrypt PUBLIC ${WOLFSSL_DEFINITIONS})
  endif ()
endif()
