//
// Created by 叶永平 on 2023/5/20.
//

#ifndef MEMCACHE_STMTWRAPPER_H
#define MEMCACHE_STMTWRAPPER_H

#include <string>
#include "sqlite3.h"

class StmtWrapper {
public:
    StmtWrapper(sqlite3* db, const std::string& sql) {
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    }

    ~StmtWrapper() {
        sqlite3_finalize(stmt);
    }

    [[nodiscard]] sqlite3_stmt* get() const {
        return stmt;
    }

private:
    sqlite3_stmt* stmt{};
};


#endif //MEMCACHE_STMTWRAPPER_H
