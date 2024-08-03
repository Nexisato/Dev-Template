#include <assert.h>

#include <iostream>
#include <list>
#include <mutex>
#include <optional>
#include <unordered_map>

template <typename K, typename V>
class ConcurrentLRUCache {
private:
    size_t capacity_;
    std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> cache_;
    std::list<std::pair<K, V>> items_;
    mutable std::mutex mutex_;

public:
    ConcurrentLRUCache(size_t capacity) : capacity_(capacity) {
        assert(capacity_ > 0);
    }

    std::optional<V> get(const K& key) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = cache_.find(key);
        if (it == cache_.end()) {
            return std::nullopt;
        }
        // Move the accessed key to the front of the list
        items_.splice(items_.begin(), items_, it->second);
        // equal to

        // items_.erase(it->second);
        // items_.push_front(*it->second);
        return it->second->second;
    }

    void put(const K& key, const V& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = cache_.find(key);
        if (it != cache_.end()) {
            // Update the value and move the key to the front of the list
            it->second->second = value;
            items_.splice(items_.begin(), items_, it->second);
        } else {
            if (cache_.size() >= capacity_) {
                // Remove the least recently used entry
                auto last = items_.back();
                cache_.erase(last.first);
                items_.pop_back();
            }
            // Add the new entry
            items_.push_front({key, value});
            cache_[key] = items_.begin();
        }
    }

    void remove(const K& key) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = cache_.find(key);
        if (it != cache_.end()) {
            items_.erase(it->second);
            cache_.erase(key);
        }
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return cache_.size();
    }

    bool contains(const K& key) const {
        std::lock_guard<std::mutex> lock(mutex_);
        return cache_.count(key);
    }
};

int main() {
    ConcurrentLRUCache<int, std::string> cache(3);
    std::cout << cache.get(3).value_or("Not Found") << std::endl;
    cache.put(1, "One");
    cache.put(2, "Two");
    cache.put(3, "Three");
    std::cout << cache.get(1).value_or("Not found")
              << std::endl;  // Output: One
    cache.put(4, "Four");
    std::cout << cache.get(2).value_or("Not found")
              << std::endl;  // Output: Not found (evicted)
    return 0;
}