// test_avl_tree.cpp
// Catch2 unit tests for template class AVLTree<K, V>

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "avl_tree.hpp"
#include <string>

using ds::AVLTree;

// -----------------------------------------------------------------------------
// Default construction
// -----------------------------------------------------------------------------
TEST_CASE("Default-constructed tree is empty", "[constructor]") {
    AVLTree<int, std::string> tree;
    REQUIRE(tree.empty());
    REQUIRE(tree.size() == 0);
}

// -----------------------------------------------------------------------------
// Insert elements
// -----------------------------------------------------------------------------
TEST_CASE("insert adds key-value pairs correctly", "[insert]") {
    AVLTree<int, std::string> tree;
    tree.insert(10, "ten");
    tree.insert(20, "twenty");
    tree.insert(5,  "five");

    REQUIRE(tree.size() == 3);
    REQUIRE(tree.contains(10));
    REQUIRE(tree.contains(5));
    REQUIRE(tree.contains(20));
    REQUIRE(*tree.find(5) == "five");
}

TEST_CASE("insert overwrites value for duplicate key", "[insert][overwrite]") {
    AVLTree<int, std::string> tree;
    tree.insert(1, "a");
    tree.insert(1, "b");

    REQUIRE(tree.size() == 1);
    REQUIRE(*tree.find(1) == "b");
}

// -----------------------------------------------------------------------------
// Contains / Find
// -----------------------------------------------------------------------------
TEST_CASE("contains and find behave as expected", "[contains][find]") {
    AVLTree<int, std::string> tree;
    tree.insert(3, "three");
    tree.insert(1, "one");
    tree.insert(4, "four");

    REQUIRE(tree.contains(1));
    REQUIRE(tree.find(1) != nullptr);
    REQUIRE(*tree.find(4) == "four");
    REQUIRE(tree.find(2) == nullptr);
}

// -----------------------------------------------------------------------------
// Erase cases
// -----------------------------------------------------------------------------
TEST_CASE("erase handles leaf, one-child, and two-child nodes", "[erase]") {
    AVLTree<int, std::string> tree;
    tree.insert(10, "a");
    tree.insert(5,  "b");
    tree.insert(15, "c");
    tree.insert(12, "d");

    // Leaf
    REQUIRE(tree.erase(12));
    REQUIRE_FALSE(tree.contains(12));

    // One child
    REQUIRE(tree.erase(15));
    REQUIRE_FALSE(tree.contains(15));

    // Two children
    REQUIRE(tree.erase(10));
    REQUIRE_FALSE(tree.contains(10));

    // Remaining key
    REQUIRE(tree.contains(5));
    REQUIRE(tree.size() == 1);
}

// -----------------------------------------------------------------------------
// Copy and move
// -----------------------------------------------------------------------------
TEST_CASE("Copy constructor creates deep copy", "[copy][constructor]") {
    AVLTree<int, std::string> tree;
    tree.insert(1, "x");
    tree.insert(2, "y");

    AVLTree<int, std::string> copy = tree;
    REQUIRE(copy.size() == 2);
    REQUIRE(*copy.find(1) == "x");
    REQUIRE(*copy.find(2) == "y");

    copy.erase(1);
    REQUIRE(copy.size() == 1);
    REQUIRE(tree.size() == 2); // original unaffected
}

TEST_CASE("Copy assignment works correctly", "[copy][assignment]") {
    AVLTree<int, std::string> a;
    a.insert(1, "a");
    a.insert(2, "b");

    AVLTree<int, std::string> b;
    b.insert(99, "z");
    b = a;

    REQUIRE(b.size() == 2);
    REQUIRE(b.contains(2));
    REQUIRE(*b.find(1) == "a");
}

TEST_CASE("Move constructor transfers ownership", "[move][constructor]") {
    AVLTree<std::string, int> t;
    t.insert("x", 1);
    t.insert("y", 2);

    AVLTree<std::string, int> moved = std::move(t);
    REQUIRE(moved.size() == 2);
    REQUIRE(moved.contains("x"));
    REQUIRE(t.empty());
}

TEST_CASE("Move assignment transfers contents", "[move][assignment]") {
    AVLTree<std::string, int> a;
    a.insert("a", 10);
    a.insert("b", 20);

    AVLTree<std::string, int> b;
    b.insert("z", 999);
    b = std::move(a);

    REQUIRE(b.size() == 2);
    REQUIRE(b.contains("b"));
    REQUIRE(a.empty());
}