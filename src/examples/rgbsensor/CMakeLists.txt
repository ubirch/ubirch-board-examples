# ADD_EXECUTABLE
ADD_EXECUTABLE(rgbsensor main.c)
TARGET_LINK_LIBRARIES(rgbsensor board i2c isl29125)

SET_TARGET_PROPERTIES(rgbsensor PROPERTIES OUTPUT_NAME "rgbsensor.elf")

TARGET_LINK_LIBRARIES(rgbsensor -Wl,--start-group)
TARGET_LINK_LIBRARIES(rgbsensor ksdk-device-lib)
TARGET_LINK_LIBRARIES(rgbsensor ksdk-mmcau)
# SYSTEM LIBRARIES
TARGET_LINK_LIBRARIES(rgbsensor m)
TARGET_LINK_LIBRARIES(rgbsensor c)
TARGET_LINK_LIBRARIES(rgbsensor gcc)
TARGET_LINK_LIBRARIES(rgbsensor nosys)
TARGET_LINK_LIBRARIES(rgbsensor -Wl,--end-group)

prepare_flash(rgbsensor)
