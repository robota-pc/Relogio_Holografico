# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

FILE(GLOB_RECURSE app_sources "esp32_rotor/*.cpp" "esp32_rotor/*.c")

idf_component_register(SRCS ${app_sources}
                       INCLUDE_DIRS "../include/esp32_rotor"
                       REQUIRES esp_http_server)
