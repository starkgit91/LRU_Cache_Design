#pragma once

#include <cstddef>
#include <memory>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <unordered_map>
#include <utility>

#include "DoublyLL.hpp"

// Thread-safe, bounded C++17 LRU cache.
// Average complexity: O(1) for get/put/evict, assuming average O(1) hashing.
// K must be hashable and equality-comparable; K and V must be copyable.
// The unordered_map uniquely owns nodes; the list maintains non-owning links.
template <typename K, typename V, typename Hash = std::hash<K>,
          typename KeyEqual = std::equal_to<K>>
class LRUCache {
public:
    explicit LRUCache(std::size_t capacity) : capacity_(capacity) {
        if (capacity == 0) {
            throw std::invalid_argument("LRUCache capacity must be positive");
        }
    }

    LRUCache(const LRUCache&) = delete;
    LRUCache& operator=(const LRUCache&) = delete;
    LRUCache(LRUCache&&) = delete;
    LRUCache& operator=(LRUCache&&) = delete;

    // Returns nullopt on a cache miss, including for non-numeric value types.
    // A cache hit marks the corresponding entry as most recently used.
    std::optional<V> get(const K& key) {
        std::lock_guard<std::mutex> lock(mutex_);
        const auto it = nodes_.find(key);
        if (it == nodes_.end()) {
            return std::nullopt;
        }

        // Copy before changing the recency order, in case copying V throws.
        std::optional<V> result(it->second->value);
        order_.moveToFront(it->second.get());
        return result;
    }

    // Insert a new key, or update an existing value and mark it as MRU.
    // If full, evict the least-recently-used entry after successful insertion.
    void put(const K& key, const V& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = nodes_.find(key);
        if (it != nodes_.end()) {
            it->second->value = value;
            order_.moveToFront(it->second.get());
            return;
        }

        auto new_node = std::make_unique<Node<K, V>>(key, value);
        // try_emplace preserves ownership of new_node if insertion throws.
        auto inserted = nodes_.try_emplace(key, std::move(new_node));
        Node<K, V>* node = inserted.first->second.get();
        order_.addFirst(node);

        if (nodes_.size() > capacity_) {
            // Look up the victim before detaching it: a custom hasher may throw.
            Node<K, V>* lru = order_.last();
            auto victim = nodes_.find(lru->key);
            order_.removeLast();
            nodes_.erase(victim);  // unique_ptr deletes the evicted entry.
        }
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return nodes_.size();
    }

    std::size_t capacity() const noexcept { return capacity_; }

private:
    const std::size_t capacity_;
    std::unordered_map<K, std::unique_ptr<Node<K, V>>, Hash, KeyEqual> nodes_;
    DoublyLinkedList<K, V> order_;
    mutable std::mutex mutex_;
};
