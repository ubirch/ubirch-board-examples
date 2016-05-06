# SIM800H (GSM chip) driver
add_library(sim800h sim800h.c sim800h_parser.c)
target_link_libraries(sim800h board timer)
target_include_directories(sim800h PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
