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

# Add sqlite3.c as a library
add_library(SQLite3 STATIC sqlite3/sqlite3.c)
target_include_directories(SQLite3 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/sqlite3)

if(IS_IOS)
    add_library(MemCache STATIC src/MemCache.cpp include/MemCache.hpp)
elseif(IS_ANDROID OR IS_MACOS OR IS_WINDOWS)
    add_library(MemCache SHARED src/MemCache.cpp include/MemCache.hpp)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# Add include directory for MemCache library
target_include_directories(MemCache PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src
        )

target_link_libraries(MemCache PRIVATE SQLite3)

# Add compile options
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(MemCache PRIVATE -Wall -Wextra -Wpedantic)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(MemCache PRIVATE /W4 /WX)
endif()

if(IS_IOS)
    get_property(MEMCACHE_OUTPUT_DIR TARGET MemCache PROPERTY ARCHIVE_OUTPUT_DIRECTORY)
    if(NOT MEMCACHE_OUTPUT_DIR)
        set(MEMCACHE_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
    endif()
    set(MEMCACHE_OUTPUT_DIR "${MEMCACHE_OUTPUT_DIR}/$<CONFIG>-iphoneos")
    set(OUTPUT_LIB ${MEMCACHE_OUTPUT_DIR}/libMemCacheMerged.a)

    add_custom_command(OUTPUT ${OUTPUT_LIB}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${MEMCACHE_OUTPUT_DIR}
            COMMAND libtool -static -o ${OUTPUT_LIB} $<TARGET_FILE:MemCache> $<TARGET_FILE:SQLite3>
            DEPENDS MemCache SQLite3
            COMMENT "Merging libMemCache.a and libSQLite3.a into libMemCacheMerged.a"
            )

    add_custom_target(MemCacheMerged ALL
            DEPENDS ${OUTPUT_LIB}
            )
endif()

if(NOT CMAKE_SYSTEM_NAME STREQUAL "iOS")
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