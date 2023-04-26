package com.yongping.jmetcache;

import android.util.Log;

public class JniBridge {
    static {
        Log.i("--------- yyp -------- cache", "初始化1");
        System.loadLibrary("MemCache");
        Log.i("--------- yyp -------- cache", "初始化2");
    }

    public native String nativeMethod();
}