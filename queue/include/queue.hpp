#pragma once

#include <cstddef>
#include <cassert>
#include <utility>

namespace ds {

template<typename T>
class Queue{
public:
    using size_type = std::size_t;

private:
    struct Node {
        T data;
        Node* prev;
        Node* next;

        Node() : data(), prev(this), next(this) {}

        template<class U>
        explicit Node(U&& u) 
            : data(std::forward<U>(u)), prev(nullptr), next(nullptr) 
        {}

        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
    };
    
    size_type size_ = 0;
    Node* head_ = nullptr;

public: 
    Queue();
    ~Queue();

    Queue(const Queue& other);
    Queue(Queue&& other) noexcept;
    Queue& operator=(Queue other) noexcept;

    template<class U>
    void push(U&& u);

    void pop();

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    size_type size() const;
    
    bool empty() const;


private:
    void swap(Queue& other) noexcept;
    void clear();
    
};

template<typename T>
Queue<T>::Queue()
    :size_(0), head_(new Node())
{}

template<typename T>
Queue<T>::~Queue() {
    if (head_) {
        clear();
        delete head_;
        head_ = nullptr;
    }
}

template<typename T>
Queue<T>::Queue(const Queue<T>& other) 
    : size_(0), head_(new Node()) {
    Node* curr = other.head_->next;
    while (curr != other.head_) {
        push(curr->data);
        curr = curr->next;
    }
}

template<typename T>
Queue<T>::Queue(Queue<T>&& other) noexcept
    : size_(other.size_), head_(other.head_){
    other.head_ = nullptr;
    other.size_ = 0;
}

template<typename T>
Queue<T>& Queue<T>::operator=(Queue<T> other) noexcept {
    swap(other);
    return *this;
}

template<typename T>
template<class U>
void Queue<T>::push(U&& u) {
    Node* newNode = new Node(std::forward<U>(u));
    newNode->next = head_;
    newNode->prev = head_->prev;
    head_->prev->next = newNode;
    head_->prev = newNode;

    ++size_;
}

template<typename T>
void Queue<T>::pop(){
    assert(!empty() && "Queue is empty");
    Node* front = head_->next;
    head_->next = front->next;
    front->next->prev = head_;
    delete front;

    --size_;
}

template<typename T>
T& Queue<T>::front() {
    assert(!empty() && "Queue is empty");
    return head_->next->data;
}

template<typename T>
const T& Queue<T>::front() const {
    assert(!empty() && "Queue is empty");
    return head_->next->data;
}

template<typename T>
T& Queue<T>::back() {
    assert(!empty() && "Queue is empty");
    return head_->prev->data;
}

template<typename T>
const T& Queue<T>::back() const {
    assert(!empty() && "Queue is empty");
    return head_->prev->data;
}

template<typename T>
typename Queue<T>::size_type Queue<T>::size() const {
    return size_;
}
    
template<typename T>
bool Queue<T>::empty() const {
    return size_ == 0;
}

template<typename T>
void Queue<T>::swap(Queue<T>& other) noexcept {
    using std::swap;
    swap(head_, other.head_);
    swap(size_, other.size_);
}

template<typename T>
void Queue<T>::clear() {
    while (!empty()) {
        pop();
    }
}

}