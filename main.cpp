#include <iostream>
#include "multiqueue.h"

void benchmark(int m, int k0, int k1) {
    int n = k0 + (m - 1) * k1;
    std::vector<priority_queue> pqs;
    pqs.reserve(m);
    std::vector<int> elements0(k0);
    std::iota(elements0.begin(), elements0.end(), 0);
    pqs.emplace_back(elements0);

    for (int i = 1; i < m; i++) {
        std::vector<int> elements1(k1);
        std::iota(elements1.begin(), elements1.end(), k0 + (i - 1) * k1);
        pqs.emplace_back(elements1);
    }

    multiqueue mq(pqs);
    mq.shuffle();
    print(mq);

    std::vector<int> returned_elements;
    returned_elements.reserve(n);
    for (int i = 0; i < n; i++) {
        returned_elements.push_back(mq.delete_min());
    }
    for (int element: returned_elements) {
        std::cout << element << " ";
    } std::cout << std::endl;
    // TODO: count the number of inversions for each returned_elements array prefix
}


int main() {
    benchmark(10, 10'000, 100);
    return 0;
}
