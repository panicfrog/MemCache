package com.yongping.jmemcache

import android.util.Log
import com.yongping.jmemcache.MemCacheTracingOption.Companion.has

sealed class MemCacheValueType {
    class Int(val value: kotlin.Int): MemCacheValueType()
    class Double(val value: kotlin.Double): MemCacheValueType()
    class Boolean(val value: kotlin.Boolean): MemCacheValueType()
    class String(val value: kotlin.String): MemCacheValueType()
    class BytesArray(val value: ByteArray) : MemCacheValueType()
}


class MemCache {
    companion object {
        init {
            System.loadLibrary("MemCache")
        }

        @JvmStatic
        fun onTracing(key: String, value: Any, size: Long, type: Int) {
            val allOptions = MemCacheTracingOption.GetOption.rawValue or MemCacheTracingOption.PutOption.rawValue
            val valueType = allOptions.inv() and type
            if (valueType < 1 || valueType > 5) {
                return
            }
            var _value: MemCacheValueType? = null
            when (valueType) {
                1 -> if (value is Int)  { // int
                       _value = MemCacheValueType.Int(value)
                   }
                2 -> if (value is Double) { // double
                    _value = MemCacheValueType.Double(value)
                }
                3 -> if (value is Boolean) { // bool
                    _value = MemCacheValueType.Boolean(value)
                }
                4 -> if (value is String){ // string
                    _value = MemCacheValueType.String(value)
                }
                5 -> if (value is ByteArray) { // byteArray
                    _value = MemCacheValueType.BytesArray(value)
                }
                else -> {
                    throw java.lang.UnsupportedOperationException()
                }
            }
            _value?.let { _value ->
                val _type: MemCacheTracingOption? = if (type.has(MemCacheTracingOption.GetOption)) {
                    MemCacheTracingOption.GetOption
                } else if (type.has(MemCacheTracingOption.PutOption)) {
                    MemCacheTracingOption.PutOption
                } else {
                    null
                }
                _type?.let {_type ->
                    Log.i("yyp", "onTracing key: $key, type: $_type, size: $size value: $value")
                }
            }
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

    private external fun putInts(nativeHandle: Long, pairs: List<Pair<String, Int>>): Int
    private external fun putDoubles(nativeHandle: Long, pairs: List<Pair<String, Double>>): Int
    private external fun putBools(nativeHandle: Long, pairs: List<Pair<String, Boolean>>): Int
    private external fun putStrings(nativeHandle: Long, pairs: List<Pair<String, String>>): Int

    private external fun getInt(nativeHandle: Long, key: String): Int?
    private external fun getDouble(nativeHandle: Long, key: String): Double?
    private external fun getBool(nativeHandle: Long, key: String): Boolean?
    private external fun getString(nativeHandle: Long, key: String): String?
    private external fun getBytes(nativeHandle: Long, key: String): ByteArray?
    private external fun deleteValue(nativeHandle: Long, key: String): Int

    private external fun putJson(nativeHandle: Long, key: String, json: String): Int
    private external fun getJson(nativeHandle: Long, key: String): String?
    private external fun queryJson(nativeHandle: Long, key: String, jsonPath: String): String?
    private external fun modifyJson(nativeHandle: Long, key: String, jsonPath: String, value: String): Int
    private external fun patchJson(nativeHandle: Long, key: String, patch: String): Int
    private external fun deleteJson(nativeHandle: Long, key: String): Int
    private external fun deleteJsonValue(nativeHandle: Long, key: String, jsonPath: String): Int
    private external fun tracing(nativeHandle: Long, key: String, type: Int): Int
    private external fun removeTracing(nativeHandle: Long, key: String, type: Int): Int

    private val nativeHandle: Long = createNativeInstance()

    fun putInt(key: String, value: Int): Int = putInt(nativeHandle, key, value)
    fun putDouble(key: String, value: Double): Int = putDouble(nativeHandle, key, value)
    fun putBool(key: String, value: Boolean): Int = putBool(nativeHandle, key, value)
    fun putString(key: String, value: String): Int = putString(nativeHandle, key, value)
    fun putBytes(key: String, value: ByteArray): Int = putBytes(nativeHandle, key, value)

    fun putInts(pairs: List<Pair<String, Int>>) = putInts(nativeHandle, pairs)
    fun putDoubles(pairs: List<Pair<String, Double>>) = putDoubles(nativeHandle, pairs)
    fun putBools(pairs: List<Pair<String, Boolean>>) = putBools(nativeHandle, pairs)
    fun putStrings(pairs: List<Pair<String, String>>) = putStrings(nativeHandle, pairs)

    fun getInt(key: String): Int? = getInt(nativeHandle, key)
    fun getDouble(key: String): Double? = getDouble(nativeHandle, key)
    fun getBool(key: String): Boolean? = getBool(nativeHandle, key)
    fun getString(key: String): String? = getString(nativeHandle, key)
    fun getBytes(key: String): ByteArray? = getBytes(nativeHandle, key)
    fun deleteValue(key: String): Int = deleteValue(nativeHandle, key)

    fun putJson(key: String, json: String): Int = putJson(nativeHandle, key, json)
    fun getJson(key: String): String? = getJson(nativeHandle, key)
    fun queryJson(key: String, jsonPath: String): String? = queryJson(nativeHandle, key, jsonPath)
    fun modifyJson(key: String, jsonPath: String, value: String): Int = modifyJson(nativeHandle, key, jsonPath, value)
    fun patchJson(key: String, patch: String): Int = patchJson(nativeHandle, key, patch)
    fun deleteJson(key: String): Int = deleteJson(nativeHandle, key)
    fun deleteJsonValue(key: String, jsonPath: String): Int = deleteJsonValue(nativeHandle, key, jsonPath)

    fun tracing(key: String, type: MemCacheTracingOption): Int {
        Log.i("yyp t", "$key, option: ${type.rawValue}")
        return tracing(nativeHandle, key, type.rawValue)
    }
    fun removeTracing(key: String, type: MemCacheTracingOption): Int = removeTracing(nativeHandle, key, type.rawValue)

}

enum class MemCacheTracingOption(val rawValue: Int) {
    GetOption(1 shl 4),
    PutOption(1 shl 5);

    companion object {
        operator fun invoke(vararg options: MemCacheTracingOption): Int {
            return options.fold(0) { acc, option -> acc or option.rawValue }
        }

        infix fun Int.has(option: MemCacheTracingOption): Boolean = (this and option.rawValue) != 0
    }
}

