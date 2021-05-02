#include "../include/priority_queue_t.h"
#include "../include/utils.h"

#include <iostream>

void priority_queue_t::make_heap() {
    std::make_heap(heap.begin(), heap.end(), std::greater<>());
}

priority_queue_t::priority_queue_t(std::vector<int> elements) : heap(std::move(elements)) {
    make_heap();
}

bool priority_queue_t::empty() const {
    return heap.empty();
}

int priority_queue_t::top() const {
    return heap[0];
}

int priority_queue_t::pop() {
    auto element = heap[0];
    std::pop_heap(heap.begin(), heap.end(), std::greater<>());
    heap.resize(heap.size() - 1);
    return element;
}

std::size_t priority_queue_t::size() const {
    return heap.size();
}

void shuffle(priority_queue_t& pq1, priority_queue_t& pq2) {
    std::vector<int> heap1;
    std::vector<int> heap2;
    std::size_t reserve_size = (pq1.heap.size() + pq2.heap.size()) / 2;
    heap1.reserve(reserve_size);
    heap2.reserve(reserve_size);
    for (int element: pq1.heap) {
        if (rand_bit()) {
            heap1.push_back(element);
        } else {
            heap2.push_back(element);
        }
    }
    for (int element: pq2.heap) {
        if (rand_bit()) {
            heap1.push_back(element);
        } else {
            heap2.push_back(element);
        }
    }
    pq1.heap = heap1;
    pq1.make_heap();
    pq2.heap = heap2;
    pq2.make_heap();
}

void print(const priority_queue_t& pq) {
    std::cout << pq.heap.size() << ": ";
    for (int element: pq.heap) {
        std::cout << element << " ";
    } std::cout << std::endl;
}