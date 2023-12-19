cmake_minimum_required(VERSION 3.5)

set(EXTRA_COMPONENT_DIRS ${CMAKE_SOURCE_DIR}/components/usb ${CMAKE_SOURCE_DIR}/components/led ${CMAKE_SOURCE_DIR}/components/esp_https_ota $ENV{IDF_PATH}/examples/common_components/qrcode)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(LTEgateway)

target_add_binary_data(LTEgateway.elf "main/certs/ingeliProdCA01.crt" TEXT)
target_add_binary_data(LTEgateway.elf "main/certs/ota/ca_cert.pem" TEXT)

