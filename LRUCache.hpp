template <typename K, typename V>
class LRUCache {
private:
    int capacity;
    unordered_map<K, Node<K, V>*> map;
    DoublyLinkedList<K, V> list;
    mutex mtx;

public:
    LRUCache(int cap) {
        // TODO: Initialize capacity
        // The map and list are default-initialized
        this->capacity = cap;
    }

    ~LRUCache() {
        // TODO: Delete all nodes stored in the map
        // Iterate through the map and delete each node pointer
        for( auto &it:map){
            delete it.second;
        }
    }

    optional<V> get(const K& key) {
        lock_guard<mutex> lock(mtx);
        
        if(map.find(key)==map.end()){
            return -1;
        }
        Node<K,V> *node = map[key];
        list.remove(node);
        list.addFirst(node);
        return node->value;
        // TODO: Implement get operation
        // Steps:
        // 1. If key not in map, return std::nullopt
        // 2. Get the node pointer from the map
        // 3. Move the node to front (mark as most recently used)
        // 4. Return the node's value
        // return std::nullopt;
    }

    void put(const K& key, const V& value) {
       lock_guard<mutex> lock(mtx);
        if(map.find(key)!=map.end()){
            Node<K,V> *oldNode = map[key];
            list.remove(oldNode);
            delete oldNode;
        }
        Node<K,V> *node = new Node<K,V>(key,value);
        map[key] = node;
        list.addFirst(node); // -> MRU
        if(map.size()>capacity){
            Node<K,V> *LRU = list.tail->prev;
            list.remove(LRU);
            map.erase(LRU->key);
            delete LRU;
        }
        // TODO: Implement put operation
        // Case 1: Key already exists
        //   - Get the existing node
        //   - Update its value
        //   - Move it to front
        //
        // Case 2: Key is new
        //   - If at capacity, evict LRU item:
        //     - Remove last node from list
        //     - Remove its key from map
        //     - Delete the node
        //   - Create new node
        //   - Add to front of list
        //   - Add to map
    }
};