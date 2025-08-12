#pragma once

#include <cstddef>
#include <cassert>
#include <utility>

namespace ds {

template <typename K, typename V>
class AVLTree {
public:
    using size_type = std::size_t;

private:
    struct Node {
        K key;
        V value;
        int height;
        Node* left;
        Node* right;
        Node* parent;

        template<typename Key, typename Value>
        Node(Key&& k, Value&& v, Node* p = nullptr) 
            : key(std::forward<Key>(k))
            , value(std::forward<Value>(v))
            , height(1)
            , left(nullptr)
            , right(nullptr)
            , parent(p) {}

        Node(const Node&) = delete;
        Node* operator=(const Node&) = delete;
    };

public:
    AVLTree();
    ~AVLTree();

    AVLTree(const AVLTree& other);
    AVLTree(AVLTree&& other) noexcept;
    
    AVLTree& operator=(AVLTree other) noexcept;

    // Insert or update
    template <typename Key, typename Value>
    void insert(Key&& key, Value&& value);

    bool erase(const K& key);

    bool contains(const K& key) const;

    V* find(const K& key);
    const V* find(const K& key) const;

    size_type size() const;
    bool empty() const;
    void clear();

private:
    void swap(AVLTree& other) noexcept;

    void destroy(Node* node);
    Node* clone(Node* node, Node* parent);

    int height(Node* node) const;
    void updateHeight(Node* node);
    // bf = left height - right height
    int getBalanceFactor(Node* node) const;

    Node* balance(Node* node);

    Node* rotateLeft(Node* node);
    Node* rotateRight(Node* node);

    Node* findMin(Node* node) const;
    Node* eraseMin(Node* node);

    Node* find(Node* node, const K& key);
    const Node* find(Node* node, const K& key) const;


private:
    size_type size_ = 0;
    Node* root_ = nullptr;
};

template<typename K, typename V>
AVLTree<K, V>::AVLTree()
    :size_(0), root_(nullptr) 
{}

template<typename K, typename V>
AVLTree<K, V>::~AVLTree() {
    destroy(root_);
}

template<typename K, typename V>
AVLTree<K, V>::AVLTree(const AVLTree& other) 
    : size_(other.size_), root_(clone(other.root_, nullptr)) 
{}

template<typename K, typename V>
AVLTree<K, V>::AVLTree(AVLTree&& other) noexcept
    : size_(other.size_), root_(other.root_) {
    other.root_ = nullptr;
    other.size_ = 0;
}

template<typename K, typename V>
AVLTree<K, V>& AVLTree<K, V>::operator=(AVLTree other) noexcept{
    swap(other);
    return *this;
}

template <typename K, typename V>
template <typename Key, typename Value>
void AVLTree<K, V>::insert(Key&& key, Value&& value) {
    if (!root_) {
        root_ = new Node(std::forward<Key>(key), std::forward<Value>(value));
        ++size_;
        return;
    }

    Node* curr = root_;
    Node* prev = nullptr;
    while (curr) {
        prev = curr;
        if (key < curr->key) {
            curr = curr->left;
        } else if (key > curr->key) {
            curr = curr->right;
        } else {
            curr->value = std::forward<Value>(value);
            return;
        }
    }

    Node* newNode = new Node(std::forward<Key>(key), std::forward<Value>(value), prev);
    if (key < prev->key) {
        prev->left = newNode;
    } else {
        prev->right = newNode;
    }
    ++size_;

    for (Node* p = prev; p != nullptr; p = p->parent) {
        updateHeight(p);

        int bf = getBalanceFactor(p);
        if (bf > 1 || bf < -1) {
            balance(p);
            break;
        }
    }
}


template <typename K, typename V>
bool AVLTree<K, V>::erase(const K& key)
{
    Node* target = root_;
    while (target && target->key != key) {
        target = (key < target->key) ? target->left : target->right;
    }
    if (!target) {
        return false;
    }

    if (target->left && target->right) {
        Node* succ = findMin(target->right); 
        target->key   = succ->key; 
        target->value = std::move(succ->value);
        target = succ; 
    }

    Node* child  = target->left ? target->left : target->right;
    Node* parent = target->parent;

    if (child) {
        child->parent = parent;
    }

    if (!parent) { 
        root_ = child;
    } else if (parent->left == target) {
        parent->left = child;
    } else {
        parent->right = child;
    }

    delete target;
    --size_;

    for (Node* p = parent; p != nullptr; p = p->parent) {
        updateHeight(p);
        int bf = getBalanceFactor(p);
        if (bf > 1 || bf < -1) {
            p = balance(p); 
        }
    }

    return true;
}

template <typename K, typename V>
bool AVLTree<K, V>::contains(const K& key) const
{
    Node* cur = root_;
    while (cur) {
        if ( key < cur->key ) {
            cur = cur->left;
        }
        else if ( key > cur->key ) {
            cur = cur->right;
        }
        else {
            return true;
        }
    }
    return false;
}

template <typename K, typename V>
V* AVLTree<K, V>::find(const K& key)
{
    Node* cur = root_;
    while (cur) {
        if ( key < cur->key ) {
            cur = cur->left;
        }
        else if ( key > cur->key ) {
            cur = cur->right;
        }
        else {
            return &cur->value;
        }
    }
    return nullptr;
}

template <typename K, typename V>
const V* AVLTree<K, V>::find(const K& key) const
{
    const Node* cur = root_;
    while (cur) {
        if ( key < cur->key ) {
            cur = cur->left;
        }
        else if ( key > cur->key ) {
            cur = cur->right;
        }
        else {
            return &cur->value;
        }
    }
    return nullptr;
}

template <typename K, typename V>
typename AVLTree<K, V>::size_type
AVLTree<K, V>::size() const
{
    return size_;
}

template <typename K, typename V>
bool AVLTree<K, V>::empty() const
{
    return size_ == 0;
}


template<typename K, typename V>
void AVLTree<K, V>::clear() {
    destroy(root_);
    root_ = nullptr;
    size_ = 0;
}


template<typename K, typename V>
void AVLTree<K, V>::swap(AVLTree& other) noexcept {
    using std::swap;
    swap(size_, other.size_);
    swap(root_, other.root_);
}

template<typename K, typename V>
void AVLTree<K, V>::destroy(Node* nood) {
    if (!nood) {
        return;
    }
    destroy(nood->left);
    destroy(nood->right);
    delete nood;
}

template<typename K, typename V>
typename AVLTree<K, V>::Node* AVLTree<K, V>::clone(Node* node, Node* parent) {
    if (!node) {
        return nullptr;
    }
    Node* newNode = new Node(node->key, node->value, node->parent);
    newNode->height = node->height;

    newNode->left = clone(node->left, newNode);
    newNode->right = clone(node->right, newNode);

    return newNode;
}

template<typename K, typename V>
int AVLTree<K, V>::height(Node* node) const {
    return node ? node->height : 0;
}

template<typename K, typename V>
void AVLTree<K, V>::updateHeight(Node* node) {
    node->height = 1 + std::max(height(node->left), height(node->right));
}

template<typename K, typename V>
int AVLTree<K, V>::getBalanceFactor(Node* node) const {
    return height(node->left) - height(node->right);
}

template<typename K, typename V>
typename AVLTree<K, V>::Node* AVLTree<K, V>::balance(Node* node) {
    int bf = getBalanceFactor(node);

    if (bf > 1) {
        if (getBalanceFactor(node->left) < 0) {
            rotateLeft(node->left);
        }
        return rotateRight(node);
    }
    if (bf < 1) {
        if (getBalanceFactor(node->right) < 0) {
            rotateRight(node->right);
        }
        return rotateLeft(node);
    }
    return node;
}
    
template<typename K, typename V>
typename AVLTree<K, V>::Node* AVLTree<K, V>::rotateLeft(Node* node) {
    Node* rightChild = node->right;
    Node* rightLeftChild = rightChild->left;

    // rotate
    node->right = rightLeftChild;
    rightChild->left = node;

    // update parent
    rightChild->parent = node->parent;
    node->parent = rightChild;
    if (rightLeftChild) {
        rightLeftChild->parent = node;
    }

    // update ancestor or root_
    if (!rightChild->parent) {
        root_ = rightChild;
    } else if (rightChild->parent->left == node) {
        rightChild->parent->left = rightChild;
    } else {
        rightChild->parent->right = rightChild;
    }

    // update height (bottom-up)
    updateHeight(node);
    updateHeight(rightChild);

    return rightChild;
}

template<typename K, typename V>
typename AVLTree<K, V>::Node* AVLTree<K, V>::rotateRight(Node* node) {
    Node* leftChild = node->left;
    Node* leftRightChild = leftChild->right;

    node->left = leftRightChild;
    leftChild->right = node;

    leftChild->parent = node->parent;
    node->parent = leftChild;
    if (leftRightChild) {
        leftRightChild->parent = node;
    }

    if (!leftChild->parent) {
        root_ = leftChild;
    } else if (leftChild->parent->right == node) {
        leftChild->parent->right = leftChild;
    } else {
        leftChild->parent->left = leftChild;
    }

    updateHeight(node);
    updateHeight(leftChild);

    return leftChild;
}

template <typename K, typename V>
typename AVLTree<K, V>::Node* AVLTree<K, V>::findMin(Node* node) const {
    if (!node) {
        return nullptr;
    }
    while (node->left) {
        node = node->left;
    }
    return node;
}

}