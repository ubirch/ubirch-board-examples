# simple crypto library
add_library(crypto crypto.c)
target_link_libraries(crypto wolfcrypt dbgutil)
