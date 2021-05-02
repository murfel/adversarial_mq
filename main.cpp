#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

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

std::vector<int> calc_rank_errors(const std::vector<int> & returned_elements) {
    std::vector<int> sorted_elements(returned_elements.size());
    std::copy(returned_elements.begin(), returned_elements.end(), sorted_elements.begin());
    std::sort(sorted_elements.begin(), sorted_elements.end());
    std::vector<int> rank_errors;
    rank_errors.reserve(returned_elements.size());
    for (int ret_element: returned_elements) {
        auto it = std::find(sorted_elements.begin(), sorted_elements.end(), ret_element);
        rank_errors.push_back(std::distance(sorted_elements.begin(), it));
        sorted_elements.erase(it);
    }
    return rank_errors;
}

template <class PQ>
void benchmark(int m, int k0, int k1, const std::vector<int> & elements, shuffle_style_t shuffle_style,
               num_shuffle_rounds_t num_shuffle_rounds = logN) {
    int n = k0 + (m - 1) * k1;
    std::vector<PQ> pqs;
    pqs.reserve(m);
    std::vector<int> elements0(k0);
    std::copy(elements.begin(), elements.begin() + k0, elements0.begin());
    pqs.emplace_back(elements0);

    for (int i = 1; i < m; i++) {
        std::vector<int> elements1(k1);
        auto begin = elements.begin() + k0 + (i - 1) * k1;
        std::copy(begin, begin + k1, elements1.begin());
        pqs.emplace_back(elements1);
    }

    multiqueue<PQ> mq(pqs);
    mq.shuffle(shuffle_style, num_shuffle_rounds);
//    print(mq);

    std::vector<int> returned_elements;
    returned_elements.reserve(n);
    for (int i = 0; i < n; i++) {
        returned_elements.push_back(mq.delete_min());
    }
//    print(returned_elements);

    auto rank_errors = calc_rank_errors(returned_elements);
    print(rank_errors);

    auto inversions = count_inversions(returned_elements);
//    print(inversions);
}


int main() {
    int m = 16;
    int k0 = 1'000;
    int k1 = 100;
    int n = k0 + (m - 1) * k1;
    std::vector<int> adversarial_input(n);
    std::iota(adversarial_input.begin(), adversarial_input.end(), 0);

    std::vector<int> uniform_input(n);
    std::iota(uniform_input.begin(), uniform_input.end(), 0);
    std::shuffle(uniform_input.begin(), uniform_input.end(), std::mt19937(0));

    std::cout << "uniform input, no shuffling" << std::endl;
    std::cout << "ys1 = [";
    benchmark<priority_queue>(m, k0, k1, uniform_input, none);
    std::cout << "]" << std::endl;
    std::cout << "adversarial input, no shuffling" << std::endl;
    std::cout << "ys2 = [";
    benchmark<priority_queue>(m, k0, k1, adversarial_input, none);
    std::cout << "]" << std::endl;
    std::cout << "adversarial input, tree shuffling, logM" << std::endl;
    benchmark<priority_queue>(m, k0, k1, adversarial_input, tree_style, logM);
    std::cout << "adversarial input, tree shuffling, logN" << std::endl;
    std::cout << "ys3 = [";
    benchmark<priority_queue>(m, k0, k1, adversarial_input, tree_style, logN);
    std::cout << "]" << std::endl;
    std::cout << "adversarial input, perm shuffling, logM" << std::endl;
    benchmark<priority_queue>(m, k0, k1, adversarial_input, permutation_style, logM);
    std::cout << "adversarial input, perm shuffling, logN" << std::endl;
    std::cout << "ys4 = [";
    benchmark<priority_queue>(m, k0, k1, adversarial_input, permutation_style, logN);
    std::cout << "]" << std::endl;
    int k = n / m;
    std::cout << "ys5 = [";
    benchmark<priority_queue>(m, k, k, uniform_input, none);
    std::cout << "]" << std::endl;
    std::cout << "delta-array, adversarial input, tree shuffling, logN" << std::endl;
    benchmark<priority_queue_with_buffer>(m, k0, k1, adversarial_input, tree_style, logN);
    return 0;
}
