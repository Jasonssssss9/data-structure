// test_queue.cpp
// Catch2 unit tests for template class Queue<T>.

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "queue.hpp"
#include <string>

using ds::Queue;

// -----------------------------------------------------------------------------
// Default construction and basic state
// -----------------------------------------------------------------------------
TEST_CASE("Default-constructed queue is empty", "[constructor]") {
    Queue<int> q;
    REQUIRE(q.empty());
    REQUIRE(q.size() == 0);
}

// -----------------------------------------------------------------------------
// push / pop / front / back
// -----------------------------------------------------------------------------
TEST_CASE("push adds elements and back returns the last", "[push][back]") {
    Queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);  // Queue: 1, 2, 3 (back = 3)

    REQUIRE(q.size() == 3);
    REQUIRE_FALSE(q.empty());
    REQUIRE(q.back() == 3);
}

TEST_CASE("pop removes front element", "[pop]") {
    Queue<int> q;
    q.push(42);
    q.push(99);  // Queue: 42, 99

    REQUIRE(q.front() == 42);
    q.pop();
    REQUIRE(q.size() == 1);
    REQUIRE(q.front() == 99);

    q.pop();
    REQUIRE(q.empty());
}

// -----------------------------------------------------------------------------
// const correctness
// -----------------------------------------------------------------------------
TEST_CASE("front and back can be accessed on const queue", "[front][back][const]") {
    Queue<int> q;
    q.push(10);
    q.push(20);

    const Queue<int>& constRef = q;
    REQUIRE(constRef.front() == 10);
    REQUIRE(constRef.back() == 20);
    REQUIRE(constRef.size() == 2);
}

// -----------------------------------------------------------------------------
// Copy constructor and assignment
// -----------------------------------------------------------------------------
TEST_CASE("Copy constructor creates an identical copy", "[copy][constructor]") {
    Queue<int> q;
    q.push(5);
    q.push(10);

    Queue<int> copy = q;
    REQUIRE(copy.size() == 2);
    REQUIRE(copy.front() == 5);
    REQUIRE(copy.back() == 10);

    copy.pop();
    REQUIRE(copy.front() == 10);   // Original unchanged
    REQUIRE(q.front() == 5);
}

TEST_CASE("Copy assignment replaces target contents", "[copy][assignment]") {
    Queue<int> q1;
    q1.push(1);
    q1.push(2);

    Queue<int> q2;
    q2.push(99);

    q2 = q1;  // Copy assignment
    REQUIRE(q2.size() == 2);
    REQUIRE(q2.front() == 1);
    REQUIRE(q2.back() == 2);
    REQUIRE(q1.front() == 1);
}

// -----------------------------------------------------------------------------
// Move constructor and assignment
// -----------------------------------------------------------------------------
TEST_CASE("Move constructor transfers ownership", "[move][constructor]") {
    Queue<std::string> q;
    q.push("first");
    q.push("second");

    Queue<std::string> moved = std::move(q);

    REQUIRE(moved.size() == 2);
    REQUIRE(moved.front() == "first");
    REQUIRE(moved.back() == "second");
    REQUIRE(q.size() == 0);
    REQUIRE(q.empty());
}

TEST_CASE("Move assignment transfers contents", "[move][assignment]") {
    Queue<std::string> q1;
    q1.push("a");
    q1.push("b");

    Queue<std::string> q2;
    q2.push("x");

    q2 = std::move(q1);
    REQUIRE(q2.size() == 2);
    REQUIRE(q2.front() == "a");
    REQUIRE(q2.back() == "b");
    REQUIRE(q1.empty());
}

// -----------------------------------------------------------------------------
// Move support: push with rvalue references
// -----------------------------------------------------------------------------
TEST_CASE("Queue supports move semantics in push", "[push][move]") {
    Queue<std::string> q;
    std::string msg = "hello";
    q.push(std::move(msg));

    REQUIRE(q.size() == 1);
    REQUIRE(q.back() == "hello");
    REQUIRE(msg.empty());

    q.push(std::string("world"));
    REQUIRE(q.back() == "world");
    REQUIRE(q.size() == 2);
}