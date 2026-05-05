#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "robin_hood_hash_table.hpp"

using Clock = std::chrono::high_resolution_clock;

template <typename Function>
double time_ms(Function&& function) {
    auto start = Clock::now();
    function();
    auto end = Clock::now();

    return std::chrono::duration<double, std::milli>(end - start).count();
}

std::vector<std::string> generate_symbols(std::size_t n) {
    std::vector<std::string> symbols;
    symbols.reserve(n);

    for (std::size_t i = 0; i < n; ++i) {
        symbols.push_back("SYM" + std::to_string(i));
    }

    return symbols;
}

int main() {
    const std::size_t n = 200000;

    std::vector<std::string> symbols = generate_symbols(n);

    RobinHoodHashTable robin_table(n * 2);

    std::unordered_map<std::string, MarketData> unordered_map;
    unordered_map.reserve(n * 2);

    double robin_insert_time = time_ms([&]() {
        for (std::size_t i = 0; i < n; ++i) {
            MarketData data{
                symbols[i],
                100.0 + static_cast<double>(i % 100),
                static_cast<int>(i)
            };

            robin_table.insert(symbols[i], data);
        }
    });

    double unordered_insert_time = time_ms([&]() {
        for (std::size_t i = 0; i < n; ++i) {
            MarketData data{
                symbols[i],
                100.0 + static_cast<double>(i % 100),
                static_cast<int>(i)
            };

            unordered_map[symbols[i]] = data;
        }
    });

    volatile double checksum = 0.0;

    double robin_lookup_time = time_ms([&]() {
        for (std::size_t i = 0; i < n; ++i) {
            MarketData* result = robin_table.lookup(symbols[i]);

            if (result != nullptr) {
                checksum += result->price;
            }
        }
    });

    double unordered_lookup_time = time_ms([&]() {
        for (std::size_t i = 0; i < n; ++i) {
            auto it = unordered_map.find(symbols[i]);

            if (it != unordered_map.end()) {
                checksum += it->second.price;
            }
        }
    });

    double robin_delete_time = time_ms([&]() {
        for (std::size_t i = 0; i < n; i += 2) {
            robin_table.erase(symbols[i]);
        }
    });

    double unordered_delete_time = time_ms([&]() {
        for (std::size_t i = 0; i < n; i += 2) {
            unordered_map.erase(symbols[i]);
        }
    });

    std::cout << "\nPart 1: Hash Table Benchmark\n";
    std::cout << "Number of symbols: " << n << "\n\n";

    std::cout << std::left
              << std::setw(28) << "Structure"
              << std::setw(18) << "Insert ms"
              << std::setw(18) << "Lookup ms"
              << std::setw(18) << "Delete ms"
              << "\n";

    std::cout << std::left
              << std::setw(28) << "RobinHoodHashTable"
              << std::setw(18) << robin_insert_time
              << std::setw(18) << robin_lookup_time
              << std::setw(18) << robin_delete_time
              << "\n";

    std::cout << std::left
              << std::setw(28) << "std::unordered_map"
              << std::setw(18) << unordered_insert_time
              << std::setw(18) << unordered_lookup_time
              << std::setw(18) << unordered_delete_time
              << "\n";

    std::cout << "\nChecksum: " << checksum << "\n";

    return 0;
}