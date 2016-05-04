# ADD_EXECUTABLE
ADD_EXECUTABLE(i2c_scan main.c)
TARGET_LINK_LIBRARIES(i2c_scan board i2c)

SET_TARGET_PROPERTIES(i2c_scan PROPERTIES OUTPUT_NAME "i2c_scan.elf")

TARGET_LINK_LIBRARIES(i2c_scan -Wl,--start-group)
TARGET_LINK_LIBRARIES(i2c_scan ksdk-device-lib)
# SYSTEM LIBRARIES
TARGET_LINK_LIBRARIES(i2c_scan m)
TARGET_LINK_LIBRARIES(i2c_scan c)
TARGET_LINK_LIBRARIES(i2c_scan gcc)
TARGET_LINK_LIBRARIES(i2c_scan nosys)
TARGET_LINK_LIBRARIES(i2c_scan -Wl,--end-group)

prepare_flash(i2c_scan)
