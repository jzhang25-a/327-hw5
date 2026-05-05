# Part 1: Optimized Hash Table for Market Data

## Overview

This section implements a Robin Hood hashing-based hash table for market data. The table stores `MarketData` objects keyed by stock symbol.

Each record contains:

- Symbol
- Price
- Volume

The implementation supports:

- Insert
- Lookup
- Delete

## Hash Function

The custom hash function is based on FNV-1a and normalizes lowercase characters to uppercase.

This is useful for financial symbols because symbols are usually short strings such as:

- AAPL
- MSFT
- TSLA
- ESZ6

The hash function also applies a final avalanche step to improve distribution.

## Robin Hood Hashing

Robin Hood hashing is an open-addressing collision resolution strategy.

The main idea is:

> If a new key has probed farther from its original bucket than the key currently occupying a bucket, the new key steals the bucket.

This reduces variance in probe lengths.

## Complexity

| Operation | Average Case | Worst Case |
|---|---:|---:|
| Insert | O(1) | O(n) |
| Lookup | O(1) | O(n) |
| Delete | O(1) | O(n) |

## Benchmark Results

| Structure | Insert ms | Lookup ms | Delete ms |
|---|---:|---:|---:|
| RobinHoodHashTable | 24.2387 | 12.9828 | 6.50979 |
| std::unordered_map | 21.1334 | 5.7474 | 12.8344 |

## Tradeoff Analysis

Robin Hood hashing improves lookup stability by reducing the variance of probe distances. This can be useful in latency-sensitive market data systems where predictable lookup time matters.

Compared with std::unordered_map, this implementation may have better cache locality because buckets are stored contiguously in a vector. However, std::unordered_map is more general, heavily optimized, and easier to use safely.

The custom implementation also has limitations:

- It does not use custom allocators.
- It stores strings directly inside buckets.
- Delete uses tombstones instead of backward-shift deletion.
- It does not shrink after many deletions.

For real HFT systems, further improvements could include:

- Fixed-size symbol keys
- Integer symbol IDs
- Cache-aligned buckets
- Custom memory pools
- Backward-shift deletion instead of tombstones