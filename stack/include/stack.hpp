#pragma once

#include <cstddef>
#include <cassert>
#include <utility>

namespace ds{

template<typename T>
class Stack{
public:
    using size_type = std::size_t;

    Stack();
    ~Stack();

    Stack(const Stack& other);
    Stack& operator=(Stack other) noexcept;

    Stack(Stack&& other) noexcept;
    
    //Stack& operator=(Stack&& other) noexcept;

    template<class U>
    void push(U&& value);

    void pop();

    T& top();
    const T& top() const;

    bool empty() const;
    size_type size() const;

private:
    void ensureCapacity();
    void swap(Stack& other) noexcept;

private:
    size_type size_;
    size_type capacity_;
    T* data_;
    
    static constexpr size_type kInitialCapacity = 4;
};

template<typename T>
Stack<T>::Stack()
    :size_(0), capacity_(kInitialCapacity), data_(new T[kInitialCapacity]){}

template<typename T>
Stack<T>::~Stack() {
    delete[] data_;
    data_ = nullptr;
    size_ = 0;
    capacity_ = 0;
}

template<typename T>
Stack<T>::Stack(const Stack<T>& other)
    :size_(other.size_), capacity_(other.capacity_), data_(new T(other.capacity_)) {
    for (size_type i = 0; i < size_; ++i) {
        data_[i] = other.data_[i];
    }
}

template<typename T>
Stack<T>& Stack<T>::operator=(Stack<T> other) noexcept{
    swap(other);
    return *this;
}

template<typename T>
Stack<T>::Stack(Stack<T>&& other) noexcept
    :size_(other.size_), capacity_(other.capacity_), data_(other.data_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

// template<typename T>
// Stack<T>& Stack<T>::operator=(Stack<T>&& other) noexcept {
//     swap(other);
//     return *this;
// }

template<typename T>
template<class U>
void Stack<T>::push(U&& value) {
    ensureCapacity();
    data_[size_++] = std::forward<U>(value);
}

template<typename T>
void Stack<T>::pop() {
    assert(size_ > 0 && "Stack underflow");
    --size_;
}

template <typename T>
T& Stack<T>::top() {
    assert(size_ > 0 && "Empty stack");
    return data_[size_ - 1];
}

template <typename T>
const T& Stack<T>::top() const {
    assert(size_ > 0 && "Empty stack");
    return data_[size_ - 1];
}

template <typename T>
typename Stack<T>::size_type Stack<T>::size() const {
    return size_;
}

template <typename T>
bool Stack<T>::empty() const {
    return size_ == 0;
}

template<typename T>
void Stack<T>::ensureCapacity() {
    if (size_ >= capacity_) {
        size_type newCapacity = capacity_ == 0 ? kInitialCapacity : capacity_ * 2;
        T* newData = new T[newCapacity];
        for (size_type i = 0; i < size_; ++i) {
            newData[i] = std::move_if_noexcept(data_[i]);
        }
        delete[] data_;
        data_ = newData;
        capacity_ = newCapacity;
    }
}

template<typename T>
void Stack<T>::swap(Stack<T>& other) noexcept{
    using std::swap;
    swap(size_, other.size_);
    swap(capacity_, other.capacity_);
    swap(data_, other.data_);
}

}