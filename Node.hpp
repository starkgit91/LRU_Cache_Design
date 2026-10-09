template <typename K, typename V>
struct Node {
    K key;
    V value;
    Node* prev;
    Node* next;

    Node(K k, V v) {
        key = k;
        value = v;
        next = nullptr;
        prev = nullptr;
    }
};