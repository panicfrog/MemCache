//
// Created by 叶永平 on 2023/4/26.
//
#include <jni.h>
#include <string>
#include "MemCache.hpp"

extern "C"
JNIEXPORT jstring JNICALL
Java_com_yongping_jmemcache_MemCache_nativeMethod(JNIEnv *env, jobject thiz) {
    std::string hello = "MemCache string value 2";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT jlong JNICALL
Java_com_yongping_jmemcache_MemCache_createNativeInstance(JNIEnv *env, jobject thiz) {
    return reinterpret_cast<jlong>(MemCache::getInstance());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_yongping_jmemcache_MemCache_destroyNativeInstance(JNIEnv *env,
                                                           jobject thiz,
                                                           jlong native_handle) {
    // 不需要显式销毁
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_yongping_jmemcache_MemCache_putInt(JNIEnv *env,
                                            jobject thiz,
                                            jlong native_handle,
                                            jstring key,
                                            jint value) {
    auto* cache = reinterpret_cast<MemCache*>(native_handle);
    const char* key_chars = env->GetStringUTFChars(key, nullptr);
    std::string key_str(key_chars);
    env->ReleaseStringUTFChars(key, key_chars);
    return cache->put<int>(key_str, static_cast<int>(value));
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_yongping_jmemcache_MemCache_putDouble(JNIEnv *env,
                                               jobject thiz,
                                               jlong native_handle,
                                               jstring key,
                                               jdouble value) {
    auto* cache = reinterpret_cast<MemCache*>(native_handle);
    const char* key_chars = env->GetStringUTFChars(key, nullptr);
    std::string key_str(key_chars);
    env->ReleaseStringUTFChars(key, key_chars);
    return cache->put<double>(key_str, static_cast<double>(value));
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_yongping_jmemcache_MemCache_putBool(JNIEnv *env,
                                             jobject thiz,
                                             jlong native_handle,
                                             jstring key,
                                             jboolean value) {
    auto* cache = reinterpret_cast<MemCache*>(native_handle);
    const char* key_chars = env->GetStringUTFChars(key, nullptr);
    std::string key_str(key_chars);
    env->ReleaseStringUTFChars(key, key_chars);
    return cache->put<bool>(key_str, static_cast<bool>(value));
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_yongping_jmemcache_MemCache_putString(JNIEnv *env,
                                               jobject thiz,
                                               jlong native_handle,
                                               jstring key,
                                               jstring value) {
    MemCache* cache = reinterpret_cast<MemCache*>(native_handle);
    const char* key_chars = env->GetStringUTFChars(key, nullptr);
    std::string key_str(key_chars);
    env->ReleaseStringUTFChars(key, key_chars);
    const char* value_chars = env->GetStringUTFChars(value, nullptr);
    std::string value_str(value_chars);
    env->ReleaseStringUTFChars(value, value_chars);
    return cache->put<std::string>(key_str, value_str);
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_yongping_jmemcache_MemCache_putBytes(JNIEnv *env,
                                              jobject thiz,
                                              jlong native_handle,
                                              jstring key,
                                              jbyteArray value) {
    MemCache* cache = reinterpret_cast<MemCache*>(native_handle);
    const char* key_chars = env->GetStringUTFChars(key, nullptr);
    std::string key_str(key_chars);
    env->ReleaseStringUTFChars(key, key_chars);

    jsize len = env->GetArrayLength(value);
    jbyte* byteArrayElements = env->GetByteArrayElements(value, nullptr);
    std::vector<uint8_t> value_vec(byteArrayElements, byteArrayElements + len);
    env->ReleaseByteArrayElements(value, byteArrayElements, JNI_ABORT);

    return cache->put<std::vector<uint8_t>>(key_str, value_vec);
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_yongping_jmemcache_MemCache_getInt(JNIEnv *env,
                                            jobject thiz,
                                            jlong native_handle,
                                            jstring key) {
    MemCache* cache = reinterpret_cast<MemCache*>(native_handle);
    const char* key_chars = env->GetStringUTFChars(key, nullptr);
    std::string key_str(key_chars);
    env->ReleaseStringUTFChars(key, key_chars);

    auto result = cache->get<int>(key_str);
    if (!result) {
        return nullptr;
    }

    jclass integerClass = env->FindClass("java/lang/Integer");
    jmethodID constructor = env->GetMethodID(integerClass, "<init>", "(I)V");
    return env->NewObject(integerClass, constructor, static_cast<jint>(result.value()));
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_yongping_jmemcache_MemCache_getDouble(JNIEnv *env,
                                               jobject thiz,
                                               jlong native_handle,
                                               jstring key) {
    MemCache* cache = reinterpret_cast<MemCache*>(native_handle);
    const char* key_chars = env->GetStringUTFChars(key, nullptr);
    std::string key_str(key_chars);
    env->ReleaseStringUTFChars(key, key_chars);

    auto result = cache->get<double>(key_str);
    if (!result) {
        return nullptr;
    }

    jclass doubleClass = env->FindClass("java/lang/Double");
    jmethodID constructor = env->GetMethodID(doubleClass, "<init>", "(D)V");
    return env->NewObject(doubleClass, constructor, static_cast<jdouble>(result.value()));
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_yongping_jmemcache_MemCache_getBool(JNIEnv *env,
                                             jobject thiz,
                                             jlong native_handle,
                                             jstring key) {
    MemCache* cache = reinterpret_cast<MemCache*>(native_handle);
    const char* key_chars = env->GetStringUTFChars(key, nullptr);
    std::string key_str(key_chars);
    env->ReleaseStringUTFChars(key, key_chars);

    auto result = cache->get<bool>(key_str);
    if (!result) {
        return nullptr;
    }

    jclass booleanClass = env->FindClass("java/lang/Boolean");
    jmethodID constructor = env->GetMethodID(booleanClass, "<init>", "(Z)V");
    return env->NewObject(booleanClass, constructor, static_cast<jboolean>(result.value()));
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_yongping_jmemcache_MemCache_getString(JNIEnv *env,
                                               jobject thiz,
                                               jlong native_handle,
                                               jstring key) {
    MemCache* cache = reinterpret_cast<MemCache*>(native_handle);
    const char* key_chars = env->GetStringUTFChars(key, nullptr);
    std::string key_str(key_chars);
    env->ReleaseStringUTFChars(key, key_chars);

    auto result = cache->get<std::string>(key_str);
    if (!result) {
        return nullptr;
    }

    return env->NewStringUTF(result.value().c_str());
}
extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_yongping_jmemcache_MemCache_getBytes(JNIEnv *env,
                                              jobject thiz,
                                              jlong native_handle,
                                              jstring key) {
    // TODO: implement getBytes()
    MemCache* cache = reinterpret_cast<MemCache*>(native_handle);
    const char* key_chars = env->GetStringUTFChars(key, nullptr);
    std::string key_str(key_chars);
    env->ReleaseStringUTFChars(key, key_chars);

    auto result = cache->get<std::vector<uint8_t>>(key_str);
    if (!result) {
        return nullptr;
    }

    jbyteArray byteArray = env->NewByteArray(static_cast<jsize>(result.value().size()));
    env->SetByteArrayRegion(byteArray,0, static_cast<jsize>(result.value().size()), reinterpret_cast<const jbyte*>(result.value().data()));
    return byteArray;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_yongping_jmemcache_MemCache_putJson(JNIEnv *env,
                                             jobject thiz,
                                             jlong native_handle,
                                             jstring key,
                                             jstring json) {
    MemCache* cache = reinterpret_cast<MemCache*>(native_handle);
    const char* key_chars = env->GetStringUTFChars(key, nullptr);
    std::string key_str(key_chars);
    env->ReleaseStringUTFChars(key, key_chars);

    const char* json_chars = env->GetStringUTFChars(json, nullptr);
    std::string json_str(json_chars);
    env->ReleaseStringUTFChars(json, json_chars);

    return cache->put_json(key_str, json_str);
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_yongping_jmemcache_MemCache_getJson(JNIEnv *env,
                                             jobject thiz,
                                             jlong native_handle,
                                             jstring key) {
    MemCache* cache = reinterpret_cast<MemCache*>(native_handle);
    const char* key_chars = env->GetStringUTFChars(key, nullptr);
    std::string key_str(key_chars);
    env->ReleaseStringUTFChars(key, key_chars);

    auto result = cache->get_json(key_str);
    if (!result) {
        return nullptr;
    }

    return env->NewStringUTF(result.value().c_str());
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_yongping_jmemcache_MemCache_queryJson(JNIEnv *env,
                                               jobject thiz,
                                               jlong native_handle,
                                               jstring key,
                                               jstring json_path) {
    MemCache* cache = reinterpret_cast<MemCache*>(native_handle);
    const char* key_chars = env->GetStringUTFChars(key, nullptr);
    std::string key_str(key_chars);
    env->ReleaseStringUTFChars(key, key_chars);

    const char* json_path_chars = env->GetStringUTFChars(json_path, nullptr);
    std::string json_path_str(json_path_chars);
    env->ReleaseStringUTFChars(json_path, json_path_chars);

    auto result = cache->query_json(key_str, json_path_str);
    if (!result) {
        return nullptr;
    }

    return env->NewStringUTF(result.value().c_str());
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_yongping_jmemcache_MemCache_modifyJson(JNIEnv *env,
                                                jobject thiz,
                                                jlong native_handle,
                                                jstring key,
                                                jstring json_path,
                                                jstring value) {
    MemCache* cache = reinterpret_cast<MemCache*>(native_handle);
    const char* key_chars = env->GetStringUTFChars(key, nullptr);
    std::string key_str(key_chars);
    env->ReleaseStringUTFChars(key, key_chars);

    const char* json_path_chars = env->GetStringUTFChars(json_path, nullptr);
    std::string json_path_str(json_path_chars);
    env->ReleaseStringUTFChars(json_path, json_path_chars);

    const char* value_chars = env->GetStringUTFChars(value, nullptr);
    std::string value_str(value_chars);
    env->ReleaseStringUTFChars(value, value_chars);

    return cache->modify_json(key_str, json_path_str, value_str);
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_yongping_jmemcache_MemCache_patchJson(JNIEnv *env,
                                               jobject thiz,
                                               jlong native_handle,
                                               jstring key,
                                               jstring patch) {
    MemCache* cache = reinterpret_cast<MemCache*>(native_handle);
    const char* key_chars = env->GetStringUTFChars(key, nullptr);
    std::string key_str(key_chars);
    env->ReleaseStringUTFChars(key, key_chars);

    const char* patch_chars = env->GetStringUTFChars(patch, nullptr);
    std::string patch_str(patch_chars);
    env->ReleaseStringUTFChars(patch, patch_chars);

    return cache->patch_json(key_str,patch_str);
}
