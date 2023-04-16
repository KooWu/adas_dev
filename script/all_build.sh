#!/bin/bash

echo "$0 file is running"
#0：ubuntu 1：j5,default ubuntu
SOC_TYPE_NAME=ubuntu
if [ $# -gt 0 ];then
    if [ $1 -eq 1 ];then
        SOC_TYPE_NAME=j5
    fi
fi

let CORE_NUM=$(cat /proc/cpuinfo | grep processor | wc -l)/2
echo "CORE_NUM: ${CORE_NUM}"

CUR_DIR=$(pwd)
echo "build CUR_DIR: ${CUR_DIR}"
PRJ_DIR=${CUR_DIR}/..

BUILD_TOOLCHAIN_FILE=
BUILD_INSTALL_DIR=${PRJ_DIR}/install/${SOC_TYPE_NAME}/
mkdir -p ${BUILD_INSTALL_DIR}

if [ ${SOC_TYPE_NAME} = ubuntu ];then
    echo "ubuntu build SOC_TYPE_NAME: ${SOC_TYPE_NAME}"
else
    echo "j5 build SOC_TYPE_NAME: ${SOC_TYPE_NAME}"
    BUILD_TOOLCHAIN_FILE=${PRJ_DIR}/cmake/toolchains/cross/arm-j5-gcc.cmake
fi

rm -rf ${PRJ_DIR}/build
cmake -S ${PRJ_DIR} -DCMAKE_TOOLCHAIN_FILE=${BUILD_TOOLCHAIN_FILE} -DCMAKE_INSTALL_PREFIX=${BUILD_INSTALL_DIR} -DBUILD_PRJ_DIR=${PRJ_DIR} -B ${PRJ_DIR}/build
cmake --build ${PRJ_DIR}/build --target format tidy -j ${CORE_NUM}
cmake --build ${PRJ_DIR}/build --target install docs -j ${CORE_NUM}
echo "$0 file is exit"
