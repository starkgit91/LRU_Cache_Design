template <typename K, typename V>
class DoublyLinkedList {
public:
    Node<K, V>* head;
    Node<K, V>* tail;
    DoublyLinkedList() {
        // TODO: Create dummy head and tail nodes
        // Use default values for K and V (e.g., K{} and V{})
        // TODO: Link head->next to tail and tail->prev to head
        head = new Node<K,V>(K{},V{});
        tail = new Node<K,V>(K{},V{});
        head->next = tail;
        tail->prev = head;
    }

    ~DoublyLinkedList() {
        // TODO: Delete the dummy head and tail nodes
        // Note: Real data nodes are deleted by LRUCache
        delete head;
        delete tail;
    }

    void addFirst(Node<K, V>* node) {
        Node<K,V> *nextNode = head->next;
        head->next = node;
        node->prev = head;
        node->next = nextNode;
        nextNode->prev = node;
        // TODO: Insert node right after head
        // Steps:
        // 1. node->next = head->next
        // 2. node->prev = head
        // 3. head->next->prev = node
        // 4. head->next = node
    }

    void remove(Node<K, V>* node) {
        Node<K,V> *prevNode = node->prev;
        Node<K,V> *nextNode = node->next;
        prevNode->next = nextNode;
        nextNode->prev = prevNode;
        // TODO: Detach node from its current position
        // Steps:
        // 1. node->prev->next = node->next
        // 2. node->next->prev = node->prev
    }

    void moveToFront(Node<K, V>* node) {
        // TODO: Move an existing node to the front
        // Hint: Remove it first, then add it to front
    }

    Node<K, V>* removeLast() {
        // TODO: Remove and return the node just before tail (the LRU node)
        // Steps:
        // 1. Check if list is empty (tail->prev == head), return nullptr if so
        // 2. Get the last real node (tail->prev)
        // 3. Remove it using the remove() method
        // 4. Return the removed node
        return nullptr;
    }
};