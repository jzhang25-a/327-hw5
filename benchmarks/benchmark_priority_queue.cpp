#include <chrono>
#include <iomanip>
#include <iostream>
#include <queue>
#include <random>
#include <vector>

#include "order_priority_queue.hpp"

using Clock = std::chrono::high_resolution_clock;

template <typename Function>
double time_ms(Function&& function) {
    auto start = Clock::now();
    function();
    auto end = Clock::now();

    return std::chrono::duration<double, std::milli>(end - start).count();
}

struct StdPriorityQueueComparator {
    bool operator()(const Order& a, const Order& b) const {
        if (a.price != b.price) {
            return a.price < b.price;
        }

        return a.timestamp > b.timestamp;
    }
};

std::vector<Order> generate_orders(std::size_t n) {
    std::vector<Order> orders;
    orders.reserve(n);

    std::mt19937 rng(42);
    std::uniform_real_distribution<double> price_distribution(90.0, 110.0);
    std::uniform_int_distribution<int> quantity_distribution(1, 1000);

    for (std::size_t i = 0; i < n; ++i) {
        Order order;
        order.id = static_cast<int>(i);
        order.price = price_distribution(rng);
        order.quantity = quantity_distribution(rng);
        order.timestamp = static_cast<int>(i);

        orders.push_back(order);
    }

    return orders;
}

int main() {
    const std::size_t n = 200000;

    std::vector<Order> orders = generate_orders(n);

    OrderMaxHeap custom_heap;
    std::priority_queue<
        Order,
        std::vector<Order>,
        StdPriorityQueueComparator
    > std_heap;

    double custom_push_time = time_ms([&]() {
        for (const Order& order : orders) {
            custom_heap.push(order);
        }
    });

    double std_push_time = time_ms([&]() {
        for (const Order& order : orders) {
            std_heap.push(order);
        }
    });

    volatile double checksum = 0.0;

    double custom_pop_time = time_ms([&]() {
        while (!custom_heap.empty()) {
            Order order = custom_heap.pop();
            checksum += order.price;
        }
    });

    double std_pop_time = time_ms([&]() {
        while (!std_heap.empty()) {
            Order order = std_heap.top();
            std_heap.pop();
            checksum += order.price;
        }
    });

    std::cout << "\nPart 2: Priority Queue Benchmark\n";
    std::cout << "Number of orders: " << n << "\n\n";

    std::cout << std::left
              << std::setw(28) << "Structure"
              << std::setw(18) << "Push ms"
              << std::setw(18) << "Pop ms"
              << std::setw(18) << "Total ms"
              << "\n";

    std::cout << std::left
              << std::setw(28) << "Custom OrderMaxHeap"
              << std::setw(18) << custom_push_time
              << std::setw(18) << custom_pop_time
              << std::setw(18) << custom_push_time + custom_pop_time
              << "\n";

    std::cout << std::left
              << std::setw(28) << "std::priority_queue"
              << std::setw(18) << std_push_time
              << std::setw(18) << std_pop_time
              << std::setw(18) << std_push_time + std_pop_time
              << "\n";

    std::cout << "\nChecksum: " << checksum << "\n";

    return 0;
}