//
// Created by 叶永平 on 2023/4/24.
//
#include "MemCache.hpp"
#include "MemCache_C.h"

using nonstd::optional;
using nonstd::nullopt;
using namespace memcache;

extern int MemCache_put_string(const char* key, const char* value) {
    auto c = MemCache::getInstance();
    return c->put(key, std::string(value));
}

extern int MemCache_put_int(const char* key, int value) {
    auto c = MemCache::getInstance();
    return c->put(key, value);
}

extern int MemCache_put_double(const char* key, double value) {
    auto c = MemCache::getInstance();
    return c->put(key, value);
}

extern int MemCache_put_bool(const char* key, bool value) {
    auto c = MemCache::getInstance();
    return c->put(key, value);
}

extern int MemCache_put_bytes(const char* key, const uint8_t* bytes, size_t size) {
    auto c = MemCache::getInstance();
    return c->put(key, std::vector<uint8_t>(bytes, bytes + size));
}

 int MemCache_delete_value(const char* key) {
    auto c = MemCache::getInstance();
    return c->deleteValue(key);
}

int MemCache_put_strings(const char* const* key, const char* const* value, size_t size) {
    std::vector<std::pair<std::string, std::string>> kvs;
    kvs.reserve(size);
    for(int i = 0; i < size; ++i) {
        kvs.emplace_back(std::pair{std::string {*(key + i)}, std::string {*(value + i)}});
    }
    auto c = MemCache::getInstance();
    return c->put(kvs);
}

int MemCache_put_ints(const char* const* key, const int* value, size_t size) {
    std::vector<std::pair<std::string, int>> kvs;
    kvs.reserve(size);
    for(int i = 0; i < size; ++i) {
       kvs.emplace_back(std::pair{std::string {*(key + i)}, *(value + i)});
    }
    auto c = MemCache::getInstance();
    return c->put(kvs);
}

int MemCache_put_doubles(const char* const* key, const double* value, size_t size) {
    std::vector<std::pair<std::string, double>> kvs;
    kvs.reserve(size);
    for(int i = 0; i < size; ++i) {
        kvs.emplace_back(std::pair{std::string {*(key + i)}, *(value + i)});
    }
    auto c = MemCache::getInstance();
    return c->put(kvs);
}

int MemCache_put_bools(const char* const* key, const bool* value, size_t size) {
    std::vector<std::pair<std::string, bool>> kvs;
    kvs.reserve(size);
    for(int i = 0; i < size; ++i) {
        kvs.emplace_back(std::pair{std::string {*(key + i)}, *(value + i)});
    }
    auto c = MemCache::getInstance();
    return c->put(kvs);
}

extern bool MemCache_get_string(const char* key, char **value) {
    auto c = MemCache::getInstance();
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

extern bool MemCache_get_int(const char* key, int *value) {
    auto c = MemCache::getInstance();
    optional<int> v = c->get<int>(key);
    if (v.has_value()) {
        *value = v.value();
        return true;
    } else {
        return false;
    }
}

extern bool MemCache_get_double(const char* key, double *value) {
    auto c = MemCache::getInstance();
    optional<double> v = c->get<double>(key);
    if (v.has_value()) {
        *value = v.value();
        return true;
    } else {
        return false;
    }
}

extern bool MemCache_get_bool(const char* key, bool *value) {
    auto c = MemCache::getInstance();
    optional<bool> v = c->get<bool>(key);
    if (v.has_value()) {
         *value = v.value();
        return true;
    } else {
        return false;
    }
}

extern bool MemCache_get_bytes(const char* key, size_t* size, uint8_t*) {
    auto c = MemCache::getInstance();
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

extern int MemCache_delete_json(const char* key) {
    auto c = MemCache::getInstance();
    return c->deleteJson(key);
}

extern int MemCache_put_json(const char* key, const char* json) {
    auto c = MemCache::getInstance();
    return c->put_json(key, json);
}

extern bool MemCache_get_json(const char* key, char **value) {
    auto c = MemCache::getInstance();
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


extern bool MemCache_query_json(const char* key, const char* json_path, char **value) {
    auto c = MemCache::getInstance();
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

extern int MemCache_modify_json(const char* key, const char* json_path, const char* value) {
    auto c = MemCache::getInstance();
    return c->modify_json(key, json_path, value);
}

extern int MemCache_patch_json(const char* key, const char* patch) {
    auto c = MemCache::getInstance();
    return c->patch_json(key, patch);
}
