# CMake settings to create a library of wolfcrypt-benchmark

if (wolfcrypt AND DEFINED(WOLFSSL_ROOT))
  message(FATAL_ERROR "wolfcrypt-benchmark requires wolfcrypt library, include before")
else ()
  add_library(wolfcrypt-benchmark STATIC "${WOLFSSL_ROOT}/wolfcrypt/benchmark/benchmark.c")
  target_link_libraries(wolfcrypt-benchmark wolfcrypt)
  target_compile_definitions(wolfcrypt-benchmark PUBLIC
    ${WOLFSSL_DEFINITIONS}
    -DBENCH_EMBEDDED
    -DNO_MAIN_DRIVER
    )
  target_compile_definitions(wolfcrypt PUBLIC
    -DDEBUG_WOLFSSL
    -DWOLFSSL_USER_CURRTIME
    )
endif ()
