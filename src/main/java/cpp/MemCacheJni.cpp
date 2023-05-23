//
// Created by 叶永平 on 2023/4/26.
//
#include <jni.h>
#include <string>
#include "../../../MemCache.hpp"

using namespace memcache;

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
    auto* cache = reinterpret_cast<MemCache*>(native_handle);
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
    auto* cache = reinterpret_cast<MemCache*>(native_handle);
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
    auto* cache = reinterpret_cast<MemCache*>(native_handle);
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
    auto* cache = reinterpret_cast<MemCache*>(native_handle);
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
    auto* cache = reinterpret_cast<MemCache*>(native_handle);
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
    auto* cache = reinterpret_cast<MemCache*>(native_handle);
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
    auto* cache = reinterpret_cast<MemCache*>(native_handle);
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
    auto* cache = reinterpret_cast<MemCache*>(native_handle);
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
    auto* cache = reinterpret_cast<MemCache*>(native_handle);
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
    auto* cache = reinterpret_cast<MemCache*>(native_handle);
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
    auto* cache = reinterpret_cast<MemCache*>(native_handle);
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
    auto* cache = reinterpret_cast<MemCache*>(native_handle);
    const char* key_chars = env->GetStringUTFChars(key, nullptr);
    std::string key_str(key_chars);
    env->ReleaseStringUTFChars(key, key_chars);

    const char* patch_chars = env->GetStringUTFChars(patch, nullptr);
    std::string patch_str(patch_chars);
    env->ReleaseStringUTFChars(patch, patch_chars);

    return cache->patch_json(key_str,patch_str);
}

int processInt(JNIEnv *env, jobject jObject) {
    jclass integerClass = env->FindClass("java/lang/Integer");
    jmethodID intValue = env->GetMethodID(integerClass, "intValue", "()I");
    jint value = env->CallIntMethod(jObject, intValue);
    return value;
}

double processDouble(JNIEnv *env, jobject jObject) {
    jclass doubleClass = env->FindClass("java/lang/Double");
    jmethodID doubleValue = env->GetMethodID(doubleClass, "doubleValue", "()D");
    jdouble value = env->CallDoubleMethod(jObject, doubleValue);
    return value;
}

bool processBoolean(JNIEnv *env, jobject jObject) {
    jclass booleanClass = env->FindClass("java/lang/Boolean");
    jmethodID booleanValue = env->GetMethodID(booleanClass, "booleanValue", "()Z");
    jboolean value = env->CallBooleanMethod(jObject, booleanValue);
    return value;
}

std::string processString(JNIEnv *env, jobject jObject) {
    auto jStr = (jstring) jObject;
    const char *rawString = env->GetStringUTFChars(jStr, 0);
    std::string result = std::string(rawString);
    env->ReleaseStringUTFChars(jStr, rawString);
    return result;
}

template <typename T>
std::vector<std::pair<std::string, T>> convert(JNIEnv *env, jobject jList, T(*processFunc)(JNIEnv*, jobject)) {
    std::vector<std::pair<std::string, T>> result;
    // 由于用户可能会创建超大量的数据会造成oom，所以这里需要添加异常处理
    try {
        jclass listClass = env->FindClass("java/util/List");
        jclass pairClass = env->FindClass("kotlin/Pair");

        jmethodID listSize = env->GetMethodID(listClass, "size", "()I");
        jmethodID listGet = env->GetMethodID(listClass, "get", "(I)Ljava/lang/Object;");
        jmethodID pairFirst = env->GetMethodID(pairClass, "getFirst", "()Ljava/lang/Object;");
        jmethodID pairSecond = env->GetMethodID(pairClass, "getSecond", "()Ljava/lang/Object;");

        jint size = env->CallIntMethod(jList, listSize);
        result.reserve(size);

        for (jint i = 0; i < size; ++i) {
            jobject jPair = env->CallObjectMethod(jList, listGet, i);
            auto jStr = (jstring) env->CallObjectMethod(jPair, pairFirst);
            jobject jObj = env->CallObjectMethod(jPair, pairSecond);

            const char *rawString = env->GetStringUTFChars(jStr, 0);
            std::string key = std::string(rawString);
            env->ReleaseStringUTFChars(jStr, rawString);

            T value = processFunc(env, jObj);

            result.emplace_back(std::make_pair(key, value));
        }
    } catch (const std::exception& e) {
        jclass newExcCls = env->FindClass("java/lang/RuntimeException");
        if (newExcCls == nullptr) {
            return result;
        }
        env->ThrowNew(newExcCls, e.what());
    }
    return result;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_yongping_jmemcache_MemCache_putInts(JNIEnv *env, jobject thiz, jlong native_handle,
                                             jobject pairs) {
    auto cache = reinterpret_cast<MemCache*>(native_handle);
    std::vector<std::pair<std::string, int>> intPairs = convert(env, pairs, &processInt);
    return cache->put(intPairs);
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_yongping_jmemcache_MemCache_putDoubles(JNIEnv *env, jobject thiz, jlong native_handle,
                                                jobject pairs) {
    auto cache = reinterpret_cast<MemCache*>(native_handle);
    std::vector<std::pair<std::string, double>> doublePairs = convert(env, pairs, &processDouble);
    return cache->put(doublePairs);
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_yongping_jmemcache_MemCache_putBools(JNIEnv *env, jobject thiz, jlong native_handle,
                                              jobject pairs) {
    auto cache = reinterpret_cast<MemCache*>(native_handle);
    std::vector<std::pair<std::string, bool>> boolPairs = convert(env, pairs, &processBoolean);
    return cache->put(boolPairs);
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_yongping_jmemcache_MemCache_putStrings(JNIEnv *env, jobject thiz, jlong native_handle,
                                                jobject pairs) {
    auto cache = reinterpret_cast<MemCache*>(native_handle);
    std::vector<std::pair<std::string, std::string>> stringPairs = convert(env, pairs, &processString);
    return cache->put(stringPairs);
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_yongping_jmemcache_MemCache_deleteValue(JNIEnv *env, jobject thiz, jlong native_handle,
                                                 jstring key) {
    auto* cache = reinterpret_cast<MemCache*>(native_handle);
    const char* key_chars = env->GetStringUTFChars(key, nullptr);
    std::string key_str(key_chars);
    env->ReleaseStringUTFChars(key, key_chars);

    auto result = cache->deleteValue(key_str);
    return result;
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_yongping_jmemcache_MemCache_deleteJson(JNIEnv *env, jobject thiz, jlong native_handle,
                                                jstring key) {
    auto* cache = reinterpret_cast<MemCache*>(native_handle);
    const char* key_chars = env->GetStringUTFChars(key, nullptr);
    std::string key_str(key_chars);
    env->ReleaseStringUTFChars(key, key_chars);

    auto result = cache->deleteJson(key_str);
    return result;
}