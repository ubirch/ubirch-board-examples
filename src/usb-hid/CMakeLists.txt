add_executable(usb-hid
  hid_generic.c
  usb_device_ch9.c
  usb_device_class.c
  usb_device_descriptor.c
  usb_device_hid.c
  ${USB_MIDDLEWARE_SRCS}
  )
target_compile_definitions(usb-hid PRIVATE -DUSB_STACK_BM)
target_include_directories(usb-hid PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${USB_MIDDLEWARE_INC})
target_link_libraries(usb-hid ubirch::${BOARD}::board)
prepare_flash(TARGET usb-hid)
