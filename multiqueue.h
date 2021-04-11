#ifndef ADVERSARIAL_MQ_MULTIQUEUE_H
#define ADVERSARIAL_MQ_MULTIQUEUE_H

#include <random>
#include <cmath>

#include "priority_queue.h"

class multiqueue {
private:
    std::vector<priority_queue> priority_queues{};
    std::mt19937 mt{ 0 };
    std::uniform_int_distribution<int> dist;
    int gen_random_index() {
        return dist(mt);
    }
public:
    explicit multiqueue(std::vector<priority_queue> priority_queues) :
            priority_queues(std::move(priority_queues)),
            dist(std::uniform_int_distribution<int>(0, this->priority_queues.size() - 1)) {}
    int delete_min() {
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

            if (pq1.peek() < pq2.peek()) {
                return pq1.delete_min();
            } else {
                return pq2.delete_min();
            }
        }
        // if we're here, probably only one non-empty queue is left
        for (auto & pq: priority_queues) {
            if (!pq.empty()) {
                return pq.delete_min();
            }
        }
        throw std::runtime_error("mq is empty");
    }
    void shuffle() {
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
    friend void print(const multiqueue & mq) {
        for (const priority_queue & pq: mq.priority_queues) {
            print(pq);
        }
    }
};

#endif //ADVERSARIAL_MQ_MULTIQUEUE_H
