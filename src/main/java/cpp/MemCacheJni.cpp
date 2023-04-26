//
// Created by 叶永平 on 2023/4/26.
//
#include <jni.h>
#include <string>

extern "C"
JNIEXPORT jstring JNICALL
Java_com_yongping_jmetcache_JniBridge_nativeMethod(JNIEnv *env, jobject thiz) {
    std::string hello = "MemCache string value";
    return env->NewStringUTF(hello.c_str());
}