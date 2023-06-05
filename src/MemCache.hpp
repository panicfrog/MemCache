#ifndef _MEM_CACHE_HPP_
#define _MEM_CACHE_HPP_

#ifndef MEM_CACHE_USE_MULTITHREAD
#define MEM_CACHE_USE_MULTITHREAD 0
#endif

#if defined(__GNUC__) || defined(__clang__)
#define UNUSED __attribute__((unused))
#elif defined(_MSC_VER)
#define UNUSED [[maybe_unused]]
#else
#define UNUSED
#endif

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <mutex>
#include <type_traits>
#include <utility>
#include "sqlite3.h"
#include "optional.hpp"
#include "StmtWrapper.hpp"

#if MEM_CACHE_USE_MULTITHREAD
#include "Connect.hpp"
#endif

using nonstd::optional;
using nonstd::nullopt;

namespace memcache {

    enum class StmtType {
        get, put, delete_value, get_json, put_json, modify_json, query_json, patch_json, delete_json, delete_json_value
    };

    template<typename T>
    using is_memcache_value_type = std::disjunction<
            std::is_same<T, std::string>,
            std::is_same<T, int>,
            std::is_same<T, double>,
            std::is_same<T, bool>,
            std::is_same<T, std::vector<uint8_t>>>;

    class MemCache {
    public:
#if MEM_CACHE_USE_MULTITHREAD
        ~MemCache() = default;
#else

        ~MemCache();

#endif

        static MemCache *getInstance() {
            std::call_once(flag_, []() { instance = new MemCache(); });
            return instance;
        }

        MemCache(const MemCache &) = delete;

        MemCache &operator=(const MemCache &) = delete;

        template<typename T, typename = typename std::enable_if<is_memcache_value_type<T>::value>::type>
        int put(const std::string &key, const T &value);

        template<typename T, typename = typename std::enable_if<is_memcache_value_type<T>::value>::type>
        optional<T> get(const std::string &key);

        int deleteValue(const std::string& key);

        template<typename T, typename = typename std::enable_if<is_memcache_value_type<T>::value>::type>
        int put(const std::vector<std::pair<std::string, T>> &kvs);

        int put_json(const std::string &key, const std::string &json);

        optional<std::string> get_json(const std::string &key);

        int deleteJson(const std::string& key);

        optional<std::string> query_json(const std::string &key, const std::string &json_path);

        int modify_json(const std::string &key, const std::string &json_path, const std::string &value);

        int patch_json(const std::string &key, const std::string &patch);

        int delete_json_value(const std::string &key, const std::string &json_path);

    private:
        MemCache();

        static sqlite3_stmt *prepareStatements(StmtType type, sqlite3 *db);

#if MEM_CACHE_USE_MULTITHREAD
        static sqlite3* get_db();
#endif

        static MemCache *instance;
        static std::once_flag flag_;

        thread_local static std::unique_ptr<StmtWrapper> put_stmt;
        thread_local static std::unique_ptr<StmtWrapper> get_stmt;
        thread_local static std::unique_ptr<StmtWrapper> put_json_stmt;
        thread_local static std::unique_ptr<StmtWrapper> get_json_stmt;
        thread_local static std::unique_ptr<StmtWrapper> query_json_stmt;
        thread_local static std::unique_ptr<StmtWrapper> modify_json_stmt;
        thread_local static std::unique_ptr<StmtWrapper> patch_json_stmt;
        thread_local static std::unique_ptr<StmtWrapper> delete_value_stmt;
        thread_local static std::unique_ptr<StmtWrapper> delete_json_stmt;
        thread_local static std::unique_ptr<StmtWrapper> delete_json_value_stmt;

#if MEM_CACHE_USE_MULTITHREAD
        thread_local static std::unique_ptr<Connect> db_connect;
#else
        sqlite3 *db{};
#endif
    };

}

#endif // _MEM_CACHE_HPP_