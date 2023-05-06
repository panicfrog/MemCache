#ifndef _MEM_CACHE_HPP_
#define _MEM_CACHE_HPP_

#pragma once

#include <concepts>
#include <cstdint>
#include <string>
#include <vector>
#include <mutex>
#include "sqlite3.h"
#include "optional.hpp"

using nonstd::optional;
using nonstd::nullopt;

template <typename T>
concept CacheValue = std::is_same_v<T, std::string> || std::is_same_v<T, int> || std::is_same_v<T, double> || std::is_same_v<T, bool> || std::is_same_v<T, std::vector<std::uint8_t>>;

class MemCache {
public:
    ~MemCache();

    static MemCache* getInstance() {
        std::call_once(flag_, []() { instance = new MemCache(); });
        return instance;
    }

    MemCache(const MemCache&) = delete;
    MemCache& operator=(const MemCache&) = delete;

    template <CacheValue T>
    int put(const std::string& key, const T& value);

    template <CacheValue T>
    optional<T> get(const std::string& key) noexcept(false);

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