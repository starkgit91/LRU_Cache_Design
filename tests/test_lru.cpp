#include <cassert>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include "../LRUCache.hpp"

struct Key {
    int id;
    explicit Key(int x) : id(x) {}
    bool operator==(const Key& other) const { return id == other.id; }
};
struct KeyHash {
    std::size_t operator()(const Key& key) const noexcept {
        return std::hash<int>{}(key.id);
    }
};

int main() {
    LRUCache<std::string, int> cache(3);
    assert(!cache.get("missing").has_value());
    cache.put("a", 1);
    cache.put("b", 2);
    cache.put("c", 3);
    assert(cache.size() == 3);
    assert(cache.get("a") == 1);  // b becomes LRU.
    cache.put("d", 4);
    assert(!cache.get("b").has_value());
    assert(cache.get("a") == 1);
    cache.put("c", 30);           // Updating preserves capacity.
    assert(cache.get("c") == 30);
    assert(cache.size() == 3);
    cache.put("e", 5);
    assert(!cache.get("d").has_value());
    assert(cache.get("e") == 5);

    LRUCache<int, std::string> words(1);
    assert(!words.get(99).has_value());
    words.put(1, "hello");
    words.put(2, "world");
    assert(!words.get(1).has_value());
    assert(words.get(2) == std::optional<std::string>("world"));

    bool rejected = false;
    try {
        LRUCache<int, int> bad(0);
    } catch (const std::invalid_argument&) {
        rejected = true;
    }
    assert(rejected);

    LRUCache<Key, std::string, KeyHash> custom(2);
    custom.put(Key(10), "ten");
    assert(custom.get(Key(10)) == std::optional<std::string>("ten"));

    LRUCache<int, int> concurrent(100);
    constexpr int threads = 8;
    constexpr int iterations = 1000;
    std::vector<std::thread> workers;
    for (int t = 0; t < threads; ++t) {
        workers.emplace_back([&, t] {
            for (int i = 0; i < iterations; ++i) {
                const int key = (i + t * iterations) % 150;
                concurrent.put(key, i);
                (void)concurrent.get(key);
                assert(concurrent.size() <= concurrent.capacity());
            }
        });
    }
    for (auto& worker : workers) {
        worker.join();
    }
    assert(concurrent.size() <= concurrent.capacity());
    return 0;
}
