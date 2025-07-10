#pragma once

#include <cstddef>
#include <cassert>
#include <utility>

namespace ds
{

// DoublyList: A circular doubly linked list with a sentinel head node
template <typename T>
class DoublyList{
public:
    using size_type = std::size_t;

private:
    struct Node {
        T data;
        Node* prev;
        Node* next;

        // For sentinel head
        Node() : data{}, prev(this), next(this) {}
        
        // Perfect forwarding
        template <class U>
        explicit Node(U&& v) 
            : data(std::forward<U>(v)), prev(nullptr), next(nullptr) 
        {}

        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
    };


    Node* head_;  // Sentinel (dummy) head
    size_type size_;

public:
    DoublyList();
    ~DoublyList();

    DoublyList(const DoublyList& other);
    DoublyList& operator=(DoublyList other) noexcept;

    DoublyList(DoublyList&& other) noexcept;
    
    //DoublyList& operator=(DoublyList&& other) noexcept;

    template <class U>
    void pushFront(U&& value);

    template <class U>
    void pushBack(U&& value);

    void popFront();
    void popBack();

    bool contains(const T& value) const;

    template <class U>
    bool insertBefore(const T& key, U&& value);

    template <class U>
    bool insertAfter(const T& key, U&& value);

    bool erase(const T& value);

    size_type size() const;
    bool empty() const;

    void clear();

private:
    void swap(DoublyList& other) noexcept;

    template <class U>
    Node* insertAfterNode(Node* pos, U&& value);

    void eraseNode(Node* pos) noexcept;

    Node* find(const T& value) const noexcept;
};

template <typename T>
DoublyList<T>::DoublyList() 
    : head_(new Node()), size_(0) {
    // head_->next = head_;
    // head_->prev = head_;
}

template <typename T>
DoublyList<T>::~DoublyList() {
    if (head_) {
        clear();
        delete head_;
    }
}

template <typename T>
DoublyList<T>::DoublyList(const DoublyList<T>& other)
    : head_(new Node()), size_(0) {
    // head_->next = head_;
    // head_->prev = head_;

    Node* curr = other.head_->next;
    while (curr != other.head_) {
        pushBack(curr->data);
        curr = curr->next;
    }
}

template <typename T>
DoublyList<T>& DoublyList<T>::operator=(DoublyList<T> other) noexcept {
    swap(other);
    return *this;
}


template <typename T>
DoublyList<T>::DoublyList(DoublyList<T>&& other) noexcept
    : head_(other.head_), size_(other.size_) {
    other.head_ = nullptr;
    other.size_ = 0;
}

// template <typename T>
// DoublyList<T>& DoublyList<T>::operator=(DoublyList<T>&& other) noexcept {
//     swap(other);
//     return *this;
// }

template <typename T>
template <class U>
void DoublyList<T>::pushFront(U&& value) {
    insertAfterNode(head_, std::forward<U>(value));
}

template <typename T>
template <class U>
void DoublyList<T>::pushBack(U&& value) {
    insertAfterNode(head_->prev, std::forward<U>(value));
}

template <typename T>
void DoublyList<T>::popFront() {
    assert(!empty()); // Cannot pop from empty list

    eraseNode(head_->next);
}

template <typename T>
void DoublyList<T>::popBack() {
    assert(!empty());

    eraseNode(head_->prev);
}

template <typename T>
bool DoublyList<T>::contains(const T& value) const {
    return find(value) != nullptr;
}

template <typename T>
template <class U>
bool DoublyList<T>::insertBefore(const T& key, U&& value) {
    if (Node* pos = find(key)) {
        insertAfterNode(pos->prev, std::forward<U>(value));
        return true;
    }
    return false;
}

template <typename T>
template <class U>
bool DoublyList<T>::insertAfter(const T& key, U&& value) {
    if (Node* pos = find(key)) {
        insertAfterNode(pos, std::forward<U>(value));
        return true;
    }
    return false;
}


template <typename T>
bool DoublyList<T>::erase(const T& value) {
    if (Node* pos = find(value)) {
        eraseNode(pos);
        return true;
    }
    return false;
}

template <typename T>
typename DoublyList<T>::size_type DoublyList<T>::size() const {
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
void DoublyList<T>::swap(DoublyList<T>& other) noexcept {
    using std::swap;
    swap(head_, other.head_);
    swap(size_, other.size_);
}

template <typename T>
template <class U>
typename DoublyList<T>::Node* DoublyList<T>::insertAfterNode(typename DoublyList<T>::Node* pos, U&& value) {
    Node* newNode = new Node(std::forward<U>(value));
    newNode->next = pos->next;
    newNode->prev = pos;
    pos->next->prev = newNode;
    pos->next = newNode;
    ++size_;
    return newNode;
}

template <typename T>
void DoublyList<T>::eraseNode(Node* pos) noexcept {
    pos->prev->next = pos->next;
    pos->next->prev = pos->prev;
    delete pos;
    --size_;
}

template <typename T>
typename DoublyList<T>::Node* DoublyList<T>::find(const T& value) const noexcept {
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