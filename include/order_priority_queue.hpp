#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>
#include <utility>

struct Order {
    int id;
    double price;
    int quantity;
    int timestamp;

    bool higher_priority_than(const Order& other) const {
        if (price != other.price) {
            return price > other.price;
        }

        return timestamp < other.timestamp;
    }
};

class OrderMaxHeap {
private:
    std::vector<Order> heap_;

private:
    void sift_up(std::size_t index) {
        while (index > 0) {
            std::size_t parent = (index - 1) / 2;

            if (!heap_[index].higher_priority_than(heap_[parent])) {
                break;
            }

            std::swap(heap_[index], heap_[parent]);
            index = parent;
        }
    }

    void sift_down(std::size_t index) {
        while (true) {
            std::size_t left = 2 * index + 1;
            std::size_t right = 2 * index + 2;
            std::size_t best = index;

            if (
                left < heap_.size() &&
                heap_[left].higher_priority_than(heap_[best])
            ) {
                best = left;
            }

            if (
                right < heap_.size() &&
                heap_[right].higher_priority_than(heap_[best])
            ) {
                best = right;
            }

            if (best == index) {
                break;
            }

            std::swap(heap_[index], heap_[best]);
            index = best;
        }
    }

public:
    void push(const Order& order) {
        heap_.push_back(order);
        sift_up(heap_.size() - 1);
    }

    Order top() const {
        if (heap_.empty()) {
            throw std::runtime_error("Heap is empty");
        }

        return heap_.front();
    }

    Order pop() {
        if (heap_.empty()) {
            throw std::runtime_error("Heap is empty");
        }

        Order best_order = heap_.front();

        heap_[0] = heap_.back();
        heap_.pop_back();

        if (!heap_.empty()) {
            sift_down(0);
        }

        return best_order;
    }

    bool empty() const {
        return heap_.empty();
    }

    std::size_t size() const {
        return heap_.size();
    }
};