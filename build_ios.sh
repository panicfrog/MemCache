#!/bin/sh

set -e

SOURCE_DIR=$(pwd)
BUILD_DIR="build-ios"
INSTALL_DIR="install-ios"
IOS_CMAKE_DIR="./toolchain/ios-cmake" # Set this to the path where you cloned the ios-cmake repository
BUILD_TYPE=Release

rm -rf "${BUILD_DIR}"
rm -rf "${INSTALL_DIR}"

# Build for iOS devices (arm64)
IOS_TARGET="iphoneos"
cmake \
    -B "${BUILD_DIR}/${IOS_TARGET}" \
    -G Xcode -DCMAKE_TOOLCHAIN_FILE="${IOS_CMAKE_DIR}/ios.toolchain.cmake" \
    -DPLATFORM=OS64 \
    -DENABLE_BITCODE=0 \
    -DDEPLOYMENT_TARGET=11 \
    -DMEMCACHE_IOS_STATIC=ON \
    "${SOURCE_DIR}"
cmake --build "${BUILD_DIR}/${IOS_TARGET}" --config ${BUILD_TYPE}

# Build for iOS simulators (x86_64)
SIM_TARGET="iphone-simulator"
SIM_TARGET_X86="${SIM_TARGET}/x86"
cmake \
    -B "${BUILD_DIR}/${SIM_TARGET_X86}" \
    -G Xcode -DCMAKE_TOOLCHAIN_FILE="${IOS_CMAKE_DIR}/ios.toolchain.cmake" \
    -DPLATFORM=SIMULATOR64 \
    -DENABLE_BITCODE=0 \
    -DDEPLOYMENT_TARGET=11 \
    -DMEMCACHE_IOS_STATIC=ON \
    "${SOURCE_DIR}"
cmake --build "${BUILD_DIR}/${SIM_TARGET_X86}" --config ${BUILD_TYPE}

SIM_TARGET_ARM="${SIM_TARGET}/arm64"
cmake \
    -B "${BUILD_DIR}/${SIM_TARGET_ARM}" \
    -G Xcode -DCMAKE_TOOLCHAIN_FILE="${IOS_CMAKE_DIR}/ios.toolchain.cmake" \
    -DPLATFORM=SIMULATORARM64 \
    -DENABLE_BITCODE=0 \
    -DMEMCACHE_IOS_STATIC=ON \
    -DDEPLOYMENT_TARGET=11 \
    "${SOURCE_DIR}"
cmake --build "${BUILD_DIR}/${SIM_TARGET_ARM}" --config ${BUILD_TYPE}

libtool -static \
    "${BUILD_DIR}/${SIM_TARGET_X86}/${BUILD_TYPE}-iphonesimulator/libMemCache.a" \
    "${BUILD_DIR}/${SIM_TARGET_ARM}/${BUILD_TYPE}-iphonesimulator/libMemCache.a" \
    -o "${BUILD_DIR}/${SIM_TARGET}/libMemCache.a"

# Check if xcframework directory exists and remove it
if [ -d "${INSTALL_DIR}/MemCache.xcframework" ]; then
    rm -rf "${INSTALL_DIR}/MemCache.xcframework"
fi

# Create xcframework
xcodebuild -create-xcframework \
    -library "${BUILD_DIR}/${IOS_TARGET}/${BUILD_TYPE}-iphoneos/libMemCache.a" \
    -headers "./include" \
    -library "${BUILD_DIR}/${SIM_TARGET}/libMemCache.a" \
    -headers "./include" \
    -output "${INSTALL_DIR}/MemCache.xcframework"

# remove build
 rm -rf "${BUILD_DIR}"