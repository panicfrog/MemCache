#ifndef _MEM_CACHE_HPP_
#define _MEM_CACHE_HPP_

#pragma once

#include <concepts>
#include <cstdint>
#include <string>
#include <vector>
#include <sqlite3.h>
#include <optional>

template <typename T>
concept CacheValue = std::is_same_v<T, std::string> || std::is_same_v<T, int> || std::is_same_v<T, double> || std::is_same_v<T, bool> || std::is_same_v<T, std::vector<std::uint8_t>>;

class MemCache {
public:
    MemCache();
    ~MemCache();

    template <CacheValue T>
    int put(const std::string& key, const T& value);

    template <CacheValue T>
    std::optional<T> get(const std::string& key) noexcept(false);

    int put_json(const std::string& key, const std::string& json);
    std::optional<std::string> get_json(const std::string& key);
    std::optional<std::string> query_json(const std::string& key, const std::string& json_path);
    int modify_json(const std::string& key, const std::string& json_path, const std::string& value);
    int patch_json(const std::string& key, const std::string& patch);

private:
    sqlite3* db;
};

#endif // _MEM_CACHE_HPP_