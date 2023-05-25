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
    extern int MemCache_put_string(const char* key, const char* value);
    extern int MemCache_put_int(const char* key, int value);
    extern int MemCache_put_double(const char* key, double value);
    extern int MemCache_put_bool(const char* key, bool value);
    extern int MemCache_put_bytes(const char* key, const uint8_t* bytes, size_t size);

    extern int MemCache_put_strings(const char* const* key, const char* const* value, size_t size);
    extern int MemCache_put_ints(const char* const* key, const int* value, size_t size);
    extern int MemCache_put_doubles(const char* const* key, const double* value, size_t size);
    extern int MemCache_put_bools(const char* const* key, const bool* value, size_t size);

    extern int MemCache_delete_value(const char* key);

    extern bool MemCache_get_string(const char* key,  char ** value);
    extern bool MemCache_get_int(const char* key, int *value);
    extern bool MemCache_get_double(const char* key, double *value);
    extern bool MemCache_get_bool(const char* key, bool *value);
    extern bool MemCache_get_bytes(const char* key, size_t* size, uint8_t** result);

    extern int MemCache_delete_json(const char* key);

    extern int MemCache_put_json(const char* key, const char* json);
    extern bool MemCache_get_json(const char* key, char **value);
    extern bool MemCache_query_json(const char* key, const char* json_path, char **value);
    extern int MemCache_modify_json(const char* key, const char* json_path, const char* value);
    extern int MemCache_patch_json(const char* key, const char* patch);

#ifdef __cplusplus
}
#endif

#endif //MEMCACHE_MEMCACHE_C_H
