## BUILD

### 构建Android库
1. 在项目根目录中创建一个名为`build-android`的新文件夹：
2. 进入新创建的文件夹：
3. 使用CMake为Android配置项目：

``` shell
cmake .. \
-DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
-DANDROID_ABI="armeabi-v7a" \
-DANDROID_PLATFORM=android-21 \
-DCMAKE_BUILD_TYPE=Release
```
注意：
- 请替换`$ANDROID_NDK`为您的Android NDK安装路径。
- 根据需要更改`-DANDROID_ABI`和`-DANDROID_PLATFORM`选项。

4. 构建项目：

``` shell
cmake --build .
```
- 在`build-android`文件夹下的`lib`子目录中找到生成的库。

### 构建iOS库
在次项目目录执行
```shell
./build_ios.sh
```


```swift
//
//  MemCache.swift
//  MemCacheDemo
//
//  Created by mac studio on 2023/4/24.
//

import Foundation

final class MemCache {
  let cache: UnsafeMutableRawPointer!
  init() {
    self.cache = MemCache_new()
  }
  
  func getString(forKey key: String) -> String? {
    let cKey = key.cString(using: .utf8)
    var cValue: UnsafeMutablePointer<Int8>?
    
    let success = MemCache_get_string(cache, cKey, &cValue)
    let value: String?
    if success {
      value = String(cString: cValue!, encoding: .utf8)
    } else {
      value = nil
    }
    return value
  }
  
  func getInt32(forKey key: String) -> Int32? {
    let cKey = key.cString(using: .utf8)
    var value: Int32 = 0
    let success = MemCache_get_int(cache, cKey, &value)
    if success {
      return value
    } else {
      return nil
    }
  }
  
  func getDouble(forKey key: String) -> Double? {
    let cKey = key.cString(using: .utf8)
    var value: Double = 0
    let success = MemCache_get_double(cache, cKey, &value)
    if success {
      return value
    } else {
      return nil
    }
  }
  
  func getBool(forKey key: String) -> Bool? {
    let cKey = key.cString(using: .utf8)
    var value: Bool = true
    let success = MemCache_get_bool(cache, cKey, &value)
    if success {
      return value
    } else {
      return nil
    }
  }
  
  func put(value: Int32, forKey key: String) -> Int32 {
    let cKey = key.cString(using: .utf8)
    return MemCache_put_int(cache, cKey, value)
  }
  
  func put(value: Bool, forKey key: String) -> Int32 {
    let cKey = key.cString(using: .utf8)
    return MemCache_put_bool(cache, cKey, value)
  }
  
  func put(value: Double, forKey key: String) -> Int32 {
    let cKey = key.cString(using: .utf8)
    return MemCache_put_double(cache, cKey, value)
  }
  
  func put(value: String, forKey key: String) -> Int32 {
    let cValue = value.cString(using: .utf8)
    let cKey = key.cString(using: .utf8)
    return MemCache_put_string(cache, cKey, cValue)
  }
  
  func putJson(value: String, forKey key: String) -> Int32 {
    let cValue = value.cString(using: .utf8)
    let cKey = key.cString(using: .utf8)
    return MemCache_put_json(cache, cKey, cValue)
  }
  
  func getJson(forkey key: String) -> String? {
    let cKey = key.cString(using: .utf8)
    var cValue: UnsafeMutablePointer<Int8>?
    
    let success = MemCache_get_json(cache, cKey, &cValue)
    let value: String?
    if success {
      value = String(cString: cValue!, encoding: .utf8)
    } else {
      value = nil
    }
    return value
  }
  
  func queryJson(forKey key: String, with path: String) -> String? {
    let cKey = key.cString(using: .utf8)
    let cPath = path.cString(using: .utf8)
    var cValue: UnsafeMutablePointer<Int8>?
    let success = MemCache_query_json(cache, cKey, cPath, &cValue)
    let value: String?
    if success {
      value = String(cString: cValue!, encoding: .utf8)
    } else {
      value = nil
    }
    return value
  }
  
  func modifyJson(value: String, withPath: String, forKey key: String) -> Int32 {
    let cValue = value.cString(using: .utf8)
    let cPath = withPath.cString(using: .utf8)
    let cKey = key.cString(using: .utf8)
    return MemCache_modify_json(cache, cKey, cPath, cValue)
  }
  
  func patchJson(patch: String, forKey key: String) -> Int32 {
    let cPatch = patch.cString(using: .utf8)
    let cKey = key.cString(using: .utf8)
    return MemCache_patch_json(cache, cKey, cPatch)
  }
  
  deinit {
    MemCache_delete(cache)
  }
}

```
