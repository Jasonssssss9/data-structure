#pragma once

#include <cstddef>
#include <cassert>
#include <utility>

namespace ds
{

// DoublyList: A circular doubly linked list with a sentinel head node
template <typename T>
class DoublyList{
private:
    // Node structure
    struct Node {
        T data;
        Node* prev;
        Node* next;

        Node(const T& value) :data(value), prev(nullptr), next(nullptr) {}
        Node(T&& value) :data(std::move(value)), prev(nullptr), next(nullptr) {}
    };

    Node* head_;  // Sentinel (dummy) head
    size_t size_;

public:
    DoublyList();
    ~DoublyList();

    DoublyList(const DoublyList& other);
    DoublyList& operator=(const DoublyList& other);

    DoublyList(DoublyList&& other);
    DoublyList& operator=(DoublyList&& other);

    void pushFront(const T& value);
    void pushBack(const T& value);

    void pushFront(T&& value);
    void pushBack(T&& value);

    void popFront();
    void popBack();

    bool contains(const T& value) const;

    bool insertBefore(const T& key, const T& newVal);
    bool insertAfter(const T& key, const T& newVal);
    bool erase(const T& value);

    size_t size() const;
    bool empty() const;

    void clear();

private:
    Node* const find(const T& value) const;
};

template <typename T>
DoublyList<T>::DoublyList() 
    : head_(new Node(T{})), size_(0) {
    head_->next = head_;
    head_->prev = head_;
}

template <typename T>
DoublyList<T>::~DoublyList() {
    clear();
    delete head_;
}

template <typename T>
DoublyList<T>::DoublyList(const DoublyList<T>& other)
    : head_(new Node(T{})), size_(0) {
    head_->next = head_;
    head_->prev = head_;

    Node* curr = other.head_->next;
    while (curr != other.head_) {
        pushBack(curr->data);
        curr = curr->next;
    }
}

template <typename T>
DoublyList<T>::DoublyList(DoublyList<T>&& other)
    : head_(other.head_), size_(other.size_) {
    other.head_ = new Node(T{});
    other.head_->next = other.head_;
    other.head_->prev = other.head_;
    other.size_ = 0;
}

template <typename T>
DoublyList<T>& DoublyList<T>::operator=(const DoublyList<T>& other) {
    if (this != &other) {
        clear();

        Node* curr = other.head_->next;
        while (curr != other.head_) {
            pushBack(curr->data);
            curr = curr->next;
        }
    }
    return *this;
}

template <typename T>
DoublyList<T>& DoublyList<T>::operator=(DoublyList<T>&& other) {
    if (this != &other) {
        clear();
        delete head_;

        head_ = other.head_;
        size_ = other.size_;

        other.head_ = new Node(T{});
        other.head_->next = other.head_;
        other.head_->prev = other.head_;
        other.size_ = 0;
    }
    return *this;
}

template <typename T>
void DoublyList<T>::pushFront(const T& value) {
    Node* newNode = new Node(value);
    Node* front = head_->next;
    
    head_->next = newNode;
    newNode->prev = head_;
    newNode->next = front;
    front->prev = newNode;

    ++size_;
}

template <typename T>
void DoublyList<T>::pushBack(const T& value) {
    Node* newNode = new Node(value);
    Node* tail = head_->prev;

    head_->prev = newNode;
    newNode->next = head_;
    newNode->prev = tail;
    tail->next = newNode;

    ++size_;
}

template <typename T>
void DoublyList<T>::pushFront(T&& value) {
    Node* newNode = new Node(std::move(value));
    Node* front = head_->next;
    
    head_->next = newNode;
    newNode->prev = head_;
    newNode->next = front;
    front->prev = newNode;

    ++size_; 
}

template <typename T>
void DoublyList<T>::pushBack(T&& value) {
    Node* newNode = new Node(std::move(value));
    Node* tail = head_->prev;

    head_->prev = newNode;
    newNode->next = head_;
    newNode->prev = tail;
    tail->next = newNode;

    ++size_;
}

template <typename T>
void DoublyList<T>::popFront() {
    assert(!empty()); // Cannot pop from empty list

    Node* front = head_->next;
    head_->next = front->next;
    front->next->prev = head_;

    delete front;
    --size_;
}

template <typename T>
void DoublyList<T>::popBack() {
    assert(!empty());

    Node* tail = head_->prev;
    tail->prev->next = head_;
    head_->prev = tail->prev;

    delete tail;
    --size_;
}

template <typename T>
bool DoublyList<T>::contains(const T& value) const {
    return find(value) != nullptr;
}

template <typename T>
bool DoublyList<T>::insertBefore(const T& key, const T& newVal) {
    Node* pos = find(key);
    if (pos == nullptr) {
        return false;
    }

    Node* newNode = new Node(newVal);
    newNode->prev = pos->prev;
    pos->prev->next = newNode;
    newNode->next = pos;
    pos->prev = newNode;
    ++size_;
    return true;
}

template <typename T>
bool DoublyList<T>::insertAfter(const T& key, const T& newVal) {
    Node* pos = find(key);
    if (pos == nullptr) {
        return false;
    }

    Node* newNode = new Node(newVal);
    newNode->next = pos->next;
    pos->next->prev = newNode;
    newNode->prev = pos;
    pos->next = newNode;
    ++size_;
    return true;
}

template <typename T>
bool DoublyList<T>::erase(const T& value) {
    Node* pos = find(value);
    if (pos == nullptr) {
        return false;
    }

    pos->prev->next = pos->next;
    pos->next->prev = pos->prev;
    delete pos;
    pos = nullptr;
    --size_;
    return true;
}


template <typename T>
size_t DoublyList<T>::size() const {
    return size_;
}

template <typename T>
bool DoublyList<T>::empty() const {
    return size_ == 0;
}


template <typename T>
void DoublyList<T>::clear() {
    while (!empty()) {
        popBack();
    }
}

template <typename T>
typename DoublyList<T>::Node* const DoublyList<T>::find(const T& value) const {
    Node* curr = head_->next;
    while (curr != head_) {
        if (curr->data == value) {
            return curr;
        }
        curr = curr->next;
    }
    return nullptr;
}

}