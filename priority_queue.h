#ifndef ADVERSARIAL_MQ_PRIORITY_QUEUE_H
#define ADVERSARIAL_MQ_PRIORITY_QUEUE_H


#include <utility>
#include <vector>
#include <queue>
#include <random>

#include "utils.h"


class priority_queue {
private:
    std::vector<int> heap;

    void make_heap() {
        std::make_heap(heap.begin(), heap.end(), std::greater<>());
    }
public:
    explicit priority_queue(std::vector<int> elements) : heap(std::move(elements)) {
        make_heap();
    }
    bool empty() const {
        return heap.empty();
    }
    int top() const {
        return heap[0];
    }
    int pop() {
        auto element = heap[0];
        std::pop_heap(heap.begin(), heap.end(), std::greater<>());
        heap.resize(heap.size() - 1);
        return element;
    }
    std::size_t size() const {
        return heap.size();
    }
    friend void shuffle(priority_queue & pq1, priority_queue & pq2);
    friend void print(const priority_queue & pq) {
        std::cout << pq.heap.size() << ": ";
        for (int element: pq.heap) {
            std::cout << element << " ";
        } std::cout << std::endl;
    }
};

void shuffle(priority_queue &pq1, priority_queue &pq2) {
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


#endif //ADVERSARIAL_MQ_PRIORITY_QUEUE_H
