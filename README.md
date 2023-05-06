## 集成

### 构建Android库
1. 将项目通过gradle集成到项目中

2. 在项目build.gradle中添加prefab的支持

   ```groovy
   android {
   	...
     buildFeatures {
           prefab true
     }
     ...
   }
   ```

3. 在项目中创建一个cpp文件夹，写一个CMakeLists.txt

   ```cmake
   # For more information about using CMake with Android Studio, read the
   # documentation: https://d.android.com/studio/projects/add-native-code.html
   
   # Sets the minimum version of CMake required to build the native library.
   
   cmake_minimum_required(VERSION 3.22.1)
   
   # Declares and names the project.
   
   project(YourProject VERSION 1.0.0 LANGUAGES CXX)
   
   find_package(MemCache REQUIRED CONFIG)
   
   add_library(
           YourProject
           SHARED
           native-lib.cpp)
   
   find_library(
           log-lib
           log)
   
   target_link_libraries(
           YourProject
           MemCache::MemCache
           ${log-lib})
   ```

4. 在配置中添加cpp的编译配置

   ```groovy
   android {
     	...
       defaultConfig {
          ...
          externalNativeBuild {
               cmake {
                   cppFlags "-O2 -frtti -fexceptions -Wall -fstack-protector-all -std=c++20 -DONANDROID"
                   abiFilters 'x86', 'x86_64', 'armeabi-v7a', 'arm64-v8a'
                   arguments '-DANDROID_STL=c++_shared'
             }
         }
         ...
     }
     buildFeatures {
           prefab true
     }
     
      externalNativeBuild {
           cmake {
               path "src/main/cpp/CMakeLists.txt"
               version '3.22.1'
           }
       }
     ...
   }
   ```

   

### 构建iOS库

1. 在此项目目录执行

```shell
./build_ios.sh
```

2. 通过cocoapods集成（通过路径或者git地址）

```rub
pod 'MemCache-Swift', :path => "../../cpp/MemCache"
```

