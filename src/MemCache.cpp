
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

template<typename T, typename>
int MemCache::put(const std::vector<std::pair<std::string, T>>& kvs) {
    std::string sql = "INSERT OR REPLACE INTO cache (key, type, value) VALUES (?, ?, ?);";
    sqlite3_stmt *stmt;
    char *zErrMsg = nullptr;

    sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, &zErrMsg);
    if (zErrMsg != nullptr) {
        return 0;
    }
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
   if constexpr (std::is_same_v<T, int>) {
       for(const auto& kv : kvs ) {
           sqlite3_bind_text(stmt, 1, kv.first.c_str(), -1, SQLITE_STATIC);
           sqlite3_bind_int(stmt, 2, CACHE_TYPE_INT);
           sqlite3_bind_int(stmt, 3, kv.second);
           int result = sqlite3_step(stmt);
           if (result != SQLITE_DONE) {
               sqlite3_finalize(stmt);
               return result;
           }
           sqlite3_reset(stmt);
       }
   } else if constexpr (std::is_same_v<T, double>) {
       for(const auto& kv : kvs ) {
           sqlite3_bind_text(stmt, 1, kv.first.c_str(), -1, SQLITE_STATIC);
           sqlite3_bind_int(stmt, 2, CACHE_TYPE_DOUBLE);
           sqlite3_bind_double(stmt, 3, kv.second);
           int result = sqlite3_step(stmt);
           if (result != SQLITE_DONE) {
               sqlite3_finalize(stmt);
               return result;
           }
           sqlite3_reset(stmt);
       }
   } else if constexpr (std::is_same_v<T, bool>) {
       for(const auto& kv : kvs ) {
           sqlite3_bind_text(stmt, 1, kv.first.c_str(), -1, SQLITE_STATIC);
           sqlite3_bind_int(stmt, 2, CACHE_TYPE_BOOL);
           sqlite3_bind_int(stmt, 3, static_cast<int>(kv.second));
           int result = sqlite3_step(stmt);
           if (result != SQLITE_DONE) {
               sqlite3_finalize(stmt);
               return result;
           }
           sqlite3_reset(stmt);
       }
   } else if constexpr (std::is_same_v<T, std::string>) {
       for(const auto& kv : kvs ) {
           sqlite3_bind_text(stmt, 1, kv.first.c_str(), -1, SQLITE_STATIC);
           sqlite3_bind_int(stmt, 2, CACHE_TYPE_STRING);
           sqlite3_bind_text(stmt, 3, kv.second.c_str(), -1, SQLITE_STATIC);
           int result = sqlite3_step(stmt);
           if (result != SQLITE_DONE) {
               sqlite3_finalize(stmt);
               return result;
           }
           sqlite3_reset(stmt);
       }
   } else if constexpr (std::is_same_v<T, std::vector<std::uint8_t>>) {
       for(auto kv : kvs ) {
           sqlite3_bind_text(stmt, 1, kv.first.c_str(), -1, SQLITE_STATIC);
           sqlite3_bind_int(stmt, 2, CACHE_TYPE_UINT8_ARRAY);
           sqlite3_bind_blob(stmt, 3, std::data(kv.second), kv.second.size(), SQLITE_STATIC);
           int result = sqlite3_step(stmt);
           if (result != SQLITE_DONE) {
               sqlite3_finalize(stmt);
               return result;
           }
           sqlite3_reset(stmt);
       }
   }
    sqlite3_exec(db, "COMMIT", nullptr, nullptr, &zErrMsg);
    sqlite3_finalize(stmt);

    return SQLITE_DONE;
}

template<typename T, typename>
int MemCache::put(const std::string &key, const T &value) {
    std::string sql = "INSERT OR REPLACE INTO cache (key, type, value) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;


    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, key.c_str(), -1, SQLITE_STATIC);

    if constexpr (std::is_same_v<T, int>) {
        sqlite3_bind_int(stmt, 2, CACHE_TYPE_INT);
        sqlite3_bind_int(stmt, 3, value);
    } else if constexpr (std::is_same_v<T, double>) {
        sqlite3_bind_int(stmt, 2, CACHE_TYPE_DOUBLE);
        sqlite3_bind_double(stmt, 3, value);
    } else if constexpr (std::is_same_v<T, bool>) {
        sqlite3_bind_int(stmt, 2, CACHE_TYPE_BOOL);
        sqlite3_bind_int(stmt, 3, static_cast<int>(value));
    } else if constexpr (std::is_same_v<T, std::string>) {
        sqlite3_bind_int(stmt, 2, CACHE_TYPE_STRING);
        sqlite3_bind_text(stmt, 3, value.c_str(), -1, SQLITE_STATIC);
    } else if constexpr (std::is_same_v<T, std::vector<std::uint8_t>>) {
        sqlite3_bind_int(stmt, 2, CACHE_TYPE_UINT8_ARRAY);
        sqlite3_bind_blob(stmt, 3, std::data(value), value.size(), SQLITE_STATIC);
    }

    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
//    assert(result == SQLITE_DONE);
    return  result;
}

template<typename T, typename>
optional<T> MemCache::get(const std::string &key) {
    std::string sql = "SELECT type, value FROM cache WHERE key = ? AND type = ?;";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, key.c_str(), -1, SQLITE_STATIC);
    if constexpr (std::is_same_v<T, int>) {
        sqlite3_bind_int(stmt, 2, CACHE_TYPE_INT);
    } else if constexpr (std::is_same_v<T, double>) {
        sqlite3_bind_int(stmt, 2, CACHE_TYPE_DOUBLE);
    } else if constexpr (std::is_same_v<T, bool>) {
        sqlite3_bind_int(stmt, 2, CACHE_TYPE_BOOL);
    } else if constexpr (std::is_same_v<T, std::string>) {
        sqlite3_bind_int(stmt, 2, CACHE_TYPE_STRING);
    } else if constexpr (std::is_same_v<T, std::vector<std::uint8_t>>) {
        sqlite3_bind_int(stmt, 2, CACHE_TYPE_UINT8_ARRAY);
    }

    auto result = sqlite3_step(stmt);
    if (result == SQLITE_ROW) {
        int type = sqlite3_column_int(stmt, 0);

        if constexpr (std::is_same_v<T, int>) {
            if (type == CACHE_TYPE_INT) {
                int value = sqlite3_column_int(stmt, 1);
                sqlite3_finalize(stmt);
                return value;
            }
        } else if constexpr (std::is_same_v<T, double>) {
            if (type == CACHE_TYPE_DOUBLE) {
                double value = sqlite3_column_double(stmt, 1);
                sqlite3_finalize(stmt);
                return value;
            }
        } else if constexpr (std::is_same_v<T, bool>) {
            if (type == CACHE_TYPE_BOOL) {
                bool value = static_cast<bool>(sqlite3_column_int(stmt, 1));
                sqlite3_finalize(stmt);
                return value;
            }
        } else if constexpr (std::is_same_v<T, std::string>) {
            if (type == CACHE_TYPE_STRING) {
                std::string value = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)));
                sqlite3_finalize(stmt);
                return value;
            }
        } else if constexpr (std::is_same_v<T, std::vector<std::uint8_t>>) {
            if (type == CACHE_TYPE_UINT8_ARRAY) {
                const void *blob = sqlite3_column_blob(stmt, 1);
                int size = sqlite3_column_bytes(stmt, 1);
                auto value = std::vector<std::uint8_t>(reinterpret_cast<const std::uint8_t *>(blob),
                                                       reinterpret_cast<const std::uint8_t *>(blob) + size);
                sqlite3_finalize(stmt);
                return value;
            }
        } else {
            return nullopt;
        }
    } else {
        sqlite3_finalize(stmt);
        return nullopt;
    }
}

// Explicit template instantiation for the supported types
template int MemCache::put<std::string>(const std::string& key, const std::string& value);
template int MemCache::put<int>(const std::string& key, const int& value);
template int MemCache::put<double>(const std::string& key, const double& value);
template int MemCache::put<bool>(const std::string& key, const bool& value);
template int MemCache::put<std::vector<std::uint8_t>>(const std::string& key, const std::vector<std::uint8_t>& value);

template int MemCache::put<std::string>(const std::vector<std::pair<std::string, std::string>>& kvs);
template int MemCache::put<int>(const std::vector<std::pair<std::string, int>>& kvs);
template int MemCache::put<double>(const std::vector<std::pair<std::string,double>>& kvs);
template int MemCache::put<bool>(const std::vector<std::pair<std::string, bool>>& kvs);
template int MemCache::put<std::vector<std::uint8_t>>(const std::vector<std::pair<std::string, std::vector<std::uint8_t>>>& kvs);

template optional<std::string> MemCache::get<std::string>(const std::string& key);
template optional<int> MemCache::get<int>(const std::string& key);
template optional<double> MemCache::get<double>(const std::string& key);
template optional<bool> MemCache::get<bool>(const std::string& key);
template optional<std::vector<std::uint8_t>> MemCache::get<std::vector<std::uint8_t>>(const std::string& key);
