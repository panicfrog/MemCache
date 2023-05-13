//
// Created by 叶永平 on 2023/5/12.
//
#include "MemCache.hpp"
#include "MemCach_Cpp.hpp"

template <>
int put<int>(const std::string& key, const int& value) {
    return MemCache::getInstance()->put<int>(key, value);
}

template <>
int put<double>(const std::string& key, const double& value) {
    return MemCache::getInstance()->put<double>(key, value);
}

template <>
int put<bool>(const std::string& key, const bool& value) {
    return MemCache::getInstance()->put<bool>(key, value);
}

template <>
int put<std::string>(const std::string& key, const std::string& value) {
    return MemCache::getInstance()->put<std::string>(key, value);
}

template <>
int put<std::vector<std::uint8_t>>(const std::string& key, const std::vector<std::uint8_t>& value) {
    return MemCache::getInstance()->put<std::vector<std::uint8_t>>(key, value);
}

template <>
optional<int> get<int>(const std::string& key) {
    return MemCache::getInstance()->get<int>(key);
}

template <>
optional<double> get<double>(const std::string& key) {
    return MemCache::getInstance()->get<double>(key);
}

template <>
optional<bool> get<bool>(const std::string& key) {
    return MemCache::getInstance()->get<bool>(key);
}

template <>
optional<std::string> get<std::string>(const std::string& key) {
    return MemCache::getInstance()->get<std::string>(key);
}

template <>
optional<std::vector<std::uint8_t>> get<std::vector<std::uint8_t>>(const std::string& key) {
    return MemCache::getInstance()->get<std::vector<std::uint8_t>>(key);
}

int put_json(const std::string& key, const std::string& json) {
    return MemCache::getInstance()->put_json(key, json);
}

optional<std::string> get_json(const std::string& key) {
    return MemCache::getInstance()->get_json(key);
}

optional<std::string> query_json(const std::string& key, const std::string& json_path) {
    return MemCache::getInstance()->query_json(key, json_path);
}

int modify_json(const std::string& key, const std::string& json_path, const std::string& value) {
    return MemCache::getInstance()->modify_json(key, json_path, value);
}

int patch_json(const std::string& key, const std::string& patch) {
    return MemCache::getInstance()->patch_json(key, patch);
}