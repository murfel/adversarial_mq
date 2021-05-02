#ifndef ADVERSARIAL_MQ_MULTIQUEUE_H
#define ADVERSARIAL_MQ_MULTIQUEUE_H

#include <random>

#include "priority_queue_t.h"
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
    int gen_random_index();

    std::size_t calc_size();
    void shuffle_tree_style(num_shuffle_rounds_t num_shuffle_rounds);
    void shuffle_permutation_style(num_shuffle_rounds_t num_shuffle_rounds);
public:
    explicit multiqueue(std::vector<PQ> priority_queues) :
            priority_queues(std::move(priority_queues)),
            dist(std::uniform_int_distribution<int>(0, this->priority_queues.size() - 1)) {}
    int pop();
    void shuffle(shuffle_style_t shuffle_style = permutation_style, num_shuffle_rounds_t num_shuffle_rounds = logN);
    friend void print(const multiqueue & mq);
};

#endif //ADVERSARIAL_MQ_MULTIQUEUE_H
