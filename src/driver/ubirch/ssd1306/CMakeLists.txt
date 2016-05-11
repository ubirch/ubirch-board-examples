# SSD1306 (OLED) driver
add_library(ssd1306 ssd1306.c)
target_link_libraries(ssd1306 i2c)
