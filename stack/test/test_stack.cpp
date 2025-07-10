// test_stack.cpp
// Catch2 unit tests for template class Stack<T>.

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "stack.hpp"
#include <string>

using ds::Stack;

// -----------------------------------------------------------------------------
// Default construction and basic state
// -----------------------------------------------------------------------------
TEST_CASE("Default-constructed stack is empty", "[constructor]") {
    Stack<int> s;
    REQUIRE(s.empty());
    REQUIRE(s.size() == 0);
}

// -----------------------------------------------------------------------------
// push / pop / top
// -----------------------------------------------------------------------------
TEST_CASE("push adds elements and top returns the last", "[push][top]") {
    Stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);  // Stack: 1, 2, 3 (top = 3)

    REQUIRE(s.size() == 3);
    REQUIRE_FALSE(s.empty());
    REQUIRE(s.top() == 3);
}

TEST_CASE("pop removes top element", "[pop]") {
    Stack<int> s;
    s.push(42);
    s.push(99);  // Stack: 42, 99

    REQUIRE(s.top() == 99);
    s.pop();
    REQUIRE(s.size() == 1);
    REQUIRE(s.top() == 42);

    s.pop();
    REQUIRE(s.empty());
}

// -----------------------------------------------------------------------------
// const correctness
// -----------------------------------------------------------------------------
TEST_CASE("top can be accessed on const stack", "[top][const]") {
    Stack<int> s;
    s.push(10);
    s.push(20);

    const Stack<int>& constRef = s;
    REQUIRE(constRef.top() == 20);
    REQUIRE(constRef.size() == 2);
}

// -----------------------------------------------------------------------------
// Copy constructor and assignment
// -----------------------------------------------------------------------------
TEST_CASE("Copy constructor creates an identical copy", "[copy][constructor]") {
    Stack<int> s;
    s.push(5);
    s.push(10);

    Stack<int> copy = s;
    REQUIRE(copy.size() == 2);
    REQUIRE(copy.top() == 10);

    copy.pop();
    REQUIRE(copy.top() == 5);   // Original unchanged
    REQUIRE(s.top() == 10);
}

TEST_CASE("Copy assignment replaces target contents", "[copy][assignment]") {
    Stack<int> s1;
    s1.push(1);
    s1.push(2);

    Stack<int> s2;
    s2.push(99);

    s2 = s1;  // Copy assignment
    REQUIRE(s2.size() == 2);
    REQUIRE(s2.top() == 2);
    REQUIRE(s1.top() == 2);
}

// -----------------------------------------------------------------------------
// Move constructor and assignment
// -----------------------------------------------------------------------------
TEST_CASE("Move constructor transfers ownership", "[move][constructor]") {
    Stack<std::string> s;
    s.push("first");
    s.push("second");

    Stack<std::string> moved = std::move(s);

    REQUIRE(moved.size() == 2);
    REQUIRE(moved.top() == "second");
    REQUIRE(s.size() == 0);  // s is in valid but empty state
    REQUIRE(s.empty());
}

TEST_CASE("Move assignment transfers contents", "[move][assignment]") {
    Stack<std::string> s1;
    s1.push("a");
    s1.push("b");

    Stack<std::string> s2;
    s2.push("x");

    s2 = std::move(s1);
    REQUIRE(s2.size() == 2);
    REQUIRE(s2.top() == "b");
    REQUIRE(s1.empty());
}

// -----------------------------------------------------------------------------
// Move support: push with rvalue references
// -----------------------------------------------------------------------------
TEST_CASE("Stack supports move semantics in push", "[push][move]") {
    Stack<std::string> s;
    std::string msg = "hello";
    s.push(std::move(msg));

    REQUIRE(s.size() == 1);
    REQUIRE(s.top() == "hello");
    REQUIRE(msg.empty());

    s.push(std::string("world"));
    REQUIRE(s.top() == "world");
    REQUIRE(s.size() == 2);
}