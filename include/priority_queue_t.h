#ifndef ADVERSARIAL_MQ_PRIORITY_QUEUE_T_H
#define ADVERSARIAL_MQ_PRIORITY_QUEUE_T_H

#include <vector>
#include <utility>
#include <cstddef>

class priority_queue_t {
private:
    std::vector<int> heap;

    void make_heap();
public:
    explicit priority_queue_t(std::vector<int> elements);
    bool empty() const;
    int top() const;
    int pop();
    std::size_t size() const;
    friend void shuffle(priority_queue_t & pq1, priority_queue_t & pq2);
    friend void print(const priority_queue_t & pq);
};


#endif //ADVERSARIAL_MQ_PRIORITY_QUEUE_T_H
