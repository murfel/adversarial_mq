#ifndef ADVERSARIAL_MQ_PRIORITY_QUEUE_H
#define ADVERSARIAL_MQ_PRIORITY_QUEUE_H


#include <utility>
#include <vector>
#include <queue>
#include <random>

#include "utils.h"


template<class T>
class priority_queue {
private:
    std::size_t max_size = 0;
    std::vector<T> heap;

    void make_heap() {
        std::make_heap(heap.begin(), heap.end(), std::greater<>());
    }
public:
    priority_queue() = default;
    explicit priority_queue(std::vector<T> elements) : heap(std::move(elements)) {
        make_heap();
    }
    bool empty() const {
        return heap.empty();
    }
    T top() const {
        return heap[0];
    }
    void push(T value) {
        heap.push_back(value);
        std::push_heap(heap.begin(), heap.end(), std::greater<>());
        max_size = std::max(max_size, size());
    }
    T pop() {
        auto element = heap[0];
        std::pop_heap(heap.begin(), heap.end(), std::greater<>());
        heap.resize(heap.size() - 1);
        return element;
    }
    std::size_t size() const {
        return heap.size();
    }
    std::size_t get_max_size() const {
        return max_size;
    }

    template<class T2>
    friend void shuffle(priority_queue<T2> & pq1, priority_queue<T2> & pq2);

    template<class T2>
    friend void print(const priority_queue<T2> & pq) {
        std::cout << pq.heap.size() << ": ";
        for (int element: pq.heap) {
            std::cout << element << " ";
        } std::cout << std::endl;
    }
};

template<class T>
void shuffle(priority_queue<T> &pq1, priority_queue<T> &pq2) {
    std::vector<T> heap1;
    std::vector<T> heap2;
    std::size_t reserve_size = (pq1.heap.size() + pq2.heap.size()) / 2;
    heap1.reserve(reserve_size);
    heap2.reserve(reserve_size);
    for (T &element: pq1.heap) {
        if (rand_bit()) {
            heap1.push_back(element);
        } else {
            heap2.push_back(element);
        }
    }
    for (T &element: pq2.heap) {
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
