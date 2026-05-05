
# HW5: Optimization

This project implements optimized C++ data structures for market data and order processing.

## Part 1: Optimized Hash Table for Market Data

Implemented a Robin Hood hashing-based hash table for storing market data keyed by stock symbol.

Features:
- Insert market data by symbol
- Lookup market data by symbol
- Delete market data by symbol
- Custom hash function for short financial symbols
- Benchmark against `std::unordered_map`

## Build

```bash
mkdir -p build
cd build
cmake ..
make
```

## Run Part 1 Benchmark

```bash
./benchmark_hash_table
```

## Part 2: Priority Queue for Order Matching

Implemented a binary max-heap priority queue for order matching.

Features:
- Insert orders
- Retrieve highest-priority order
- Extract highest-priority order
- Price-time priority:
  - Higher price has higher priority
  - Earlier timestamp wins when prices tie
- Benchmark against `std::priority_queue`

## Run Part 2 Benchmark

```bash
./benchmark_priority_queue
```

