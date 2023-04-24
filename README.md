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
