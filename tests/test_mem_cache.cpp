//
// Created by 叶永平 on 2023/4/21.
//

#include <catch2/catch.hpp>
#include "../src/MemCache.hpp"

TEST_CASE("Test MemCache int storage and retrieval", "[mem_cache]") {
MemCache cache;

std::string key = "int_key";
int value = 42;

cache.put<int>(key, value);
auto retrieved_value = cache.get<int>(key);

REQUIRE(retrieved_value == value);
}

TEST_CASE("Test MemCache double storage and retrieval", "[mem_cache]") {
MemCache cache;

std::string key = "double_key";
double value = 42.42;

cache.put<double>(key, value);
auto retrieved_value = cache.get<double>(key);

REQUIRE(retrieved_value == Approx(value));
}

TEST_CASE("Test MemCache bool storage and retrieval", "[mem_cache]") {
MemCache cache;

std::string key = "bool_key";
bool value = true;

cache.put<bool>(key, value);
auto retrieved_value = cache.get<bool>(key);

REQUIRE(retrieved_value == value);
}

TEST_CASE("Test MemCache string storage and retrieval", "[mem_cache]") {
MemCache cache;

std::string key = "string_key";
std::string value = "test_string";

cache.put<std::string>(key, value);
auto retrieved_value = cache.get<std::string>(key);

REQUIRE(retrieved_value == value);
}

TEST_CASE("Test MemCache uint8_t array storage and retrieval", "[mem_cache]") {
MemCache cache;

std::string key = "uint8_array_key";
std::vector<std::uint8_t> value = {1, 2, 3, 4, 5};

cache.put<std::vector<std::uint8_t>>(key, value);
auto retrieved_value = cache.get<std::vector<std::uint8_t>>(key);
//    REQUIRE(retrieved_value->data())
REQUIRE(retrieved_value == value);
}

TEST_CASE("Test MemCache JSON storage, retrieval, and modification", "[mem_cache]") {
MemCache cache;

std::string key = "json_key";
std::string json = R"({"key1": "value1", "key2": 42})";

cache.put_json(key, json);
auto retrieved_json = cache.get_json(key);

REQUIRE(retrieved_json == json);

auto value = cache.query_json(key, "$.key1");

REQUIRE(value == "value1");

std::string json_path = "$.key1";
std::string new_value = "new_value1";

cache.modify_json(key, json_path, new_value);
auto modified_value = cache.query_json(key, "$.key1");

std::string expected_modified_json = R"({"key1": "new_value1", "key2": 42})";

REQUIRE(modified_value == new_value);
}
