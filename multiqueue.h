#ifndef ADVERSARIAL_MQ_MULTIQUEUE_H
#define ADVERSARIAL_MQ_MULTIQUEUE_H

#include <random>
#include <cmath>

#include "priority_queue.h"
#include "priority_queue_with_buffer.h"

enum shuffle_style_t {
    none,
    tree_style,
    permutation_style
};

enum num_shuffle_rounds_t {
    logN, logM
};

template <class PQ>
class multiqueue {
private:
    std::vector<PQ> priority_queues{};
    std::mt19937 mt{ 0 };
    std::uniform_int_distribution<int> dist;
    int gen_random_index() {
        return dist(mt);
    }

    std::size_t calc_size() {
        std::size_t sz = 0;
        for (const auto & pq: priority_queues) {
            sz += pq.size();
        }
        return sz;
    }
    void shuffle_tree_style(num_shuffle_rounds_t num_shuffle_rounds) {
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
                        ::shuffle(priority_queues[b * sz + i], priority_queues[b * sz + i + sz / 2]);
                    }
                }
            }
        }
    }
    void shuffle_permutation_style(num_shuffle_rounds_t num_shuffle_rounds) {
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
                ::shuffle(priority_queues[indexes[i]], priority_queues[indexes[i + 1]]);
            }
        }
    }
public:
    explicit multiqueue(std::vector<PQ> priority_queues) :
            priority_queues(std::move(priority_queues)),
            dist(std::uniform_int_distribution<int>(0, this->priority_queues.size() - 1)) {}
    int pop() {
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
    void shuffle(shuffle_style_t shuffle_style = permutation_style, num_shuffle_rounds_t num_shuffle_rounds = logN) {
        if (shuffle_style == tree_style) {
            shuffle_tree_style(num_shuffle_rounds);
        } else if (shuffle_style == permutation_style) {
            shuffle_permutation_style(num_shuffle_rounds);
        }
    }
    friend void print(const multiqueue & mq) {
        for (const auto & pq: mq.priority_queues) {
            print(pq);
        }
    }
};

#endif //ADVERSARIAL_MQ_MULTIQUEUE_H
