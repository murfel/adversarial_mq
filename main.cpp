#include <algorithm>
#include <iostream>
#include <set>

#include "multiqueue.h"

void print(const std::vector<int> & v) {
    for (int elem: v) {
        std::cout << elem << ", ";
    } std::cout << std::endl;
}

std::vector<int> count_inversions(const std::vector<int> & v) {
    std::vector<int> element_inversions;
    element_inversions.reserve(v.size());

    for (auto it = v.begin(); it != v.end(); it++) {
        element_inversions.push_back(std::count_if(v.begin(), it, [it](int x) { return x > *it; }));
    }
    std::vector<int> total_inversions(v.size());
    std::partial_sum(element_inversions.begin(), element_inversions.end(), total_inversions.begin());
    return total_inversions;
}

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
    print(returned_elements);

    std::vector<int> rank_errors;
    rank_errors.reserve(returned_elements.size());
    for (int i = 0; i < returned_elements.size(); i++) {
        rank_errors.push_back(i - returned_elements[i]);
    }
    print(rank_errors);

    auto inversions = count_inversions(returned_elements);
    print(inversions);
}


int main() {
    benchmark(10, 10'000, 100);
    return 0;
}