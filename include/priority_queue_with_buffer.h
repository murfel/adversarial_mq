#ifndef ADVERSARIAL_MQ_PRIORITY_QUEUE_WITH_BUFFER_H
#define ADVERSARIAL_MQ_PRIORITY_QUEUE_WITH_BUFFER_H

#include <cstddef>
#include <deque>
#include <vector>

class priority_queue_with_buffer {
private:
    const int delta = 10;
    std::deque<int> sorted_array;
    std::vector<int> heap;

    void equalize();
public:
    explicit priority_queue_with_buffer(std::vector<int> & elements);
    bool empty() const;
    int top() const;
    int pop();
    std::size_t size() const;
    friend void shuffle(priority_queue_with_buffer & pq1, priority_queue_with_buffer & pq2);
    friend void print(const priority_queue_with_buffer & pq);
};

#endif //ADVERSARIAL_MQ_PRIORITY_QUEUE_WITH_BUFFER_H
