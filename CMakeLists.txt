# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly

cmake_minimum_required(VERSION 3.5)
# set(CMAKE_CXX_STANDARD 17)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(esp32_c3_042_oled_c)

idf_build_set_property(COMPILE_OPTIONS "-Wno-array-bounds" APPEND)
idf_build_set_property(COMPILE_OPTIONS "-Wno-error=unused-value" APPEND) # hmi solve ui.c
# Enable colors for compile output
idf_build_set_property(COMPILE_OPTIONS "-fdiagnostics-color=always" APPEND)