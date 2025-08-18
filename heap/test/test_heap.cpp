// test_heap.cpp
// Catch2 unit tests for template class Heap<T, Compare> (array-based).

#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "heap.hpp"
#include <string>
#include <vector>
#include <utility>
#include <cstddef>

using ds::Heap;
using ds::Less;
using ds::Greater;

// -----------------------------------------------------------------------------
// Default construction and basic state
// -----------------------------------------------------------------------------
TEST_CASE("Default-constructed heap is empty (max-heap by default)", "[constructor]") {
    Heap<int> h; // default: Compare = Less<int> ==> max-heap
    REQUIRE(h.empty());
    REQUIRE(h.size() == 0);
}

// -----------------------------------------------------------------------------
// push / pop / top (max-heap semantics with Less<T>)
// -----------------------------------------------------------------------------
TEST_CASE("push/pop/top maintain max-heap order", "[push][pop][top]") {
    Heap<int> h;
    h.push(5);
    h.push(3);
    h.push(7);
    h.push(1);

    REQUIRE_FALSE(h.empty());
    REQUIRE(h.size() == 4);
    REQUIRE(h.top() == 7);

    h.pop(); REQUIRE(h.top() == 5);
    h.pop(); REQUIRE(h.top() == 3);
    h.pop(); REQUIRE(h.top() == 1);
    h.pop(); REQUIRE(h.empty());
}

// -----------------------------------------------------------------------------
// Build-heap from raw array and initializer_list (O(n) heapify)
// -----------------------------------------------------------------------------
TEST_CASE("build heap from array yields max-heap", "[constructor][heapify]") {
    int arr[] = {4, 9, 1, 6, 7, 2};
    Heap<int> h(arr, static_cast<std::size_t>(sizeof(arr)/sizeof(arr[0]))); // array ctor
    REQUIRE(h.size() == 6);
    REQUIRE(h.top() == 9);

    // Popping should yield non-increasing sequence for max-heap
    int last = 1e9;
    while (!h.empty()) {
        int x = h.top();
        h.pop();
        REQUIRE(x <= last);
        last = x;
    }
}

TEST_CASE("build heap from initializer_list yields max-heap", "[constructor][heapify]") {
    Heap<int> h{3, 10, 5, 8, 2};
    REQUIRE(h.size() == 5);
    REQUIRE(h.top() == 10);

    int last = 1e9;
    while (!h.empty()) {
        int x = h.top();
        h.pop();
        REQUIRE(x <= last);
        last = x;
    }
}

// -----------------------------------------------------------------------------
// const correctness
// -----------------------------------------------------------------------------
TEST_CASE("top can be accessed on const heap", "[top][const]") {
    Heap<int> h;
    h.push(10);
    h.push(20);

    const Heap<int>& cref = h;
    REQUIRE(cref.top() == 20);
    REQUIRE(cref.size() == 2);
}

// -----------------------------------------------------------------------------
// Copy constructor and copy assignment
// -----------------------------------------------------------------------------
TEST_CASE("Copy constructor creates an identical max-heap", "[copy][constructor]") {
    Heap<int> h;
    for (int x : {4, 1, 9}) h.push(x);

    Heap<int> copy = h;
    REQUIRE(copy.size() == 3);
    REQUIRE(copy.top() == 9);

    copy.pop();
    REQUIRE(copy.top() == 4);
    REQUIRE(h.top() == 9); // original unchanged
}

TEST_CASE("Copy assignment replaces target contents", "[copy][assignment]") {
    Heap<int> h1;
    for (int x : {2, 7, 5}) h1.push(x);

    Heap<int> h2;
    for (int x : {42}) h2.push(x);

    h2 = h1;  // copy-swap idiom inside
    REQUIRE(h2.size() == 3);
    REQUIRE(h2.top() == 7);
    REQUIRE(h1.top() == 7);
}

// -----------------------------------------------------------------------------
// Move constructor and move assignment
// -----------------------------------------------------------------------------
TEST_CASE("Move constructor transfers ownership", "[move][constructor]") {
    Heap<std::string> h;
    h.push("alpha");
    h.push("beta");

    Heap<std::string> moved = std::move(h);
    REQUIRE(moved.size() == 2);
    REQUIRE(moved.top() == "beta"); // "beta" > "alpha" lexicographically
    REQUIRE(h.size() == 0);
    REQUIRE(h.empty());
}

TEST_CASE("Move assignment transfers contents", "[move][assignment]") {
    Heap<std::string> h1;
    h1.push("a");
    h1.push("c");
    h1.push("b");

    Heap<std::string> h2;
    h2.push("x");

    h2 = std::move(h1);
    REQUIRE(h2.size() == 3);
    REQUIRE(h2.top() == "c");
    REQUIRE(h1.empty());
}

// -----------------------------------------------------------------------------
// Move support: push with rvalue references
// -----------------------------------------------------------------------------
TEST_CASE("Heap supports move semantics in push", "[push][move]") {
    Heap<std::string> h;
    std::string s = "hello";
    h.push(std::move(s));

    REQUIRE(h.size() == 1);
    REQUIRE(h.top() == "hello");
    REQUIRE(s.empty());

    h.push(std::string("world"));
    REQUIRE(h.top() == "world");
    REQUIRE(h.size() == 2);
}

// -----------------------------------------------------------------------------
// Clear / empty / size
// -----------------------------------------------------------------------------
TEST_CASE("clear resets size but keeps capacity for reuse", "[clear]") {
    Heap<int> h{1, 2, 3, 4};
    REQUIRE(h.size() == 4);
    REQUIRE_FALSE(h.empty());

    h.clear();
    REQUIRE(h.size() == 0);
    REQUIRE(h.empty());

    h.push(5);
    REQUIRE(h.top() == 5);
    REQUIRE(h.size() == 1);
}

// -----------------------------------------------------------------------------
// Duplicates and ordering robustness
// -----------------------------------------------------------------------------
TEST_CASE("Heap handles duplicates and maintains correct order", "[order]") {
    Heap<int> h;
    for (int x : {5, 5, 3, 7, 7, 7, 1}) h.push(x);

    // Non-increasing pops with duplicates
    int last = 1e9;
    std::vector<int> popped;
    while (!h.empty()) {
        int x = h.top();
        h.pop();
        REQUIRE(x <= last);
        popped.push_back(x);
        last = x;
    }

    // Expected multiset-like order: 7,7,7,5,5,3,1
    std::vector<int> expect{7,7,7,5,5,3,1};
    REQUIRE(popped == expect);
}

// -----------------------------------------------------------------------------
// Custom comparator: min-heap using Greater<T>
// -----------------------------------------------------------------------------
TEST_CASE("Custom comparator Greater<T> yields min-heap", "[comparator]") {
    Heap<int, Greater<int>> h; // now min-heap
    for (int x : {5, 1, 7, 3}) h.push(x);

    REQUIRE(h.top() == 1);
    h.pop(); REQUIRE(h.top() == 3);
    h.pop(); REQUIRE(h.top() == 5);
    h.pop(); REQUIRE(h.top() == 7);
    h.pop(); REQUIRE(h.empty());
}

// -----------------------------------------------------------------------------
// Custom comparator with state (build heap from array + instance comparator)
// -----------------------------------------------------------------------------
TEST_CASE("Comparator with state works via constructor-injected instance", "[comparator][state]") {
    struct ModKeyLess {
        int mod;
        // "less on key" -> default logic makes it a max-heap by key
        bool operator()(int a, int b) const noexcept {
            return (a % mod) < (b % mod);
        }
    };

    ModKeyLess cmp{5};
    int arr[] = {7, 12, 3, 25, 10}; // keys mod 5: 2, 2, 3, 0, 0
    Heap<int, ModKeyLess> h(arr, 5, cmp);

    // The top should have the maximum key (mod 5), i.e., key==3 -> value 3
    REQUIRE(h.top() % 5 == 3);

    // Popping should produce non-increasing sequence by key
    int lastKey = 100;
    while (!h.empty()) {
        int x = h.top();
        h.pop();
        REQUIRE((x % 5) <= lastKey);
        lastKey = x % 5;
    }
}