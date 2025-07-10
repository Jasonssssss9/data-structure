#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "doubly_list.hpp"
#include <string>

using ds::DoublyList;

// -----------------------------------------------------------------------------
// Basic construction and state inspection
// -----------------------------------------------------------------------------
TEST_CASE("DoublyList default-constructed container is empty", "[constructor]") {
    DoublyList<int> list;
    REQUIRE(list.empty());
    REQUIRE(list.size() == 0);
}

// -----------------------------------------------------------------------------
// pushBack / pushFront / popBack / popFront
// -----------------------------------------------------------------------------
TEST_CASE("pushBack and pushFront modify size and order correctly",
          "[push][pop]") {
    DoublyList<int> list;

    SECTION("pushBack appends to the tail") {
        list.pushBack(1);
        list.pushBack(2);
        REQUIRE(list.size() == 2);
        list.popFront(); // now should be 2 left
        REQUIRE(list.contains(2));
        REQUIRE_FALSE(list.contains(1));
    }

    SECTION("pushFront inserts at the head") {
        list.pushFront(2);
        list.pushFront(1); // now list = {1, 2}
        REQUIRE(list.size() == 2);
        list.popBack();
        REQUIRE(list.contains(1));
        REQUIRE_FALSE(list.contains(2));
    }

    SECTION("popBack removes last element") {
        list.pushBack(1);
        list.pushBack(2);
        list.popBack(); // now list = {1}
        REQUIRE(list.size() == 1);
        REQUIRE(list.contains(1));
        REQUIRE_FALSE(list.contains(2));
    }

    SECTION("popFront removes first element") {
        list.pushBack(1);
        list.pushBack(2);
        list.popFront(); // now list = {2}
        REQUIRE(list.size() == 1);
        REQUIRE(list.contains(2));
        REQUIRE_FALSE(list.contains(1));
    }
}

// -----------------------------------------------------------------------------
// insertAfter / insertBefore / erase
// -----------------------------------------------------------------------------
TEST_CASE("insertAfter, insertBefore and erase operate correctly", "[modify]") {
    DoublyList<int> list;
    list.pushBack(1);
    list.pushBack(3);

    REQUIRE(list.insertAfter(1, 2)); // list = {1, 2, 3}
    REQUIRE(list.contains(2));
    REQUIRE(list.size() == 3);

    REQUIRE(list.insertBefore(3, 10)); // list = {1, 2, 10, 3}
    REQUIRE(list.contains(10));
    REQUIRE(list.size() == 4);

    REQUIRE(list.erase(2)); // remove 2
    REQUIRE_FALSE(list.contains(2));
    REQUIRE(list.size() == 3);

    REQUIRE_FALSE(list.erase(42)); // no such value
}

// -----------------------------------------------------------------------------
// contains / move semantics
// -----------------------------------------------------------------------------
TEST_CASE("contains and move semantics", "[contains][move]") {
    DoublyList<std::string> list;
    std::string s = "apple";

    list.pushBack(std::move(s));
    REQUIRE(list.size() == 1);
    REQUIRE(list.contains("apple"));
    REQUIRE(s.empty());

    list.pushFront("banana");
    REQUIRE(list.size() == 2);
    REQUIRE(list.contains("banana"));
}

// -----------------------------------------------------------------------------
// Move constructor and move assignment
// -----------------------------------------------------------------------------
TEST_CASE("DoublyList supports move constructor", "[move][constructor]") {
    DoublyList<std::string> list;
    list.pushBack("a");
    list.pushBack("b");

    DoublyList<std::string> moved = std::move(list); // move constructor

    REQUIRE(moved.size() == 2);
    REQUIRE(moved.contains("a"));
    REQUIRE(moved.contains("b"));

    REQUIRE(list.size() == 0);      // moved-from list is empty
    REQUIRE(list.empty());
}

TEST_CASE("DoublyList supports move assignment", "[move][assignment]") {
    DoublyList<std::string> list1;
    list1.pushBack("x");
    list1.pushBack("y");

    DoublyList<std::string> list2;
    list2.pushBack("old");

    list2 = std::move(list1);       // move assignment

    REQUIRE(list2.size() == 2);
    REQUIRE(list2.contains("x"));
    REQUIRE(list2.contains("y"));

    REQUIRE(list1.empty());
    REQUIRE(list1.size() == 0);
}