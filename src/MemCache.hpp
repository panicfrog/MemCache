#ifndef _MEM_CACHE_HPP_
#define _MEM_CACHE_HPP_

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <mutex>
#include <type_traits>
#include <utility>
#include "sqlite3.h"
#include "optional.hpp"

using nonstd::optional;
using nonstd::nullopt;

template <typename T>
using is_memcache_value_type = std::disjunction<
    std::is_same<T, std::string>,
    std::is_same<T, int>,
    std::is_same<T, double>,
    std::is_same<T, bool>,
    std::is_same<T, std::vector<uint8_t>>>;

class MemCache {
public:
    ~MemCache();

    static MemCache* getInstance() {
        std::call_once(flag_, []() { instance = new MemCache(); });
        return instance;
    }

    MemCache(const MemCache&) = delete;
    MemCache& operator=(const MemCache&) = delete;

    template <typename T, typename = typename std::enable_if<is_memcache_value_type<T>::value>::type>
    int put(const std::string& key, const T& value);

    template <typename T, typename = typename std::enable_if<is_memcache_value_type<T>::value>::type>
    optional<T> get(const std::string& key);

    template <typename T, typename = typename std::enable_if<is_memcache_value_type<T>::value>::type>
    int put(const std::vector<std::pair<std::string, T>>& kvs);

    int put_json(const std::string& key, const std::string& json);
    optional<std::string> get_json(const std::string& key);
    optional<std::string> query_json(const std::string& key, const std::string& json_path);
    int modify_json(const std::string& key, const std::string& json_path, const std::string& value);
    int patch_json(const std::string& key, const std::string& patch);

private:
    MemCache();
    sqlite3* db;
    static MemCache* instance;
    static std::once_flag flag_;
};

#endif // _MEM_CACHE_HPP_