#pragma <once>
#include <cstddef>
#include <cassert>
#include <iostream>

namespace ds{

template<typename T>
class SeqList{
public:
    // Constructor & Destructor
    SeqList();
    ~SeqList();

    // Copy and Move
    SeqList(const SeqList<T>& other);
    SeqList<T>& operator=(const SeqList<T>& other);
    SeqList(SeqList<T>&& other) noexcept;
    SeqList<T>& operator=(SeqList<T>&& other) noexcept;

    // Core operations
    void pushBack(const T& value);
    void pushFront(const T& value);
    void popBack();
    void popFront();

    // Move version
    void pushBack(T&& value);
    void pushFront(T&& value);

    void insert(size_t pos, const T& value);
    void erase(size_t pos);

    int find(const T& value) const;
    void set(size_t pos, const T& value);

    size_t size() const;
    bool empty() const;

    T& operator[](size_t pos);
    const T& operator[](size_t pos) const;

    void print() const;  // Debug print

private:
    void ensureCapacity();

private:
    T* data_;            // pointer to dynamic array
    size_t size_;        // current number of elements
    size_t capacity_;    // current capacity
    static constexpr size_t kInitialCapacity = 4;
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
    for (size_t i = 0; i < other.size_; ++i) {
        data_[i] = other.data_[i];
    }
}

// Copy assignment operator
template <typename T>
SeqList<T>& SeqList<T>::operator=(const SeqList<T>& other) {
    if (this != &other) {
        T* newData = new T[other.capacity_];
        for (size_t i = 0; i < other.size_; ++i) {
            newData[i] = other.data_[i];
        }

        delete[] data_;
        data_ = newData;
        size_ = other.size_;
        capacity_ = other.capacity_;
    }
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
    if (this != &other) {
        delete[] data_;

        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;

        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
}

template<typename T>
void SeqList<T>::ensureCapacity() {
    if (size_ >= capacity_) {
        // Double the capacity (or use initial capacity if capacity_ is 0)
        size_t newCapacity = capacity_ == 0 ? kInitialCapacity : capacity_ * 2;

        // allocate a new array with the new capacity
        T* newData = new T[newCapacity];

        // Copy existing elements to the new array
        for (size_t i = 0; i < size_; ++i) {
            newData[i] = data_[i];
        }

        // Release the old array and update the internol pointers
        delete[] data_;
        data_ = newData;
        capacity_ = newCapacity;
    }
}

template<typename T>
void SeqList<T>::pushBack(const T& value) {
    ensureCapacity();   // Ensure enough capacity
    
    data_[size_++] = value; // Insert the value safely
}

template<typename T>
void SeqList<T>::pushFront(const T& value) {
    ensureCapacity();

    // Shift all elements to the right and make room at the front
    for (size_t i = size_; i > 0; --i) {
        data_[i] = std::move_if_noexcept(data_[i - 1]);
    }

    data_[0] = value;
    ++size_;
}

template<typename T>
void SeqList<T>::popBack() {
    assert(size_ > 0 && "Cannot pop from an empty list.");
    --size_;
}

template<typename T>
void SeqList<T>::popFront() {
    assert(size_ > 0 && "Cannot pop from an empty list.");

    // Shift elements to the left to remove the front
    for (size_t i = 1; i < size_; ++i) {
        data_[i - 1] = std::move_if_noexcept(data_[i]);
    }
    --size_;
}

template <typename T>
void SeqList<T>::pushBack(T&& value) {
    ensureCapacity();
    data_[size_++] = std::move(value);
}
    
template <typename T>
void SeqList<T>::pushFront(T&& value) {
    ensureCapacity();
    
    // Shift elements to the right and make room at the front
    for (size_t i = size_; i > 0; --i) {
        data_[i] = std::move_if_noexcept(data_[i - 1]);
    }
    data_[0] = std::move(value);
    ++size_;
}

template<typename T>
void SeqList<T>::insert(size_t pos, const T& value) {
    assert(pos <= size_); // Position must be within [0, size_]
    ensureCapacity();
    
    // Shift elements to the right
    for (size_t i = size_; i > pos; --i) {
        data_[i] = std::move_if_noexcept(data_[i - 1]);
    }

    data_[pos] = value;
    ++size_;
}

template<typename T>
void SeqList<T>::erase(size_t pos) {
    assert(pos < size_); // Position must be within [0, size_)
    
    // Shift elements to the left
    for (size_t i = pos; i < size_ - 1; ++i) {
        data_[i] = std::move_if_noexcept(data_[i + 1]);
    }

    --size_;
}

// Find the first index of the given value; return -1 if not found
template<typename T>
int SeqList<T>::find(const T& value) const {
    for (size_t i = 0; i < size_; ++i) {
        if (data_[i] == value) {
            return static_cast<int>(i);
        }
    } 
    return -1;
}

template<typename T>
void SeqList<T>::set(size_t pos, const T& value) {
    assert(pos < size_);
    data_[pos] = value;
}

template<typename T>
size_t SeqList<T>::size() const {
    return size_;
}

template<typename T>
bool SeqList<T>::empty() const {
    return size_ == 0;
}

template<typename T>
T& SeqList<T>::operator[](size_t pos) {
    assert(pos < size_);
    return data_[pos];
}

template<typename T>
const T& SeqList<T>::operator[](size_t pos) const {
    assert(pos < size_);
    return data_[pos];
}

template<typename T>
void SeqList<T>::print() const {
    // Print all elements in the sequence list
    std::cout << "[";
    for (size_t i = 0; i < size_; ++i) {
        std::cout << data_[i];
        if (i + 1 < size_) {
            std::cout << ", ";
        }
    }
    std::cout << "]\n";
}

}