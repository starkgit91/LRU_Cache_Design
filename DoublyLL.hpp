#pragma once

#include "Node.hpp"

// Intrusive, non-owning doubly linked list. The front is most recently used.
// This implementation does not need dummy nodes or default-constructible keys.
template <typename K, typename V>
class DoublyLinkedList {
public:
    using Entry = Node<K, V>;

    DoublyLinkedList() = default;
    DoublyLinkedList(const DoublyLinkedList&) = delete;
    DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;

    void addFirst(Entry* node) noexcept {
        node->prev = nullptr;
        node->next = head_;
        if (head_ != nullptr) {
            head_->prev = node;
        } else {
            tail_ = node;
        }
        head_ = node;
    }

    void remove(Entry* node) noexcept {
        if (node->prev != nullptr) {
            node->prev->next = node->next;
        } else {
            head_ = node->next;
        }

        if (node->next != nullptr) {
            node->next->prev = node->prev;
        } else {
            tail_ = node->prev;
        }

        node->prev = nullptr;
        node->next = nullptr;
    }

    void moveToFront(Entry* node) noexcept {
        if (node != head_) {
            remove(node);
            addFirst(node);
        }
    }

    Entry* removeLast() noexcept {
        Entry* last = tail_;
        if (last != nullptr) {
            remove(last);
        }
        return last;
    }

    Entry* last() const noexcept { return tail_; }
    bool empty() const noexcept { return head_ == nullptr; }

private:
    Entry* head_ = nullptr;
    Entry* tail_ = nullptr;
};
