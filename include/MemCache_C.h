//
// Created by 叶永平 on 2023/4/24.
//

#ifndef MEMCACHE_MEMCACHE_C_H
#define MEMCACHE_MEMCACHE_C_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    extern void *MemCache_new();
    extern void MemCache_delete(void *cache);

    extern int MemCache_put_string(void* cache, const char* key, const char* value);
    extern int MemCache_put_int(void* cache, const char* key, int value);
    extern int MemCache_put_double(void* cache, const char* key, double value);
    extern int MemCache_put_bool(void* cache, const char* key, bool value);
    extern int MemCache_put_bytes(void* cache, const char* key, const uint8_t* bytes, size_t size);

    extern bool MemCache_get_string(void* cache, const char* key,  char *value);
    extern bool MemCache_get_int(void* cache, const char* key, int *value);
    extern bool MemCache_get_double(void* cache, const char* key, double *value);
    extern bool MemCache_get_bool(void* cache, const char* key, bool *value);
    extern bool MemCache_get_bytes(void* cache, const char* key, size_t* size, uint8_t*);

    extern int MemCache_put_json(void* cache, const char* key, const char* json);
    extern bool MemCache_get_json(void* cache, const char* key, char *value);
    extern bool MemCache_query_json(void* cache, const char* key, const char* json_path, char *value);
    extern int MemCache_modify_json(void* cache, const char* key, const char* json_path, const char* value);
    extern int MemCache_patch_json(void* cache, const char* key, const char* patch);

#ifdef __cplusplus
}
#endif

#endif //MEMCACHE_MEMCACHE_C_H
