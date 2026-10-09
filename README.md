This is generic LRU Cache written in C++17 for cache and multithreading tasks equipped with mutex lock_guards to mitigate resource deadlocks.
* Designed a generic C++ LRU cache using hash maps and doubly linked lists, enabling average O(1) lookup, insertion, and eviction.
* Implemented capacity-based LRU eviction, MRU updates, key replacement, and explicit dynamic memory management.
* Integrated mutex-based synchronization with std::lock_guard for thread-safe cache access and updates.
