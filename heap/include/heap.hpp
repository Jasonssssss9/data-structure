#pragma once

#include <cstddef>
#include <cassert>
#include <utility>
#include <initializer_list>

namespace ds {

template<typename T>
struct Less {
    constexpr bool operator()(const T& a, const T& b) const noexcept  {
        return a < b;
    }
};

template<typename T>
struct Greater {
    constexpr bool operator()(const T& a, const T& b) const noexcept {
        return a > b;
    }
};

template<typename T, typename Compare = Less<T>>
class Heap {
public: 
    using size_type = std::size_t;

private:
    size_type size_;
    size_type capacity_;
    T* data_;
    Compare comp_;

    static constexpr size_type kInitialCapacity = 4;
};



}