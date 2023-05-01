cmake_minimum_required (VERSION 3.16)

project(mqtt_test)

set(THIRD_PART_LIB_DIR)
if (EXISTS ${CMAKE_TOOLCHAIN_FILE})
    set(THIRD_PART_LIB_DIR ${BUILD_PRJ_DIR}/install/j5/)
else()
    set(THIRD_PART_LIB_DIR ${BUILD_PRJ_DIR}/install/ubuntu/)
endif()

add_executable(mqtt_sub sub/mqtt_sub.cpp)
target_include_directories(mqtt_sub PRIVATE ${THIRD_PART_LIB_DIR}/include/)
target_link_libraries(mqtt_sub PRIVATE ${THIRD_PART_LIB_DIR}/lib/libmosquitto.so)

install(TARGETS mqtt_sub RUNTIME DESTINATION bin)