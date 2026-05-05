# Part 2: Priority Queue for Order Matching

## Overview

This section implements a binary heap-based priority queue for order matching.

The custom heap stores `Order` objects with:

- Order ID
- Price
- Quantity
- Timestamp

The heap uses price-time priority:

1. Higher price has higher priority.
2. If prices are equal, earlier timestamp has higher priority.

This models a simplified bid-side matching engine where the best bid should be processed first.

## Data Structure

The priority queue is implemented as a binary max-heap stored in a contiguous `std::vector`.

For any node at index `i`:

- Parent index: `(i - 1) / 2`
- Left child index: `2 * i + 1`
- Right child index: `2 * i + 2`

## Supported Operations

| Operation | Description |
|---|---|
| `push` | Insert a new order |
| `top` | Return the highest-priority order |
| `pop` | Remove and return the highest-priority order |
| `empty` | Check whether the heap is empty |
| `size` | Return number of orders |

## Complexity

| Operation | Complexity |
|---|---:|
| Insert / Push | O(log n) |
| Top | O(1) |
| Extract / Pop | O(log n) |
| Size | O(1) |
| Empty | O(1) |

## Benchmark Results

| Structure | Push ms | Pop ms | Total ms |
|---|---:|---:|---:|
| OrderMaxHeap | 11.2496 | 63.3526 | 74.6022 |
| std::priority_queue | 10.4075 | 21.4885 | 31.8959 |

The custom `OrderMaxHeap` was slower than `std::priority_queue` in the benchmark. This is expected because `std::priority_queue` is already an optimized binary heap implementation provided by the C++ standard library. Both data structures have the same asymptotic complexity, O(log n) insertion and O(log n) extraction, so performance differences are mostly due to constant factors, compiler optimizations, memory access patterns, and implementation details.

The custom implementation is still useful because it provides direct control over price-time priority logic and can be extended with exchange-specific features such as order ID indexing, custom memory pools, fixed-capacity arrays, or cancellation support.

## Trade-Off Analysis

A binary heap is a natural data structure for repeatedly retrieving the highest-priority order. It provides efficient insertion and extraction with logarithmic complexity.

The custom heap is useful because it gives full control over the priority logic, memory layout, and future extensions. For example, it could be extended to support custom allocators, fixed-capacity memory pools, or direct index tracking.

However, the custom heap has limitations compared with a full production order book:

- It does not support efficient cancellation by order ID.
- It does not support efficient modification by order ID.
- It only gives direct access to the best order, not all orders at a price level.
- It is not ideal for querying market depth.

std::priority_queue is highly optimized and should usually perform similarly or better than a simple custom heap. The main reason to implement a custom heap is control, not necessarily raw speed.

In real HFT systems, a heap alone is usually insufficient for order book management. Matching engines typically need fast cancellation, modification, and price-level queries. Therefore, a heap may be useful for some scheduling or event-priority tasks, but a full order book usually requires a combination of hash maps and ordered price-level structures.