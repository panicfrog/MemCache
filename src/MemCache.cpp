
#include "MemCache.hpp"

#define CACHE_TYPE_INT 1
#define CACHE_TYPE_DOUBLE 2
#define CACHE_TYPE_BOOL 3
#define CACHE_TYPE_STRING 4
#define CACHE_TYPE_UINT8_ARRAY 5

using nonstd::optional;
using nonstd::nullopt;

namespace memcache {

MemCache* MemCache::instance = nullptr;
std::once_flag MemCache::flag_;

thread_local std::unique_ptr<StmtWrapper> MemCache::put_stmt = nullptr;
thread_local std::unique_ptr<StmtWrapper> MemCache::get_stmt = nullptr;
thread_local std::unique_ptr<StmtWrapper> MemCache::get_json_stmt = nullptr;
thread_local std::unique_ptr<StmtWrapper> MemCache::put_json_stmt = nullptr;
thread_local std::unique_ptr<StmtWrapper> MemCache::modify_json_stmt = nullptr;
thread_local std::unique_ptr<StmtWrapper> MemCache::query_json_stmt = nullptr;
thread_local std::unique_ptr<StmtWrapper> MemCache::patch_json_stmt = nullptr;
thread_local std::unique_ptr<StmtWrapper> MemCache::delete_value_stmt = nullptr;
thread_local std::unique_ptr<StmtWrapper> MemCache::delete_json_stmt = nullptr;
thread_local std::unique_ptr<StmtWrapper> MemCache::delete_json_value_stmt = nullptr;

#if MEM_CACHE_USE_MULTITHREAD
thread_local std::unique_ptr<Connect> MemCache::db_connect = nullptr;
#endif

MemCache::MemCache() {
#if MEM_CACHE_USE_MULTITHREAD
    int rc = sqlite3_config(SQLITE_CONFIG_MULTITHREAD);
    auto db = get_db();
    if (rc != SQLITE_OK) {
        std::cout << "configure fial" << rc << std::endl;
        throw std::runtime_error("Cannot configure SQLite for multi-threading, error code: " + std::string(
                sqlite3_errmsg(db)));
    }
#else
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        throw std::runtime_error("Cannot open database: " + std::string(sqlite3_errmsg(db)));
    }
#endif
//    int mode = sqlite3_threadsafe();
//    std::cout << "SQLite thread safety mode: " << mode << std::endl;
    sqlite3_exec(db, "CREATE TABLE cache (key TEXT PRIMARY KEY NOT NULL, type INTEGER NOT NULL, value BLOB NOT NULL);", nullptr, nullptr, nullptr);
    sqlite3_exec(db, "CREATE TABLE json_cache (key TEXT PRIMARY KEY NOT NULL, json JSON NOT NULL CHECK (json_valid(json)));", nullptr, nullptr, nullptr);
}

#if MEM_CACHE_USE_MULTITHREAD
inline  sqlite3* MemCache::get_db() {
    if (!db_connect) {
        sqlite3* db{};
        int rc = sqlite3_open("file:memcache_shared?mode=memory&cache=shared", &db);
        if (rc != SQLITE_OK) {
            throw std::runtime_error("Cannot open database: " + std::string(sqlite3_errmsg(db)) + "code: " + std::to_string(rc));
        }
        db_connect = std::make_unique<Connect>(db);
    }
    return db_connect->get();
}
#else
MemCache::~MemCache() {
    sqlite3_close(db);
}
#endif

inline sqlite3_stmt* MemCache::prepareStatements(StmtType type, sqlite3 *db) {
    switch (type) {
        case StmtType::put:
            if (!put_stmt) {
                put_stmt = std::make_unique<StmtWrapper>(db, "INSERT OR REPLACE INTO cache (key, type, value) VALUES (?, ?, ?);");
            }
            return put_stmt->get();
        case StmtType::get:
            if (!get_stmt) {
                get_stmt = std::make_unique<StmtWrapper>(db, "SELECT type, value FROM cache WHERE key = ? AND type = ?;");
            }
            return get_stmt->get();
        case StmtType::delete_value:
            if (!delete_value_stmt) {
                delete_value_stmt = std::make_unique<StmtWrapper>(db, "DELETE FROM cache WHERE key = ?;");
            }
            return delete_value_stmt->get();
        case StmtType::get_json:
            if (!get_json_stmt) {
                get_json_stmt = std::make_unique<StmtWrapper>(db, "SELECT json FROM json_cache WHERE key = ?;");
            }
            return get_json_stmt->get();
        case StmtType::put_json:
            if (!put_json_stmt) {
                put_json_stmt = std::make_unique<StmtWrapper>(db, "INSERT OR REPLACE INTO json_cache (key, json) VALUES (?, ?);");
            }
            return put_json_stmt->get();
        case StmtType::delete_json:
            if (!delete_json_stmt) {
                delete_json_stmt = std::make_unique<StmtWrapper>(db, "DELETE FROM json_cache WHERE key = ?");
            }
            return delete_json_stmt->get();
        case StmtType::query_json:
            if (!query_json_stmt) {
                query_json_stmt = std::make_unique<StmtWrapper>(db, "SELECT json_extract(json, ? ) FROM json_cache WHERE key = ?;");
            }
            return query_json_stmt->get();
        case StmtType::modify_json:
            if (!modify_json_stmt) {
                modify_json_stmt = std::make_unique<StmtWrapper>(db, "UPDATE json_cache SET json = json_set(json, ?, json(?)) WHERE key = ?;");
            }
            return modify_json_stmt->get();
        case StmtType::patch_json:
            if (!patch_json_stmt) {
                patch_json_stmt = std::make_unique<StmtWrapper>(db, "UPDATE json_cache SET json = (CASE WHEN json_valid(?) THEN json_patch(json, ?) ELSE json END) WHERE key = ?;");
            }
            return patch_json_stmt->get();
        case StmtType::delete_json_value:
            if(!delete_json_value_stmt) {
                delete_json_value_stmt = std::make_unique<StmtWrapper>(db, "UPDATE json_cache SET json = json_remove(json, ?) WHERE key = ?;");
            }
            return delete_json_value_stmt->get();
    }
}

int MemCache::put_json(const std::string& key, const std::string& json) {
#if MEM_CACHE_USE_MULTITHREAD
    auto db = get_db();
#endif
    auto stmt = prepareStatements(StmtType::put_json, db);
    sqlite3_reset(stmt);
    sqlite3_bind_text(stmt, 1, key.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, json.c_str(), -1, SQLITE_STATIC);

    auto result = sqlite3_step(stmt);
    return result;
}

optional<std::string> MemCache::get_json(const std::string& key) {
#if MEM_CACHE_USE_MULTITHREAD
    auto db = get_db();
#endif
    auto stmt = prepareStatements(StmtType::get_json, db);
    sqlite3_reset(stmt);
    sqlite3_bind_text(stmt, 1, key.c_str(), -1, SQLITE_STATIC);

    std::string json;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        json = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        return  json;
    }
    return nullopt;
}

int MemCache::deleteValue(const std::string &key) {
#if MEM_CACHE_USE_MULTITHREAD
    auto db = get_db();
#endif
    auto stmt = prepareStatements(StmtType::delete_value, db);
    sqlite3_reset(stmt);
    sqlite3_bind_text(stmt, 1, key.c_str(), -1, SQLITE_STATIC);
    auto result = sqlite3_step(stmt);
    return result;
}

int MemCache::deleteJson(const std::string &key) {
#if MEM_CACHE_USE_MULTITHREAD
    auto db = get_db();
#endif
    auto stmt = prepareStatements(StmtType::delete_json, db);
    sqlite3_reset(stmt);
    sqlite3_bind_text(stmt, 1, key.c_str(), -1, SQLITE_STATIC);
    auto result = sqlite3_step(stmt);
    return result;
}

optional<std::string>  MemCache::query_json(const std::string& key, const std::string& json_path) {
#if MEM_CACHE_USE_MULTITHREAD
    auto db = get_db();
#endif
    auto stmt = prepareStatements(StmtType::query_json, db);
    sqlite3_reset(stmt);
    sqlite3_bind_text(stmt, 1, json_path.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, key.c_str(), -1, SQLITE_STATIC);
    std::string value;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        if (sqlite3_column_type(stmt, 0) != SQLITE_NULL) {
            value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            return value;
        }
    }
    return nullopt;
}

int MemCache::modify_json(const std::string& key, const std::string& json_path, const std::string& value) {
#if MEM_CACHE_USE_MULTITHREAD
    auto db = get_db();
#endif
    auto stmt = prepareStatements(StmtType::modify_json, db);
    sqlite3_reset(stmt);
    sqlite3_bind_text(stmt, 1, json_path.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, value.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, key.c_str(), -1, SQLITE_STATIC);
    auto result = sqlite3_step(stmt);
    return result;
}

int MemCache::patch_json(const std::string& key, const std::string& patch) {
#if MEM_CACHE_USE_MULTITHREAD
    auto db = get_db();
#endif
    auto stmt = prepareStatements(StmtType::patch_json, db);
    sqlite3_reset(stmt);
    sqlite3_bind_text(stmt, 1, patch.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, patch.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, key.c_str(), -1, SQLITE_STATIC);
    auto result = sqlite3_step(stmt);
    return result;
}

int MemCache::delete_json_value(const std::string &key, const std::string &json_path) {
#if MEM_CACHE_USE_MULTITHREAD
    auto db = get_db();
#endif
    auto stmt = prepareStatements(StmtType::delete_json_value, db);
    sqlite3_reset(stmt);
    sqlite3_bind_text(stmt, 1, json_path.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, key.c_str(), -1, SQLITE_STATIC);
    auto result = sqlite3_step(stmt);
    return result;
}

template<typename T, typename>
int MemCache::put(const std::vector<std::pair<std::string, T>>& kvs) {
    UNUSED char *zErrMsg = nullptr;
#if MEM_CACHE_USE_MULTITHREAD
    auto db = get_db();
#endif
    sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, &zErrMsg);
    if (zErrMsg != nullptr) {
        return 0;
    }
    UNUSED auto stmt = prepareStatements(StmtType::put, db);
    sqlite3_reset(stmt);
   if constexpr (std::is_same_v<T, int>) {
       for(UNUSED const auto& kv : kvs ) {
           sqlite3_bind_text(stmt, 1, kv.first.c_str(), -1, SQLITE_STATIC);
           sqlite3_bind_int(stmt, 2, CACHE_TYPE_INT);
           sqlite3_bind_int(stmt, 3, kv.second);
           UNUSED int result = sqlite3_step(stmt);
           if (result != SQLITE_DONE) {
               return result;
           }
           sqlite3_reset(stmt);
       }
   } else if constexpr (std::is_same_v<T, double>) {
       for(UNUSED const auto& kv : kvs ) {
           sqlite3_bind_text(stmt, 1, kv.first.c_str(), -1, SQLITE_STATIC);
           sqlite3_bind_int(stmt, 2, CACHE_TYPE_DOUBLE);
           sqlite3_bind_double(stmt, 3, kv.second);
           UNUSED int result = sqlite3_step(stmt);
           if (result != SQLITE_DONE) {
               return result;
           }
           sqlite3_reset(stmt);
       }
   } else if constexpr (std::is_same_v<T, bool>) {
       for(UNUSED const auto& kv : kvs ) {
           sqlite3_bind_text(stmt, 1, kv.first.c_str(), -1, SQLITE_STATIC);
           sqlite3_bind_int(stmt, 2, CACHE_TYPE_BOOL);
           sqlite3_bind_int(stmt, 3, static_cast<int>(kv.second));
           UNUSED int result = sqlite3_step(stmt);
           if (result != SQLITE_DONE) {
               return result;
           }
           sqlite3_reset(stmt);
       }
   } else if constexpr (std::is_same_v<T, std::string>) {
       for(UNUSED const auto& kv : kvs ) {
           sqlite3_bind_text(stmt, 1, kv.first.c_str(), -1, SQLITE_STATIC);
           sqlite3_bind_int(stmt, 2, CACHE_TYPE_STRING);
           sqlite3_bind_text(stmt, 3, kv.second.c_str(), -1, SQLITE_STATIC);
           UNUSED int result = sqlite3_step(stmt);
           if (result != SQLITE_DONE) {
               return result;
           }
           sqlite3_reset(stmt);
       }
   } else if constexpr (std::is_same_v<T, std::vector<std::uint8_t>>) {
       for(UNUSED auto kv : kvs ) {
           sqlite3_bind_text(stmt, 1, kv.first.c_str(), -1, SQLITE_STATIC);
           sqlite3_bind_int(stmt, 2, CACHE_TYPE_UINT8_ARRAY);
           sqlite3_bind_blob(stmt, 3, std::data(kv.second), kv.second.size(), SQLITE_STATIC);
           UNUSED int result = sqlite3_step(stmt);
           if (result != SQLITE_DONE) {
               return result;
           }
           sqlite3_reset(stmt);
       }
   }
    sqlite3_exec(db, "COMMIT", nullptr, nullptr, &zErrMsg);

    return SQLITE_DONE;
}

template<typename T, typename>
int MemCache::put(const std::string &key, const T &value) {
#if MEM_CACHE_USE_MULTITHREAD
    auto db = get_db();
#endif
    auto stmt = prepareStatements(StmtType::put, db);
    sqlite3_reset(stmt);
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
//    assert(result == SQLITE_DONE);
    return  result;
}

template<typename T, typename>
optional<T> MemCache::get(const std::string &key) {
#if MEM_CACHE_USE_MULTITHREAD
    auto db = get_db();
#endif
    auto stmt = prepareStatements(StmtType::get, db);
    sqlite3_reset(stmt);
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
                return value;
            }
        } else if constexpr (std::is_same_v<T, double>) {
            if (type == CACHE_TYPE_DOUBLE) {
                double value = sqlite3_column_double(stmt, 1);
                return value;
            }
        } else if constexpr (std::is_same_v<T, bool>) {
            if (type == CACHE_TYPE_BOOL) {
                bool value = static_cast<bool>(sqlite3_column_int(stmt, 1));
                return value;
            }
        } else if constexpr (std::is_same_v<T, std::string>) {
            if (type == CACHE_TYPE_STRING) {
                std::string value = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)));
                return value;
            }
        } else if constexpr (std::is_same_v<T, std::vector<std::uint8_t>>) {
            if (type == CACHE_TYPE_UINT8_ARRAY) {
                const void *blob = sqlite3_column_blob(stmt, 1);
                int size = sqlite3_column_bytes(stmt, 1);
                auto value = std::vector<std::uint8_t>(reinterpret_cast<const std::uint8_t *>(blob),
                                                       reinterpret_cast<const std::uint8_t *>(blob) + size);
                return value;
            }
        } else {
            return nullopt;
        }
    } else {
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

}