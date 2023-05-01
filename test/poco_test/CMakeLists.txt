cmake_minimum_required (VERSION 3.16)

project(http_test)

set(THIRD_PART_LIB_DIR)
if (EXISTS ${CMAKE_TOOLCHAIN_FILE})
    set(THIRD_PART_LIB_DIR ${BUILD_PRJ_DIR}/install/j5/)
else()
    set(THIRD_PART_LIB_DIR ${BUILD_PRJ_DIR}/install/ubuntu/)
endif()

add_executable(http_client http/http_client.cpp)
target_include_directories(http_client PRIVATE ${THIRD_PART_LIB_DIR}/include/)
target_link_libraries(http_client PRIVATE ${THIRD_PART_LIB_DIR}/lib/libPocoNet.so)
target_link_libraries(http_client PRIVATE ${THIRD_PART_LIB_DIR}/lib/libPocoUtil.so)
target_link_libraries(http_client PRIVATE ${THIRD_PART_LIB_DIR}/lib/libPocoFoundation.so)

add_executable(http_server http/http_server.cpp)
target_include_directories(http_server PRIVATE ${THIRD_PART_LIB_DIR}/include/)
target_link_libraries(http_server PRIVATE ${THIRD_PART_LIB_DIR}/lib/libPocoNet.so)
target_link_libraries(http_server PRIVATE ${THIRD_PART_LIB_DIR}/lib/libPocoUtil.so)
target_link_libraries(http_server PRIVATE ${THIRD_PART_LIB_DIR}/lib/libPocoFoundation.so)

install(TARGETS http_client http_server RUNTIME DESTINATION bin)