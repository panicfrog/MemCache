//
// Created by 叶永平 on 2023/5/12.
//

#ifndef MEMCACHE_MEMCACH_CPP_HPP
#define MEMCACHE_MEMCACH_CPP_HPP

#include <string>
#include "optional.hpp"

using namespace nonstd;

namespace memcache {
    template<typename T>
    int put(const std::string &key, const T &value) = delete; // 禁止使用未特化版本的put函数
    template<>
    int put<int>(const std::string &key, const int &value);

    template<>
    int put<double>(const std::string &key, const double &value);

    template<>
    int put<bool>(const std::string &key, const bool &value);

    template<>
    int put<std::string>(const std::string &key, const std::string &value);

    template<>
    int put<std::vector<std::uint8_t>>(const std::string &key, const std::vector<std::uint8_t> &value);

    template<typename T>
    int put(const std::vector<std::pair<std::string, T>> &kvs) = delete;

    template<>
    int put<std::string>(const std::vector<std::pair<std::string, std::string>> &kvs);

    template<>
    int put<int>(const std::vector<std::pair<std::string, int>> &kvs);

    template<>
    int put<double>(const std::vector<std::pair<std::string, double>> &kvs);

    template<>
    int put<bool>(const std::vector<std::pair<std::string, bool>> &kvs);

    template<>
    int put<std::vector<std::uint8_t>>(const std::vector<std::pair<std::string, std::vector<std::uint8_t>>> &kvs);

    template<typename T>
    optional<T> get(const std::string &key) = delete; // 禁止使用未特化版本的get函数
    template<>
    optional<int> get<int>(const std::string &key);

    template<>
    optional<double> get<double>(const std::string &key);

    template<>
    optional<bool> get<bool>(const std::string &key);

    template<>
    optional<std::string> get<std::string>(const std::string &key);

    template<>
    optional<std::vector<std::uint8_t>> get<std::vector<std::uint8_t>>(const std::string &key);

    int put_json(const std::string &key, const std::string &json);

    optional<std::string> get_json(const std::string &key);

    optional<std::string> query_json(const std::string &key, const std::string &json_path);

    int modify_json(const std::string &key, const std::string &json_path, const std::string &value);

    int patch_json(const std::string &key, const std::string &patch);
}

#endif //MEMCACHE_MEMCACH_CPP_HPP
