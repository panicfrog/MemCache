package com.yongping.jmemcache

class MemCache {
    companion object {
        init {
            System.loadLibrary("MemCache")
        }
    }
    external fun nativeMethod(): String;

    private external fun createNativeInstance(): Long
    private external fun destroyNativeInstance(nativeHandle: Long)

    private external fun putInt(nativeHandle: Long, key: String, value: Int): Int
    private external fun putDouble(nativeHandle: Long, key: String, value: Double): Int
    private external fun putBool(nativeHandle: Long, key: String, value: Boolean): Int
    private external fun putString(nativeHandle: Long, key: String, value: String): Int
    private external fun putBytes(nativeHandle: Long, key: String, value: ByteArray): Int

    private external fun getInt(nativeHandle: Long, key: String): Int?
    private external fun getDouble(nativeHandle: Long, key: String): Double?
    private external fun getBool(nativeHandle: Long, key: String): Boolean?
    private external fun getString(nativeHandle: Long, key: String): String?
    private external fun getBytes(nativeHandle: Long, key: String): ByteArray?

    private external fun putJson(nativeHandle: Long, key: String, json: String): Int
    private external fun getJson(nativeHandle: Long, key: String): String?
    private external fun queryJson(nativeHandle: Long, key: String, jsonPath: String): String?
    private external fun modifyJson(nativeHandle: Long, key: String, jsonPath: String, value: String): Int
    private external fun patchJson(nativeHandle: Long, key: String, patch: String): Int

    private val nativeHandle: Long = createNativeInstance()

    fun putInt(key: String, value: Int): Int = putInt(nativeHandle, key, value)
    fun putDouble(key: String, value: Double): Int = putDouble(nativeHandle, key, value)
    fun putBool(key: String, value: Boolean): Int = putBool(nativeHandle, key, value)
    fun putString(key: String, value: String): Int = putString(nativeHandle, key, value)
    fun putBytes(key: String, value: ByteArray): Int = putBytes(nativeHandle, key, value)

    fun getInt(key: String): Int? = getInt(nativeHandle, key)
    fun getDouble(key: String): Double? = getDouble(nativeHandle, key)
    fun getBool(key: String): Boolean? = getBool(nativeHandle, key)
    fun getString(key: String): String? = getString(nativeHandle, key)
    fun getBytes(key: String): ByteArray? = getBytes(nativeHandle, key)

    fun putJson(key: String, json: String): Int = putJson(nativeHandle, key, json)
    fun getJson(key: String): String? = getJson(nativeHandle, key)
    fun queryJson(key: String, jsonPath: String): String? = queryJson(nativeHandle, key, jsonPath)
    fun modifyJson(key: String, jsonPath: String, value: String): Int = modifyJson(nativeHandle, key, jsonPath, value)
    fun patchJson(key: String, patch: String): Int = patchJson(nativeHandle, key, patch)

}