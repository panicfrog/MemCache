#include <iostream>

#include "MemCache.hpp"

#define CACHE_TYPE_INT 1
#define CACHE_TYPE_DOUBLE 2
#define CACHE_TYPE_BOOL 3
#define CACHE_TYPE_STRING 4
#define CACHE_TYPE_UINT8_ARRAY 5

using nonstd::optional;
using nonstd::nullopt;

MemCache* MemCache::instance = nullptr;
std::once_flag MemCache::flag_;

MemCache::MemCache() {
    sqlite3_open(":memory:", &db);
//    int mode = sqlite3_threadsafe();
//    std::cout << "SQLite thread safety mode: " << mode << std::endl;
    sqlite3_exec(db, "CREATE TABLE cache (key TEXT PRIMARY KEY NOT NULL, type INTEGER NOT NULL, value BLOB NOT NULL);", nullptr, nullptr, nullptr);
    sqlite3_exec(db, "CREATE TABLE json_cache (key TEXT PRIMARY KEY NOT NULL, json JSON NOT NULL CHECK (json_valid(json)));", nullptr, nullptr, nullptr);
}

MemCache::~MemCache() {
    sqlite3_close(db);
}

int MemCache::put_json(const std::string& key, const std::string& json) {
    std::string sql = "INSERT OR REPLACE INTO json_cache (key, json) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, key.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, json.c_str(), -1, SQLITE_STATIC);

    auto result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return result;
}

optional<std::string> MemCache::get_json(const std::string& key) {
    std::string sql = "SELECT json FROM json_cache WHERE key = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, key.c_str(), -1, SQLITE_STATIC);

    std::string json;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        json = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        sqlite3_finalize(stmt);
        return  json;
    }
    sqlite3_finalize(stmt);
    return nullopt;
}

optional<std::string>  MemCache::query_json(const std::string& key, const std::string& json_path) {
    std::string sql = "SELECT json_extract(json, ? ) FROM json_cache WHERE key = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, json_path.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, key.c_str(), -1, SQLITE_STATIC);
    std::string value;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        sqlite3_finalize(stmt);
        return value;
    }
    sqlite3_finalize(stmt);
    return nullopt;
}

int MemCache::modify_json(const std::string& key, const std::string& json_path, const std::string& value) {
    std::string sql = "UPDATE json_cache SET json = json_set(json, ?, ?) WHERE key = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, json_path.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, value.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, key.c_str(), -1, SQLITE_STATIC);
    auto result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return result;
}

int MemCache::patch_json(const std::string& key, const std::string& patch) {
    std::string sql = "UPDATE json_cache SET json = (CASE WHEN json_valid(?) THEN json_patch(json, ?) ELSE json END) WHERE key = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, patch.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, patch.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, key.c_str(), -1, SQLITE_STATIC);
    auto result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    return result;
}

template <typename T, typename std::enable_if<std::is_same<T, std::string>::value || std::is_same<T, int>::value || std::is_same<T, double>::value || std::is_same<T, bool>::value || std::is_same<T, std::vector<std::uint8_t>>::value, int>::type = 0>
int put(const std::string& key, const T& value);

#define PUT_IMPL(Type, BindFunc, ...) \
    std::string sql = "INSERT OR REPLACE INTO cache (key, type, value) VALUES (?, ?, ?);"; \
    sqlite3_stmt* stmt; \
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr); \
    sqlite3_bind_text(stmt, 1, key.c_str(), -1, SQLITE_STATIC); \
    sqlite3_bind_int(stmt, 2, Type); \
    BindFunc(stmt, 3, __VA_ARGS__); \
    int result = sqlite3_step(stmt); \
    sqlite3_finalize(stmt); \
    return result;

template <>
int MemCache::put<int>(const std::string& key, const int& value) {
    PUT_IMPL(CACHE_TYPE_INT, sqlite3_bind_int, value)
}

template <>
int MemCache::put<double>(const std::string& key, const double& value) {
    PUT_IMPL(CACHE_TYPE_DOUBLE, sqlite3_bind_double, value)
}

template <>
int MemCache::put<bool>(const std::string& key, const bool& value) {
    PUT_IMPL(CACHE_TYPE_BOOL, sqlite3_bind_int, static_cast<int>(value))
}

template <>
int MemCache::put<std::string>(const std::string& key, const std::string& value) {
    PUT_IMPL(CACHE_TYPE_STRING, sqlite3_bind_text, value.c_str(), -1, SQLITE_STATIC)
}

template <>
int MemCache::put<std::vector<std::uint8_t>>(const std::string& key, const std::vector<std::uint8_t>& value) {
    PUT_IMPL(CACHE_TYPE_UINT8_ARRAY, sqlite3_bind_blob, std::data(value), value.size(), SQLITE_STATIC)
}

#define GET_IMPL(Type, Transfer) \
    std::string sql = "SELECT type, value FROM cache WHERE key = ?;"; \
    sqlite3_stmt* stmt; \
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr); \
    sqlite3_bind_text(stmt, 1, key.c_str(), -1, SQLITE_STATIC); \
    auto result = sqlite3_step(stmt); \
    if (result == SQLITE_ROW) { \
        int type = sqlite3_column_int(stmt, 0); \
        if (type == Type) { \
            Transfer; \
            sqlite3_finalize(stmt); \
            return value; \
        } else { \
            sqlite3_finalize(stmt); \
            return nullopt; \
        } \
    } else { \
        sqlite3_finalize(stmt); \
        return nullopt; \
    }

template <>
optional<int> MemCache::get<int>(const std::string& key) {
    GET_IMPL(CACHE_TYPE_INT, int value = sqlite3_column_int(stmt, 1))
}

template <>
optional<double> MemCache::get<double>(const std::string& key) {
    GET_IMPL(CACHE_TYPE_DOUBLE, double value = sqlite3_column_double(stmt, 1))
}

template <>
optional<bool> MemCache::get<bool>(const std::string& key) {
    GET_IMPL(CACHE_TYPE_BOOL, bool value = static_cast<bool>(sqlite3_column_int(stmt, 1)))
}

template <>
optional<std::string> MemCache::get<std::string>(const std::string& key) {
    GET_IMPL(CACHE_TYPE_STRING, std::string value = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))))
}

template <>
optional<std::vector<std::uint8_t>> MemCache::get<std::vector<std::uint8_t>>(const std::string& key) {
    std::string sql = "SELECT type, value FROM cache WHERE key = ?;";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, key.c_str(), -1, SQLITE_STATIC);
    auto result = sqlite3_step(stmt);
    if (result == SQLITE_ROW) {
        int type = sqlite3_column_int(stmt, 0);
            if (type == CACHE_TYPE_UINT8_ARRAY) {
                const void *blob = sqlite3_column_blob(stmt, 1);
                int size = sqlite3_column_bytes(stmt, 1);
                auto value = std::vector<std::uint8_t>(reinterpret_cast<const std::uint8_t *>(blob),
                                                       reinterpret_cast<const std::uint8_t *>(blob) + size);
                sqlite3_finalize(stmt);
                return value;
        } else {
            return nullopt;
        }
    } else {
        sqlite3_finalize(stmt);
        return nullopt;
    }
}
