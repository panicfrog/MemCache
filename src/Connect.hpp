//
// Created by 叶永平 on 2023/5/21.
//

#ifndef MEMCACHE_CONNECT_H
#define MEMCACHE_CONNECT_H

#include "sqlite3.h"
//#include <iostream>

namespace memcache {
    class Connect {
    public:
        explicit Connect(sqlite3 *db) : db{db} {}

        ~Connect() {
//        std::cout << "close db" << std::endl;
            sqlite3_close(db);
        }

        [[nodiscard]] sqlite3 *get() const {
            return db;
        }

    private:
        sqlite3 *db{};
    };
}
#endif //MEMCACHE_CONNECT_H
