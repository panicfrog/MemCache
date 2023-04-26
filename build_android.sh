#!/bin/sh

# Use CMake to generate build files for Android

# Set toolchain file as a variable
TOOLCHAIN=/Users/yyp/Library/Android/sdk/ndk-bundle/build/cmake/android.toolchain.cmake

# Set ABI as a variable
# ABI="armeabi-v7a;x86;x86_64;arm64-v8a"
ABI="arm64-v8a"

# dir
DIR=./build_android

# Use CMake to generate build files for Android
cmake -S ./ \
    -B $DIR \
    -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN \
    -DANDROID_ABI=$ABI \
    -DANDROID_PLATFORM=android-21

# Build the arr package using Gradle
cd /Users/yyp/source/cpp/MemCache/build_android
./gradlew assembleRelease