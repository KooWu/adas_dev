#!/bin/bash

echo "$0 file is running"
#0：ubuntu 1：j5,default ubuntu
SOC_TYPE_NAME=ubuntu
if [ $# -gt 0 ];then
    if [ $1 -eq 1 ];then
        SOC_TYPE_NAME=j5
    fi
fi
PACKAGE_NAME=$2
MODULE_NAME=$3

let CORE_NUM=$(cat /proc/cpuinfo | grep processor | wc -l)/2
if [ ${CORE_NUM} -eq 0 ];then
    CORE_NUM=1
fi
echo "CORE_NUM: ${CORE_NUM}"

CUR_DIR=$(pwd)
echo "build CUR_DIR: ${CUR_DIR}"
PRJ_DIR=${CUR_DIR}/..
BUILD_MODULE_DIR=${PRJ_DIR}/${PACKAGE_NAME}/${MODULE_NAME}
if [ ! -e ${BUILD_MODULE_DIR} ];then
    echo "not exsit module dir:${BUILD_MODULE_DIR}"
    exit 1
fi

BUILD_CC_DIR=gcc
BUILD_CXX_DIR=g++
BUILD_TOOLCHAIN_FILE=
BUILD_INSTALL_DIR=${PRJ_DIR}/install/${SOC_TYPE_NAME}/
mkdir -p ${BUILD_INSTALL_DIR}

if [ ${SOC_TYPE_NAME} = ubuntu ];then
    echo "ubuntu build SOC_TYPE_NAME: ${SOC_TYPE_NAME}"
else
    echo "j5 build SOC_TYPE_NAME: ${SOC_TYPE_NAME}"
    BUILD_TOOLCHAIN_FILE=${PRJ_DIR}/cmake/toolchains/cross/arm-j5-gcc.cmake
    BUILD_CC_DIR=${PRJ_DIR}/tools/gcc-ubuntu-9.3.0-2020.03-x86_64-aarch64-linux-gnu/bin/aarch64-linux-gnu-gcc
    BUILD_CXX_DIR=${PRJ_DIR}/tools/gcc-ubuntu-9.3.0-2020.03-x86_64-aarch64-linux-gnu/bin/aarch64-linux-gnu-g++
fi

if [ ${MODULE_NAME} = libevent-master -o ${MODULE_NAME} = asio -o ${MODULE_NAME} = protobuf-3.6.1 -o ${MODULE_NAME} = popt-1.18 -o ${MODULE_NAME} = logrotate-3.18.0 ];then
    cd ${BUILD_MODULE_DIR}
    #第一次需要执行这个
    ./autogen.sh
    if [ ${MODULE_NAME} = logrotate-3.18.0 ];then
        ./configure --host=arm-linux CC=${BUILD_CC_DIR} CXX=${BUILD_CXX_DIR} --prefix=${BUILD_INSTALL_DIR}  LDFLAGS=-L${BUILD_INSTALL_DIR}/lib CPPFLAGS=-I${BUILD_INSTALL_DIR}/include
    else
        ./configure --disable-openssl --host=arm-linux --disable-static CC=${BUILD_CC_DIR} CXX=${BUILD_CXX_DIR} --prefix=${BUILD_INSTALL_DIR}
    fi
    make clean
    make -j ${CORE_NUM}
    make install 
elif [ ${MODULE_NAME} = boost_1_74_0 ];then
    cd ${BUILD_MODULE_DIR}
    #第一次需要执行这个
    #./bootstrap.sh --prefix=${BUILD_INSTALL_DIR} --with-toolset=${BUILD_CXX_DIR}
    cp project-config-${SOC_TYPE_NAME}.jam project-config.jam
    ./b2 clean
    ./b2 install --prefix=${BUILD_INSTALL_DIR}
else
    #clean
    #cmake --build ${PRJ_DIR}/build --target clean
    #format
    #clang-format -i -style=file $(find ${BUILD_MODULE_DIR} -regex '.*\.\(cpp\|h\|c\)$')
    #cmake config
    cmake -S ${BUILD_MODULE_DIR} -DCMAKE_TOOLCHAIN_FILE=${BUILD_TOOLCHAIN_FILE} -DCMAKE_INSTALL_PREFIX=${BUILD_INSTALL_DIR} -DBUILD_SHARED_LIBS=ON -DBUILD_PRJ_DIR=${PRJ_DIR} -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B ${BUILD_MODULE_DIR}/build
    #tidy
    #find ${BUILD_MODULE_DIR} -name '*.cpp' -or -name '*.h' | xargs clang-tidy -p ${BUILD_MODULE_DIR}/build
    #build & install
    cmake --build ${BUILD_MODULE_DIR}/build --target install -j ${CORE_NUM}
fi
echo "$0 file is exit"