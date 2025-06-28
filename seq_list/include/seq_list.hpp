#pragma once

#include <cstddef>
#include <cassert>
#include <utility>

namespace ds{

// SeqList: A standard sequential list
template <typename T>
class SeqList{
public:
    using size_type = std::size_t;
    static constexpr size_type npos = static_cast<size_type>(-1);

    // Constructor & Destructor
    SeqList();
    ~SeqList();

    // Copy and Move
    SeqList(const SeqList& other);
    SeqList& operator=(SeqList other) noexcept;
    SeqList(SeqList&& other) noexcept;
    SeqList& operator=(SeqList&& other) noexcept;

    template<class U> 
    void pushBack (U&& value);
    
    template<class U> 
    void pushFront(U&& value);

    void popBack();
    void popFront();

    template<class U>
    bool insert(size_type pos, U&& value);

    bool erase(size_type pos);

    size_type find(const T& value) const noexcept;

    void set(size_type pos, const T& value);

    size_type size() const;
    bool empty() const;

    T& operator[](size_type pos);
    const T& operator[](size_type pos) const;

private:
    void ensureCapacity();

    void swap(SeqList& other) noexcept;

    template<class U>
    void insertAt(size_type pos, U&& value);

    void removeAt(size_type pos) noexcept;

private:
    T* data_;            // pointer to dynamic array
    size_type size_;        // current number of elements
    size_type capacity_;    // current capacity
    static constexpr size_type kInitialCapacity = 4;
};



template <typename T>
SeqList<T>::SeqList()
    : data_(new T[kInitialCapacity]), size_(0), capacity_(kInitialCapacity) {}

template <typename T>
SeqList<T>::~SeqList() {
    delete[] data_;
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
}

// Copy constructor
template <typename T>
SeqList<T>::SeqList(const SeqList<T>& other)
    : data_(new T[other.capacity_]), size_(other.size_), capacity_(other.capacity_) {
    for (size_type i = 0; i < other.size_; ++i) {
        data_[i] = other.data_[i];
    }
}

// Copy assignment operator
template <typename T>
SeqList<T>& SeqList<T>::operator=(SeqList<T> other) noexcept {
    swap(other);
    return *this;
}

// Move constructor
template <typename T>
SeqList<T>::SeqList(SeqList<T>&& other) noexcept 
    :data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

// Move assignment operator
template <typename T>
SeqList<T>& SeqList<T>::operator=(SeqList<T>&& other) noexcept {
    swap(other);
    return *this;
}

template <typename T>
void SeqList<T>::ensureCapacity() {
    if (size_ >= capacity_) {
        // Double the capacity (or use initial capacity if capacity_ is 0)
        size_type newCapacity = capacity_ == 0 ? kInitialCapacity : capacity_ * 2;

        // allocate a new array with the new capacity
        T* newData = new T[newCapacity];

        // Copy existing elements to the new array
        for (size_type i = 0; i < size_; ++i) {
            newData[i] = data_[i];
        }

        // Release the old array and update the internol pointers
        delete[] data_;
        data_ = newData;
        capacity_ = newCapacity;
    }
}

template<typename T>
template<class U>
void SeqList<T>::pushBack(U&& value)
{
    insertAt(size_, std::forward<U>(value));
}

template<typename T>
template<class U>
void SeqList<T>::pushFront(U&& value)
{
    insertAt(0, std::forward<U>(value)); 
}

template <typename T>
void SeqList<T>::popBack() {
    assert(size_ > 0 && "Cannot pop from an empty list.");
    --size_;
}

template <typename T>
void SeqList<T>::popFront() {
    assert(size_ > 0 && "Cannot pop from an empty list.");

    removeAt(0);
}

template<typename T>
template<class U>
bool SeqList<T>::insert(size_type pos, U&& value)
{
    if (pos > size_) {
        return false;
    }
    insertAt(pos, std::forward<U>(value));
    return true;
}

template<typename T>
bool SeqList<T>::erase(size_type pos)
{
    if (pos >= size_) {
        return false;
    }
    removeAt(pos);
    return true;
}


template <typename T>
typename SeqList<T>::size_type SeqList<T>::find(const T& value) const noexcept {
    for (size_type i = 0; i < size_; ++i) {
        if (data_[i] == value) {
            return i; 
        }
    }
    return npos;
}

template <typename T>
void SeqList<T>::set(size_type pos, const T& value) {
    assert(pos < size_);
    data_[pos] = value;
}

template <typename T>
typename SeqList<T>::size_type SeqList<T>::size() const {
    return size_;
}

template <typename T>
bool SeqList<T>::empty() const {
    return size_ == 0;
}

template <typename T>
T& SeqList<T>::operator[](size_type pos) {
    assert(pos < size_);
    return data_[pos];
}

template <typename T>
const T& SeqList<T>::operator[](size_type pos) const {
    assert(pos < size_);
    return data_[pos];
}


template <typename T>
void SeqList<T>::swap(SeqList& other) noexcept {
    using std::swap;
    swap(data_, other.data_);
    swap(size_, other.size_);
    swap(capacity_, other.capacity_);
}

template<typename T>
template<class U>
void SeqList<T>::insertAt(size_type pos, U&& value)
{
    assert(pos <= size_);
    ensureCapacity(); 

    for (size_type i = size_; i > pos; --i) {
        data_[i] = std::move_if_noexcept(data_[i - 1]);
    }

    data_[pos] = std::forward<U>(value);
    ++size_;
}

template<typename T>
void SeqList<T>::removeAt(size_type pos) noexcept
{
    assert(pos < size_);

    for (size_type i = pos + 1; i < size_; ++i) {
        data_[i - 1] = std::move_if_noexcept(data_[i]);
    }
    --size_;
}

}