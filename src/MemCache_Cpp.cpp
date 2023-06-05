//
// Created by 叶永平 on 2023/5/12.
//
#include "MemCache.hpp"
#include "MemCach_Cpp.hpp"

namespace memcache {

    template<>
    int put<int>(const std::string &key, const int &value) {
        return MemCache::getInstance()->put<int>(key, value);
    }

    template<>
    int put<double>(const std::string &key, const double &value) {
        return MemCache::getInstance()->put<double>(key, value);
    }

    template<>
    int put<bool>(const std::string &key, const bool &value) {
        return MemCache::getInstance()->put<bool>(key, value);
    }

    template<>
    int put<std::string>(const std::string &key, const std::string &value) {
        return MemCache::getInstance()->put<std::string>(key, value);
    }

    template<>
    int put<std::vector<std::uint8_t>>(const std::string &key, const std::vector<std::uint8_t> &value) {
        return MemCache::getInstance()->put<std::vector<std::uint8_t>>(key, value);
    }

    template<>
    int put<std::string>(const std::vector<std::pair<std::string, std::string>> &kvs) {
        return MemCache::getInstance()->put<std::string>(kvs);
    }

    UNUSED
    int deleteValue(const std::string& key) {
        return MemCache::getInstance()->deleteValue(key);
    }

    template<>
    int put<int>(const std::vector<std::pair<std::string, int>> &kvs) {
        return MemCache::getInstance()->put<int>(kvs);
    }

    template<>
    int put<double>(const std::vector<std::pair<std::string, double>> &kvs) {
        return MemCache::getInstance()->put<double>(kvs);
    }

    template<>
    int put<bool>(const std::vector<std::pair<std::string, bool>> &kvs) {
        return MemCache::getInstance()->put<bool>(kvs);
    }

    template<>
    int put<std::vector<std::uint8_t>>(const std::vector<std::pair<std::string, std::vector<std::uint8_t>>> &kvs) {
        return MemCache::getInstance()->put<std::vector<std::uint8_t>>(kvs);
    }

    template<>
    optional<int> get<int>(const std::string &key) {
        return MemCache::getInstance()->get<int>(key);
    }

    template<>
    optional<double> get<double>(const std::string &key) {
        return MemCache::getInstance()->get<double>(key);
    }

    template<>
    optional<bool> get<bool>(const std::string &key) {
        return MemCache::getInstance()->get<bool>(key);
    }

    template<>
    optional<std::string> get<std::string>(const std::string &key) {
        return MemCache::getInstance()->get<std::string>(key);
    }

    template<>
    optional<std::vector<std::uint8_t>> get<std::vector<std::uint8_t>>(const std::string &key) {
        return MemCache::getInstance()->get<std::vector<std::uint8_t>>(key);
    }

    UNUSED
    int deleteJson(const std::string& key) {
        return MemCache::getInstance()->deleteJson(key);
    }

    UNUSED
    int put_json(const std::string &key, const std::string &json) {
        return MemCache::getInstance()->put_json(key, json);
    }

    UNUSED
    optional<std::string> get_json(const std::string &key) {
        return MemCache::getInstance()->get_json(key);
    }

    UNUSED
    optional<std::string> query_json(const std::string &key, const std::string &json_path) {
        return MemCache::getInstance()->query_json(key, json_path);
    }

    UNUSED
    int modify_json(const std::string &key, const std::string &json_path, const std::string &value) {
        return MemCache::getInstance()->modify_json(key, json_path, value);
    }

    UNUSED
    int patch_json(const std::string &key, const std::string &patch) {
        return MemCache::getInstance()->patch_json(key, patch);
    }

    UNUSED
    int delete_json_value(const std::string &key, const std::string &json_path) {
        return MemCache::getInstance()->delete_json_value(key, json_path);
    }

}