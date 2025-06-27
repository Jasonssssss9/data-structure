// test_seq_list.cpp
// Catch2 unit tests for template class SeqList<T>.

#define CATCH_CONFIG_MAIN          // Tell Catch2 to provide its own main()
#include <catch2/catch_test_macros.hpp>

#include "seq_list.hpp"            // The container under test
#include <string>

using ds::SeqList;                          // Short-hand for the qualified name

// -----------------------------------------------------------------------------
// Basic construction and state inspection
// -----------------------------------------------------------------------------
TEST_CASE("SeqList default-constructed container is empty", "[constructor]") {
    SeqList<int> list;                     // Default construction
    REQUIRE(list.empty());                 // Should contain no elements
    REQUIRE(list.size() == 0);             // Size must be zero
}

// -----------------------------------------------------------------------------
// pushBack / pushFront / popBack / popFront
// -----------------------------------------------------------------------------
TEST_CASE("pushBack and pushFront modify size and order correctly",
          "[push][pop][front][back]") {
    SeqList<int> list;

    SECTION("pushBack appends to the tail") {
        list.pushBack(1);
        list.pushBack(2);
        REQUIRE(list.size() == 2);
        REQUIRE(list[0] == 1);
        REQUIRE(list[1] == 2);
    }

    SECTION("pushFront inserts at the head") {
        list.pushBack(2);           // List = {2}
        list.pushFront(1);          // List = {1, 2}
        REQUIRE(list.size() == 2);
        REQUIRE(list[0] == 1);
        REQUIRE(list[1] == 2);
    }

    SECTION("popBack removes the last element") {
        list.pushBack(1);
        list.pushBack(2);
        list.popBack();             // List = {1}
        REQUIRE(list.size() == 1);
        REQUIRE(list[0] == 1);
    }

    SECTION("popFront removes the first element") {
        list.pushBack(1);
        list.pushBack(2);
        list.popFront();            // List = {2}
        REQUIRE(list.size() == 1);
        REQUIRE(list[0] == 2);
    }
}

// -----------------------------------------------------------------------------
// insert / erase
// -----------------------------------------------------------------------------
TEST_CASE("insert and erase work at arbitrary positions", "[insert][erase]") {
    SeqList<int> list;
    list.pushBack(1);
    list.pushBack(3);
    list.insert(1, 2);              // List = {1, 2, 3}

    REQUIRE(list.size() == 3);
    REQUIRE(list[1] == 2);

    list.erase(0);                  // Remove first element → {2, 3}
    REQUIRE(list.size() == 2);
    REQUIRE(list[0] == 2);

    list.erase(1);                  // Remove last element  → {2}
    REQUIRE(list.size() == 1);
    REQUIRE(list[0] == 2);
}

// -----------------------------------------------------------------------------
// find / set
// -----------------------------------------------------------------------------
TEST_CASE("find locates values and set updates elements", "[find][set]") {
    SeqList<int> list;
    list.pushBack(10);
    list.pushBack(20);
    list.pushBack(30);

    REQUIRE(list.find(20) == 1);    // Value 20 is at index 1
    REQUIRE(list.find(40) == -1);   // Value 40 not found

    list.set(1, 25);                // {10, 25, 30}
    REQUIRE(list[1] == 25);
}

// -----------------------------------------------------------------------------
// Move support: pushBack / pushFront with rvalue references
// -----------------------------------------------------------------------------
TEST_CASE("SeqList supports move semantics for push operations", "[move]") {
    SeqList<std::string> list;
    std::string hello = "hello";

    list.pushBack(std::move(hello));     // Move into container
    REQUIRE(list.size() == 1);
    REQUIRE(list[0] == "hello");
    REQUIRE(hello.empty());              // Moved-from string is empty

    list.pushFront(std::string("world"));
    REQUIRE(list.size() == 2);
    REQUIRE(list[0] == "world");
    REQUIRE(list[1] == "hello");
}