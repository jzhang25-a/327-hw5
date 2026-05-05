#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <utility>

struct MarketData {
    std::string symbol;
    double price;
    int volume;
};

class RobinHoodHashTable {
private:
    enum class State {
        EMPTY,
        OCCUPIED,
        DELETED
    };

    struct Bucket {
        std::string key;
        MarketData value;
        std::size_t hash = 0;
        std::size_t probe_distance = 0;
        State state = State::EMPTY;
    };

    std::vector<Bucket> table_;
    std::size_t size_;
    double max_load_factor_;

private:
    static std::uint64_t hash_symbol(const std::string& key) {
        std::uint64_t hash = 14695981039346656037ull;

        for (unsigned char c : key) {
            if (c >= 'a' && c <= 'z') {
                c -= 32;
            }

            hash ^= c;
            hash *= 1099511628211ull;
        }

        hash ^= hash >> 33;
        hash *= 0xff51afd7ed558ccdULL;
        hash ^= hash >> 33;
        hash *= 0xc4ceb9fe1a85ec53ULL;
        hash ^= hash >> 33;

        return hash;
    }

    std::size_t index_for_hash(std::size_t hash) const {
        return hash % table_.size();
    }

    void maybe_rehash() {
        double load = static_cast<double>(size_ + 1) / table_.size();

        if (load > max_load_factor_) {
            rehash(table_.size() * 2);
        }
    }

    void rehash(std::size_t new_capacity) {
        std::vector<Bucket> old_table = std::move(table_);

        table_.clear();
        table_.resize(new_capacity);
        size_ = 0;

        for (const Bucket& bucket : old_table) {
            if (bucket.state == State::OCCUPIED) {
                insert(bucket.key, bucket.value);
            }
        }
    }

public:
    explicit RobinHoodHashTable(std::size_t capacity = 1024)
        : size_(0), max_load_factor_(0.70) {
        if (capacity < 8) {
            capacity = 8;
        }

        table_.resize(capacity);
    }

    std::size_t size() const {
        return size_;
    }

    std::size_t capacity() const {
        return table_.size();
    }

    void insert(const std::string& key, const MarketData& value) {
        maybe_rehash();

        std::size_t hash = static_cast<std::size_t>(hash_symbol(key));
        std::size_t index = index_for_hash(hash);

        Bucket incoming;
        incoming.key = key;
        incoming.value = value;
        incoming.hash = hash;
        incoming.probe_distance = 0;
        incoming.state = State::OCCUPIED;

        while (true) {
            Bucket& current = table_[index];

            if (current.state == State::EMPTY || current.state == State::DELETED) {
                current = std::move(incoming);
                ++size_;
                return;
            }

            if (current.hash == hash && current.key == key) {
                current.value = value;
                return;
            }

            if (current.probe_distance < incoming.probe_distance) {
                std::swap(current, incoming);
            }

            index = (index + 1) % table_.size();
            ++incoming.probe_distance;
        }
    }

    MarketData* lookup(const std::string& key) {
        std::size_t hash = static_cast<std::size_t>(hash_symbol(key));
        std::size_t index = index_for_hash(hash);
        std::size_t distance = 0;

        while (true) {
            Bucket& current = table_[index];

            if (current.state == State::EMPTY) {
                return nullptr;
            }

            if (
                current.state == State::OCCUPIED &&
                current.hash == hash &&
                current.key == key
            ) {
                return &current.value;
            }

            if (
                current.state == State::OCCUPIED &&
                current.probe_distance < distance
            ) {
                return nullptr;
            }

            index = (index + 1) % table_.size();
            ++distance;
        }
    }

    bool erase(const std::string& key) {
        std::size_t hash = static_cast<std::size_t>(hash_symbol(key));
        std::size_t index = index_for_hash(hash);
        std::size_t distance = 0;

        while (true) {
            Bucket& current = table_[index];

            if (current.state == State::EMPTY) {
                return false;
            }

            if (
                current.state == State::OCCUPIED &&
                current.hash == hash &&
                current.key == key
            ) {
                current.state = State::DELETED;
                current.key.clear();
                --size_;
                return true;
            }

            if (
                current.state == State::OCCUPIED &&
                current.probe_distance < distance
            ) {
                return false;
            }

            index = (index + 1) % table_.size();
            ++distance;
        }
    }
};