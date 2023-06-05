//
// Created by 叶永平 on 2023/4/21.
//

#include <catch2/catch.hpp>
#include "../src/MemCache.hpp"
#include <thread>
#include <iostream>

using namespace memcache;

TEST_CASE("Test MemCache int storage and retrieval", "[mem_cache]") {
auto cache = MemCache::getInstance();

std::string key = "int_key";
int value = 42;

cache->put<int>(key, value);
auto retrieved_value = cache->get<int>(key);

REQUIRE(retrieved_value == value);
}

TEST_CASE("Test MemCache double storage and retrieval", "[mem_cache]") {
    auto cache = MemCache::getInstance();

std::string key = "double_key";
double value = 42.42;

cache->put<double>(key, value);
auto retrieved_value = cache->get<double>(key);

REQUIRE(retrieved_value == Approx(value));
}

TEST_CASE("Test MemCache bool storage and retrieval", "[mem_cache]") {
    auto cache = MemCache::getInstance();

std::string key = "bool_key";
bool value = true;

cache->put<bool>(key, value);
auto retrieved_value = cache->get<bool>(key);

REQUIRE(retrieved_value == value);
}

TEST_CASE("Test MemCache string storage and retrieval", "[mem_cache]") {
    auto cache = MemCache::getInstance();

std::string key = "string_key";
std::string value = "test_string";

cache->put<std::string>(key, value);
auto retrieved_value = cache->get<std::string>(key);

REQUIRE(retrieved_value == value);
}

TEST_CASE("Test MemCache uint8_t array storage and retrieval", "[mem_cache]") {
    auto cache = MemCache::getInstance();

std::string key = "uint8_array_key";
std::vector<std::uint8_t> value = {1, 2, 3, 4, 5};

cache->put<std::vector<std::uint8_t>>(key, value);
auto retrieved_value = cache->get<std::vector<std::uint8_t>>(key);
//    REQUIRE(retrieved_value->data())
REQUIRE(retrieved_value == value);
}

TEST_CASE("Test MemCache int pairs storage and retrieval", "[mem_cache]") {
    auto cache = MemCache::getInstance();
    std::vector<std::pair<std::string, int>> kvs{
        std::pair{"key1", 1},
        std::pair{"key2", 2},
        std::pair{"key3", 3},
        std::pair{"key4", 4},
        std::pair{"key5", 5}
    };
    cache->put(kvs);
    auto value1 = cache->get<int>("key1");
    auto value2 = cache->get<int>("key2");
    auto value3 = cache->get<int>("key3");
    auto value4 = cache->get<int>("key4");
    auto value5 = cache->get<int>("key5");
    REQUIRE(value1 == 1);
    REQUIRE(value2 == 2);
    REQUIRE(value3 == 3);
    REQUIRE(value4 == 4);
    REQUIRE(value5 == 5);
}

TEST_CASE("Test MemCache double pairs storage and retrieval", "[mem_cache]") {
    auto cache = MemCache::getInstance();
    std::vector<std::pair<std::string, double>> kvs{
            std::pair{"key1", 1},
            std::pair{"key2", 2},
            std::pair{"key3", 3},
            std::pair{"key4", 4},
            std::pair{"key5", 5}
    };
    cache->put(kvs);
    auto value1 = cache->get<double>("key1");
    auto value2 = cache->get<double>("key2");
    auto value3 = cache->get<double>("key3");
    auto value4 = cache->get<double>("key4");
    auto value5 = cache->get<double>("key5");
    REQUIRE(value1 == 1);
    REQUIRE(value2 == 2);
    REQUIRE(value3 == 3);
    REQUIRE(value4 == 4);
    REQUIRE(value5 == 5);
}

TEST_CASE("Test MemCache bool pairs storage and retrieval", "[mem_cache]") {
    auto cache = MemCache::getInstance();
    std::vector<std::pair<std::string, bool>> kvs{
            std::pair{"key1", true},
            std::pair{"key2", false},
            std::pair{"key3", true},
            std::pair{"key4", false},
            std::pair{"key5", true}
    };
    cache->put(kvs);
    auto value1 = cache->get<bool>("key1");
    auto value2 = cache->get<bool>("key2");
    auto value3 = cache->get<bool>("key3");
    auto value4 = cache->get<bool>("key4");
    auto value5 = cache->get<bool>("key5");
    REQUIRE(value1 == true);
    REQUIRE(value2 == false);
    REQUIRE(value3 == true);
    REQUIRE(value4 == false);
    REQUIRE(value5 == true);
}

TEST_CASE("Test MemCache string pairs storage and retrieval", "[mem_cache]") {
    auto cache = MemCache::getInstance();
    std::string value1{"value1"};
    std::string value2{"value2"};
    std::string value3{"value3"};
    std::string value4{"value4"};
    std::string value5{"value5"};
    std::vector<std::pair<std::string, std::string>> kvs{
            std::pair{"key1", value1},
            std::pair{"key2", value1},
            std::pair{"key3", value1},
            std::pair{"key4", value1},
            std::pair{"key5", value1}
    };
    cache->put(kvs);
    auto rvalue1 = cache->get<std::string>("key1");
    auto rvalue2 = cache->get<std::string>("key2");
    auto rvalue3 = cache->get<std::string>("key3");
    auto rvalue4 = cache->get<std::string>("key4");
    auto rvalue5 = cache->get<std::string>("key5");
    REQUIRE(value1 == value1);
    REQUIRE(value2 == value2);
    REQUIRE(value3 == value3);
    REQUIRE(value4 == value4);
    REQUIRE(value5 == value5);
}

TEST_CASE("Test MemCache JSON storage, retrieval, and modification", "[mem_cache]") {

    auto cache = MemCache::getInstance();

std::string key = "json_key";
std::string json = R"({"key1": "value1", "key2": 42})";

int result = cache->put_json(key, json);
REQUIRE(result == SQLITE_DONE);
auto retrieved_json = cache->get_json(key);
REQUIRE(retrieved_json == json);

auto value = cache->query_json(key, "$.key1");

REQUIRE(value == "value1");

std::string json_path = "$.key1";
std::string new_value = R"("new_value1")";
cache->modify_json(key, json_path, new_value);
auto modified_value = cache->query_json(key, "$.key1");
REQUIRE(modified_value == "new_value1");

std::string patch = R"({"key3": "value3"})";
result = cache->patch_json(key, patch);
REQUIRE(result == SQLITE_DONE);
auto patch_value = cache->query_json(key, "$.key3");
REQUIRE(patch_value == "value3");

cache->delete_json_value(key, "$.key1");
auto delete_value = cache->query_json(key, "$.key1");
REQUIRE(delete_value == nullopt);
}

TEST_CASE("Test MemCache JSON delete", "[mem_cache]") {
    auto cache = MemCache::getInstance();
    std::string key = "json_key";
    std::string json = R"({"key1": "value1", "key2": 42})";
    int result = cache->put_json(key, json);
    REQUIRE(result == SQLITE_DONE);
    auto retrieved_json = cache->get_json(key);
    REQUIRE(retrieved_json == json);
    auto deleteResult = cache->deleteJson(key);
    REQUIRE(deleteResult == SQLITE_DONE);
    auto deleteded = cache->get_json(key);
    REQUIRE(!deleteded.has_value());
}

#if MEM_CACHE_USE_MULTITHREAD
TEST_CASE("Test MemCache multi-thread put, retrieval", "[mem_cache]") {
    std::thread t1([](){
        auto cache = MemCache::getInstance();
        cache->put<std::string>("key1_t1", "value1");
        cache->put<std::string>("key2_t1", "value2");
        cache->put<std::string>("key3_t1", "value3");
        cache->put<std::string>("key4_t1", "value4");
        cache->put<std::string>("key5_t1", "value5");
    });
    std::thread t2([](){
        auto cache = MemCache::getInstance();
        cache->put<std::string>("key1_t1", "value1");
        cache->put<std::string>("key2_t1", "value2");
        cache->put<std::string>("key3_t1", "value3");
        cache->put<std::string>("key4_t1", "value4");
        cache->put<std::string>("key5_t1", "value5");
    });

    t1.join();
    t2.join();

    auto cache = MemCache::getInstance();
    auto valuet1_5 = cache->get<std::string>("key5_t1");
    auto valuet2_5 = cache->get<std::string>("key5_t1");
    REQUIRE(valuet1_5 == "value5");
    REQUIRE(valuet2_5 == "value5");
}
#endif