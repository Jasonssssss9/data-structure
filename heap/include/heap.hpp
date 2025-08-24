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

    explicit Heap(Compare comp = Compare());
    Heap(const T* ptr, size_type n, Compare comp = Compare());
    Heap(std::initializer_list<T> init, Compare comp = Compare());
    ~Heap();

    Heap(const Heap& other);
    Heap(Heap&& other) noexcept;
    Heap& operator=(Heap other) noexcept;

    const T& top() const;

    template <class U>
    void push(U&& value);  // append then sift_up

    void pop();            // swap top with last, --size_, then sift_down
    
    void clear() noexcept;
    bool empty() const noexcept;
    size_type size() const noexcept;

private:
    void ensureCapacity();

    void swap(Heap& other) noexcept;

    void siftDown(size_type i); // returns true if moved downward
    void siftUp(size_type i);
    void buildHeap();           // bottom-up heapify, O(n)

private:
    size_type size_ = 0;
    size_type capacity_ = 0;
    T* data_ = nullptr;
    Compare comp_{};

    static constexpr size_type kInitialCapacity = 4;
};

template<typename T, typename Compare>
Heap<T, Compare>::Heap(Compare comp) 
    : size_(0), 
      capacity_(kInitialCapacity), 
      data_(new T[kInitialCapacity]), 
      comp_(std::move(comp)) 
{}

template<typename T, typename Compare>
Heap<T, Compare>::Heap(const T* ptr, size_type n, Compare comp)
    : size_(n), 
      capacity_(n > kInitialCapacity ? n : kInitialCapacity), 
      data_(new T[capacity_]),
      comp_(std::move(comp)) {
    for (size_type i = 0; i < n; ++i) {
        data_[i] = ptr[i];
    }
    buildHeap(); // O(n) bottom-up heapify
}

template <typename T, typename Compare>
Heap<T, Compare>::Heap(std::initializer_list<T> init, Compare comp)
    : size_(static_cast<size_type>(init.size())),
      capacity_(size_ > kInitialCapacity ? size_ : kInitialCapacity),
      data_(new T[capacity_]),
      comp_(std::move(comp)) {
    size_type i = 0;
    for (const auto& x : init) data_[i++] = x;
    buildHeap(); // O(n)
}

template <typename T, typename Compare>
Heap<T, Compare>::~Heap() {
    delete[] data_;
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
}

template <typename T, typename Compare>
Heap<T, Compare>::Heap(const Heap& other)
    : size_(other.size_),
      capacity_(other.capacity_),
      data_(new T[capacity_]),
      comp_(other.comp_){
    for (size_type i = 0; i < size_; ++i) {
        data_[i] = other.data_[i];
    }
}

template <typename T, typename Compare>
Heap<T, Compare>::Heap(Heap&& other) noexcept
    : size_(other.size_), 
      capacity_(other.capacity_),
      data_(other.data_),
      comp_(std::move(other.comp_)) {
    other.size_ = 0;
    other.capacity_ = 0;
    other.data_ = nullptr;
}

template <typename T, typename Compare>
Heap<T, Compare>& Heap<T, Compare>::operator=(Heap other) noexcept {
    swap(other);
    return *this;
}

template<typename T, typename Compare>
const T& Heap<T, Compare>::top() const {
    assert(size_ > 0 && "Heap is empty");
    return data_[0];
}

template<typename T, typename Compare>
template<class U>
void Heap<T, Compare>::push(U&& value) {
    ensureCapacity();
    data_[size_] = std::forward<U>(value);
    siftUp(size_);
    ++size_;
}

template<typename T, typename Compare>
void Heap<T, Compare>::pop() {
    assert(size_ > 0 && "Heap is empty");
    std::swap(data_[0], data_[size_ - 1]);
    --size_;
    if (size_ > 0) {
        siftDown(0);
    }
}

template<typename T, typename Compare>
void Heap<T, Compare>::clear() noexcept {
    size_ = 0;
}

template<typename T, typename Compare>
bool Heap<T, Compare>::empty() const noexcept {
    return size_ == 0;
}

template<typename T, typename Compare>
typename Heap<T, Compare>::size_type
Heap<T, Compare>::size() const noexcept {
    return size_;
}


template <typename T, typename Compare>
void Heap<T, Compare>::ensureCapacity() {
    if (size_ >= capacity_) {
        size_type newCapacity = capacity_ == 0 ? kInitialCapacity : capacity_ * 4;
        T* newData = new T[newCapacity];
        for (size_type i = 0; i < size_; ++i) {
            newData[i] = data_[i];
        }
        delete[] data_;
        data_ = newData;
        capacity_ = newCapacity;
    }
}

template <typename T, typename Compare>
void Heap<T, Compare>::swap(Heap& other) noexcept{
    using std::swap;
    swap(size_, other.size_);
    swap(capacity_, other.capacity_);
    swap(data_, other.data_);
    swap(comp_, other.comp_);
}

template <typename T, typename Compare>
void Heap<T, Compare>::siftDown(size_type idx) {
    while (true) {
        size_type left  = 2 * idx + 1;
        size_type right = 2 * idx + 2;
        size_type best  = idx;

        // Compare for max-heap by default (Less<T> means greater value has priority)
        if (left < size_ && comp_(data_[best], data_[left])) {
            best = left;
        }
        if (right < size_ && comp_(data_[best], data_[right])) {
            best = right;
        }

        if (best != idx) {
            std::swap(data_[idx], data_[best]);
            idx = best;
        } else {
            break;
        }
    }
}

template<typename T, typename Compare>
void Heap<T, Compare>::siftUp(size_type idx) {
    while (idx > 0) {
        size_type parent = (idx - 1) / 2;
        if (comp_(data_[parent], data_[idx])) {
            std::swap(data_[idx], data_[parent]);
            idx = parent;
        } else {
            break;
        }
    }
}

template<typename T, typename Compare>
void Heap<T, Compare>::buildHeap() {
    if (size_ <= 1) {
        return; 
    }

    // Traverse all non-leaf nodes in reverse order to build the heap.
    // Use `i-- > 0` instead of `i >= 0` because `size_type` is unsigned;
    // this avoids underflow when decrementing from 0.
    for (size_type i = size_ / 2; i-- > 0; ) {
        siftDown(i);
    }
}


}