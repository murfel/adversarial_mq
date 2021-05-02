#include "../include/multiqueue.h"

#include <random>
#include <cmath>

template <class PQ>
int multiqueue<PQ>::gen_random_index() {
    return dist(mt);
}

template <class PQ>
std::size_t multiqueue<PQ>::calc_size() {
    std::size_t sz = 0;
    for (const auto & pq: priority_queues) {
        sz += pq.size();
    }
    return sz;
}

template <class PQ>
void multiqueue<PQ>::shuffle_tree_style(num_shuffle_rounds_t num_shuffle_rounds) {
    int iterations;
    if (num_shuffle_rounds == logM) {
        iterations = 1;
    } else {
        double logN = log(calc_size());
        double logM = log(priority_queues.size());
        iterations = ceil(logN / logM);
    }
    for (int iteration = 0; iteration < iterations; iteration++) {
        for (int sz = 2; sz < priority_queues.size() * 2; sz *= 2) {
            int num_blocks = (int)priority_queues.size() / sz + (priority_queues.size() % sz != 0);
            for (int b = 0; b < num_blocks; b++) {
                // block [b, b+sz)
                for (int i = 0; i < sz / 2; i++) {
                    int i1 = b * sz + i;
                    int i2 = i1 + sz / 2;
                    if (i1 >= priority_queues.size() || i2 >= priority_queues.size()) {
                        break;
                    }
                    shuffle(priority_queues[b * sz + i], priority_queues[b * sz + i + sz / 2]);
                }
            }
        }
    }
}

template <class PQ>
void multiqueue<PQ>::shuffle_permutation_style(num_shuffle_rounds_t num_shuffle_rounds) {
    int iterations;
    if (num_shuffle_rounds == logN) {
        iterations = ceil(log(calc_size()));
    } else if (num_shuffle_rounds == logM) {
        iterations = ceil(log(priority_queues.size()));
    }
    std::vector<int> indexes(priority_queues.size());
    std::iota(indexes.begin(), indexes.end(), 0);
    for (int iteration = 0; iteration < iterations; iteration++) {
        std::shuffle(indexes.begin(), indexes.end(), std::mt19937(0));
        for (int i = 0; i < indexes.size(); i += 2) {
            shuffle(priority_queues[indexes[i]], priority_queues[indexes[i + 1]]);
        }
    }
}

template <class PQ>
int multiqueue<PQ>::pop() {
    for (int it = 0; it < 1000; it++) {
        int i, j;
        i = gen_random_index();
        do {
            j = gen_random_index();
        } while (i == j);
        auto &pq1 = priority_queues[std::min(i, j)];
        auto &pq2 = priority_queues[std::max(i, j)];

        if (pq1.empty() || pq2.empty()) {
            continue;
        }

        if (pq1.top() <pq2.top()) {
            return pq1.pop();
        } else {
            return pq2.pop();
        }
    }
    // if we're here, probably only one non-empty queue is left
    for (auto & pq: priority_queues) {
        if (!pq.empty()) {
            return pq.pop();
        }
    }
    throw std::runtime_error("mq is empty");
}

template <class PQ>
void multiqueue<PQ>::shuffle(shuffle_style_t shuffle_style, num_shuffle_rounds_t num_shuffle_rounds) {
    if (shuffle_style == tree_style) {
        shuffle_tree_style(num_shuffle_rounds);
    } else if (shuffle_style == permutation_style) {
        shuffle_permutation_style(num_shuffle_rounds);
    }
}

template <class PQ>
void print(const multiqueue<PQ> & mq) {
    for (const auto & pq: mq.priority_queues) {
        print(pq);
    }
}
