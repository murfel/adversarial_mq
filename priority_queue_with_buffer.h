#ifndef ADVERSARIAL_MQ_PRIORITY_QUEUE_WITH_BUFFER_H
#define ADVERSARIAL_MQ_PRIORITY_QUEUE_WITH_BUFFER_H

#include <utility>
#include <vector>
#include <queue>
#include <deque>
#include <random>
#include <algorithm>

#include "utils.h"

template<class T, int delta>
class priority_queue_with_buffer {
private:
    std::size_t max_size = 0;
    std::deque<T> sorted_array;
    std::vector<T> heap;

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
            heap.pop_back();
        }
        std::inplace_merge(sorted_array.begin(), middle, sorted_array.end());
    }
public:
    priority_queue_with_buffer() = default;
    explicit priority_queue_with_buffer(std::vector<T> & elements) :
            sorted_array(elements.begin(), elements.begin() + delta),
            heap(elements.begin() + delta, elements.end()) {
        std::sort(sorted_array.begin(), sorted_array.end());
        std::make_heap(heap.begin(), heap.end(), std::greater<>());
    }
    bool empty() const {
        return sorted_array.empty();
    }
    T top() const {
        assert(!empty());
        return sorted_array[0];
    }
    void push(T value) {
        sorted_array.insert(std::upper_bound(sorted_array.begin(), sorted_array.end(), value), value);
        if (sorted_array.size() > delta) {
            heap.push_back(sorted_array.back());
            std::push_heap(heap.begin(), heap.end(), std::greater<>());
            sorted_array.pop_back();
        }
        max_size = std::max(max_size, size());
    }
    T pop() {
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
    std::size_t size() const {
        return sorted_array.size() + heap.size();
    }
    std::size_t get_max_size() const {
        return max_size;
    }

    template<class T2, int inner_delta>
    friend void shuffle(priority_queue_with_buffer<T2, inner_delta> & pq1, priority_queue_with_buffer<T2, inner_delta> & pq2);

    template<class T2, int inner_delta>
    friend void print(const priority_queue_with_buffer<T2, inner_delta> & pq) {
        std::cout << pq.sorted_array.size() << ": ";
        for (const T &element: pq.sorted_array) {
            std::cout << element << " ";
        } std::cout << "|\t";
        std::cout << pq.heap.size() << ": ";
        for (const T &element: pq.heap) {
            std::cout << element << " ";
        } std::cout << std::endl;
    }
};

template <class T, int delta>
void shuffle(priority_queue_with_buffer<T, delta> &pq1, priority_queue_with_buffer<T, delta> &pq2) {
    std::vector<T> merged_arrays(pq1.sorted_array.size() + pq2.sorted_array.size());
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
