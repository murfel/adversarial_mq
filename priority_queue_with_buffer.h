#ifndef ADVERSARIAL_MQ_PRIORITY_QUEUE_WITH_BUFFER_H
#define ADVERSARIAL_MQ_PRIORITY_QUEUE_WITH_BUFFER_H

#include <utility>
#include <vector>
#include <queue>
#include <deque>
#include <random>
#include <algorithm>

#include "utils.h"

class priority_queue_with_buffer {
private:
    const int delta = 10;
    std::deque<int> sorted_array;
    std::vector<int> heap;

    void equalize() {
        while (sorted_array.size() > delta) {
            heap.push_back(sorted_array.back());
            sorted_array.pop_back();
            std::push_heap(heap.begin(), heap.end(), std::greater<>());
        }
        auto middle = sorted_array.end();
        for (int i = sorted_array.size(); i < delta; i++) {
            if (heap.empty()) {
                return;
            }
            sorted_array.push_back(heap.front());
            std::pop_heap(heap.begin(), heap.end(), std::greater<>());
        }
        std::inplace_merge(sorted_array.begin(), middle, sorted_array.end());
    }
public:
    explicit priority_queue_with_buffer(std::vector<int> & elements) :
            sorted_array(elements.begin(), elements.begin() + delta),
            heap(elements.begin() + delta, elements.end()) {
        std::sort(sorted_array.begin(), sorted_array.end());
        std::make_heap(heap.begin(), heap.end(), std::greater<>());
    }
    bool empty() {
        return sorted_array.empty();
    }
    int peek() {
        return heap[0];
    }
    int delete_min() {
        if (heap.empty()) {
            auto array_top = sorted_array.front();
            sorted_array.pop_front();
            return array_top;
        } else {
            auto heap_top = heap[0];
            std::pop_heap(heap.begin(), heap.end(), std::greater<>());
            heap.resize(heap.size() - 1);
            auto array_top = sorted_array.front();
            sorted_array.pop_front();
            sorted_array.push_back(heap_top);
            return array_top;
        }
    }
    friend void shuffle(priority_queue_with_buffer & pq1, priority_queue_with_buffer & pq2);
    friend void print(const priority_queue_with_buffer & pq) {
        std::cout << pq.sorted_array.size() << ": ";
        for (int element: pq.sorted_array) {
            std::cout << element << " ";
        } std::cout << "|\t";
        std::cout << pq.heap.size() << ": ";
        for (int element: pq.heap) {
            std::cout << element << " ";
        } std::cout << std::endl;
    }
};

void shuffle(priority_queue_with_buffer &pq1, priority_queue_with_buffer &pq2) {
    std::vector<int> merged_arrays(pq1.delta + pq2.delta);
    std::merge(pq1.sorted_array.begin(), pq1.sorted_array.end(),
               pq2.sorted_array.begin(), pq2.sorted_array.end(),
               merged_arrays.begin());
    pq1.sorted_array.clear();
    pq2.sorted_array.clear();
    std::for_each(merged_arrays.begin(), merged_arrays.end(), [&pq1, &pq2](int x) {
        if (rand_bit()) {
            pq1.sorted_array.push_back(x);
        } else {
            pq2.sorted_array.push_back(x);
        }
    });
    pq1.equalize();
    pq2.equalize();
}

#endif //ADVERSARIAL_MQ_PRIORITY_QUEUE_WITH_BUFFER_H
