#ifndef _MEM_CACHE_HPP_
#define _MEM_CACHE_HPP_

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <mutex>
#include <type_traits>
#include "sqlite3.h"
#include "optional.hpp"

using nonstd::optional;
using nonstd::nullopt;

class MemCache {
public:
    ~MemCache();

    static MemCache* getInstance() {
        std::call_once(flag_, []() { instance = new MemCache(); });
        return instance;
    }

    MemCache(const MemCache&) = delete;
    MemCache& operator=(const MemCache&) = delete;

    template <typename T>
    int put(const std::string& key, const T& value) = delete; // 禁止使用未特化版本的put函数

    template <>
    int put<int>(const std::string& key, const int& value);

    template <>
    int put<double>(const std::string& key, const double& value);

    template <>
    int put<bool>(const std::string& key, const bool& value);

    template <>
    int put<std::string>(const std::string& key, const std::string& value);

    template <>
    int put<std::vector<std::uint8_t>>(const std::string& key, const std::vector<std::uint8_t>& value);

    template <typename T>
    optional<T> get(const std::string& key) = delete; // 禁止使用未特化版本的get函数

    template <>
    optional<int> get<int>(const std::string& key);

    template <>
    optional<double> get<double>(const std::string& key);

    template <>
    optional<bool> get<bool>(const std::string& key);

    template <>
    optional<std::string> get<std::string>(const std::string& key);

    template <>
    optional<std::vector<std::uint8_t>> get<std::vector<std::uint8_t>>(const std::string& key);

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