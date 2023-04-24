//
// Created by 叶永平 on 2023/4/24.
//
#include "MemCache.hpp"
#include "MemCache_C.h"

using nonstd::optional;
using nonstd::nullopt;

extern void *MemCache_new() {
    return new MemCache();
}

extern void MemCache_delete(void *cache) {
    auto *c = (MemCache* )cache;
    delete c;
}

extern int MemCache_put_string(void* cache, const char* key, const char* value) {
    auto *c = (MemCache* )cache;
    return c->put(key, std::string(value));
}

extern int MemCache_put_int(void* cache, const char* key, int value) {
    auto *c = (MemCache* )cache;
    return c->put(key, value);
}

extern int MemCache_put_double(void* cache, const char* key, double value) {
    auto *c = (MemCache* )cache;
    return c->put(key, value);
}

extern int MemCache_put_bool(void* cache, const char* key, bool value) {
    auto *c = (MemCache* )cache;
    return c->put(key, value);
}

extern int MemCache_put_bytes(void* cache, const char* key, const uint8_t* bytes, size_t size) {
    auto *c = (MemCache* )cache;
    return c->put(key, std::vector<uint8_t>(bytes, bytes + size));
}

extern bool MemCache_get_string(void* cache, const char* key, char **value) {
    auto *c = (MemCache* )cache;
    optional<std::string> v = c->get<std::string>(key);
    if (v.has_value()) {
        char *result = (char *)malloc(strlen(v.value().c_str()) + 1);
        strcpy(result, v.value().c_str());
        *value = result;
        return true;
    } else {
        return false;
    }
}

extern bool MemCache_get_int(void* cache, const char* key, int *value) {
    auto *c = (MemCache* )cache;
    optional<int> v = c->get<int>(key);
    if (v.has_value()) {
        *value = v.value();
        return true;
    } else {
        return false;
    }
}

extern bool MemCache_get_double(void* cache, const char* key, double *value) {
    auto *c = (MemCache* )cache;
    optional<double> v = c->get<double>(key);
    if (v.has_value()) {
        *value = v.value();
        return true;
    } else {
        return false;
    }
}

extern bool MemCache_get_bool(void* cache, const char* key, bool *value) {
    auto *c = (MemCache *)cache;
    optional<bool> v = c->get<bool>(key);
    if (v.has_value()) {
         *value = v.value();
        return true;
    } else {
        return false;
    }
}

extern bool MemCache_get_bytes(void* cache, const char* key, size_t* size, uint8_t*) {
    auto *c = (MemCache *)cache;
    optional<std::vector<uint8_t>> v = c->get<std::vector<uint8_t>>(key);
    if (v.has_value()) {
        std::vector<uint8_t> bytes = v.value();
        auto* result = new uint8_t[bytes.size()];
        std::copy(bytes.begin(), bytes.end(), result);
        *size = bytes.size();
        return true;
    } else {
        *size = 0;
        return false;
    }
}

extern int MemCache_put_json(void* cache, const char* key, const char* json) {
    auto *c = (MemCache*)cache;
    return c->put_json(key, json);
}

extern bool MemCache_get_json(void* cache, const char* key, char **value) {
    auto *c = (MemCache*)cache;
    optional<std::string> v = c->get_json(key);
    if (v.has_value()) {
        char *result = (char *)malloc(strlen(v.value().c_str()) + 1);
        strcpy(result, v.value().c_str());
        *value = result;
        return true;
    } else {
        return false;
    }
}

extern bool MemCache_query_json(void* cache, const char* key, const char* json_path, char **value) {
    auto *c = (MemCache*)cache;
    optional<std::string> v = c->query_json(key, json_path);
    if (v.has_value()) {
        char *result = (char *)malloc(strlen(v.value().c_str()) + 1);
        strcpy(result, v.value().c_str());
        *value = result;
        return true;
    } else {
        return false;
    }
}

extern int MemCache_modify_json(void* cache, const char* key, const char* json_path, const char* value) {
    auto *c = (MemCache*)cache;
    return c->modify_json(key, json_path, value);
}

extern int MemCache_patch_json(void* cache, const char* key, const char* patch) {
    auto *c = (MemCache*)cache;
    return c->patch_json(key, patch);
}
