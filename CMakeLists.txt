cmake_minimum_required(VERSION 3.21)

project(MemCache)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

## Add sqlite3.c as a library
#add_library(src/sqlite3 STATIC src/sqlite3/sqlite3.c)
#target_include_directories(src/sqlite3 PUBLIC src/sqlite3)

if(IS_ANDROID)
set(MEMCACHE_SOURCE_CODE
        src/MemCache.cpp
        src/MemCache.hpp
        include/MemCache_C.h
        src/MemCache_C.cpp
        src/optional.hpp
        src/sqlite3/sqlite3.c
        src/sqlite3/sqlite3.h
        src/main/java/cpp/MemCacheJni.cpp)
else()
set(MEMCACHE_SOURCE_CODE
        src/MemCache.cpp
        src/MemCache.hpp
        include/MemCache_C.h
        src/MemCache_C.cpp
        src/optional.hpp
        src/sqlite3/sqlite3.c
        src/sqlite3/sqlite3.h)
endif()

if(IS_IOS)
    add_library(MemCache STATIC ${MEMCACHE_SOURCE_CODE})
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

#target_link_libraries(MemCache PRIVATE src/sqlite3)
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

#if(IS_IOS)
#    get_property(MEMCACHE_OUTPUT_DIR TARGET MemCache PROPERTY ARCHIVE_OUTPUT_DIRECTORY)
#    if(NOT MEMCACHE_OUTPUT_DIR)
#        set(MEMCACHE_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
#    endif()
#    set(MEMCACHE_OUTPUT_DIR "${MEMCACHE_OUTPUT_DIR}/$<CONFIG>-iphoneos")
#    set(OUTPUT_LIB ${MEMCACHE_OUTPUT_DIR}/merged/libMemCache.a)
#
#    add_custom_command(OUTPUT ${OUTPUT_LIB}
#            COMMAND ${CMAKE_COMMAND} -E make_directory ${MEMCACHE_OUTPUT_DIR}
#            COMMAND libtool -static -o ${OUTPUT_LIB} $<TARGET_FILE:MemCache> $<TARGET_FILE:SQLite3>
#            DEPENDS MemCache src/sqlite3
#            COMMENT "Merging libMemCache.a and libSQLite3.a into merged/libMemCache.a"
#            )
#
#    add_custom_target(MemCacheMerged ALL
#            DEPENDS ${OUTPUT_LIB}
#            )
#endif()

if(NOT CMAKE_SYSTEM_NAME STREQUAL "iOS" AND NOT CMAKE_SYSTEM_NAME STREQUAL "Android")
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