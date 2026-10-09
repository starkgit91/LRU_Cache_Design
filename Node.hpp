#pragma once

// A cache entry. The LRUCache's unordered_map owns each allocated node;
// the doubly linked list only stores non-owning pointers to those nodes.
template <typename K, typename V>
struct Node {
    K key;
    V value;
    Node* prev = nullptr;
    Node* next = nullptr;

    Node(const K& k, const V& v) : key(k), value(v) {}
};
