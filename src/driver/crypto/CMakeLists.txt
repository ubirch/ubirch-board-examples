# simple crypto library
add_library(crypto crypto.c)
target_link_libraries(crypto wolfcrypt dbgutil)
target_include_directories(crypto PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
