# Thread-safe LRU Cache (C++17)

An **in-memory LRU (Least Recently Used) cache** implemented in C++17.
This project demonstrates templates, hash-based lookup, an intrusive doubly
linked list, RAII ownership, and mutex synchronization. It is **not** a
hardware CPU/GPU cache simulator or a compiler project.

## How it works

- A `std::unordered_map<K, std::unique_ptr<Node<K,V>>>` owns cache entries
  and provides **average O(1)** key lookup.
- A custom doubly linked list stores non-owning node pointers in **MRU → LRU**
  order; moving a node or removing the tail is O(1).
- `get(key)` returns `std::optional<V>`: an absent key returns
  `std::nullopt`, while a hit moves the entry to MRU.
- `put(key, value)` inserts or updates an entry and moves it to MRU; inserting
  beyond capacity evicts the LRU entry.
- `std::lock_guard<std::mutex>` protects both structures so concurrent
  `get`, `put`, and `size` calls do not race.

**Constraints:** The capacity must be positive. Keys must support hashing and
equality; keys and values must be copyable. Supplying custom hash and equality
types is supported by the third and fourth template parameters. Cache destruction
must not overlap with ongoing operations.

**Complexities:** `get` and `put` are O(1) on average (not worst-case) because
hash tables can have collisions and rehashing. Operations also incur a mutex
critical section and copying the stored value.

## Build and run

Requires a C++17 compiler and POSIX threads (Linux/macOS):

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -pthread -I. main.cpp -o lru_demo
./lru_demo
```

Run the regression and concurrency tests:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -pthread -I. tests/test_lru.cpp -o lru_tests
./lru_tests
```

AddressSanitizer + UndefinedBehaviorSanitizer:

```bash
g++ -std=c++17 -g -O1 -Wall -Wextra -pthread -fsanitize=address,undefined \
    -fno-omit-frame-pointer -I. tests/test_lru.cpp -o lru_tests_asan
./lru_tests_asan
```

## Tests

The tests cover missing keys, correct eviction and recency, updating an existing
entry, string values (which catch invalid numeric cache-miss sentinels),
single-entry capacity, rejection of zero capacity, non-default-constructible
keys with custom hashing, and basic concurrent read/write operations.

