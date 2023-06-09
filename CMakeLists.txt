cmake_minimum_required(VERSION 3.10.2)

project(MemCache)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(MEMCACHE_IOS_STATIC "Build MemCache as a static library for iOS" OFF)

# Detect platform
if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    set(IS_IOS TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    set(IS_ANDROID TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(IS_MACOS TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(IS_WINDOWS TRUE)
endif()

if(IS_ANDROID)
    set(MEMCACHE_SOURCE_CODE
            include/MemCache_C.h
            include/MemCach_Cpp.hpp
            include/optional.hpp
            src/MemCache.cpp
            src/MemCache.hpp
            src/MemCache_C.cpp
            src/sqlite3/sqlite3.c
            src/sqlite3/sqlite3.h
            src/main/java/cpp/MemCacheJni.cpp
            src/MemCache_Cpp.cpp
            src/Connect.hpp
            src/ThreadPool.hpp)
else()
    set(MEMCACHE_SOURCE_CODE
            include/MemCache_C.h
            include/optional.hpp
            include/MemCach_Cpp.hpp
            src/MemCache.cpp
            src/MemCache.hpp
            src/MemCache_C.cpp
            src/sqlite3/sqlite3.c
            src/sqlite3/sqlite3.h
            src/MemCache_Cpp.cpp
            src/Connect.hpp
            src/ThreadPool.hpp)
endif()

if(IS_IOS)
    if(MEMCACHE_IOS_STATIC)
        add_library(MemCache STATIC ${MEMCACHE_SOURCE_CODE})
    else()
        add_library(MemCache SHARED ${MEMCACHE_SOURCE_CODE})
        set_target_properties(MemCache PROPERTIES
                XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "${MEMCACHE_DEVELOPMENT_TEAM}"
                )
    endif()
elseif(IS_ANDROID)
    find_library(log-lib log)
    add_library(MemCache SHARED ${MEMCACHE_SOURCE_CODE})
elseif(IS_MACOS OR IS_WINDOWS)
    add_library(MemCache SHARED ${MEMCACHE_SOURCE_CODE})
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# Add include directory for MemCache library
target_include_directories(MemCache PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src src/sqlite3
        )

if(IS_ANDROID)
    target_link_libraries(MemCache ${log-lib})
else()
    target_link_libraries(MemCache)
endif()

# Add compile options
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(MemCache PRIVATE -Wall -Wextra -Wpedantic)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(MemCache PRIVATE /W4 /WX)
endif()

if(NOT CMAKE_SYSTEM_NAME STREQUAL "iOS" AND NOT CMAKE_SYSTEM_NAME STREQUAL "Android")
    # Add default implement for callback
    target_compile_definitions(MemCache PRIVATE MOCK_GET_TRACING)
    # Add example
    add_executable(memory_cache_example example.cpp)
    target_link_libraries(memory_cache_example PRIVATE MemCache)

    # Add tests
    include(FetchContent)
    FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG        v2.13.7 # 或任何您需要的版本标签
    )
    FetchContent_MakeAvailable(Catch2)

    add_executable(tests
            tests/test_mem_cache.cpp
            tests/test_main.cpp)

    target_link_libraries(tests MemCache Catch2::Catch2)
endif()