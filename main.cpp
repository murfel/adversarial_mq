#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
#include <set>
#include <fstream>

#include "multiqueue.h"
#include "dijkstra.h"
#include "read_input.h"

template <class T>
void print(const std::vector<T> & v, std::ostream& ostream) {
    for (const T &elem: v) {
        ostream << elem << " ";
    } ostream << std::endl;
}

void print_python(const std::vector<int> & v, const std::string &name) {
    std::cout << name << " = [";
    for (int elem: v) {
        std::cout << elem << ", ";
    } std::cout << "]" << std::endl;
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

    multiqueue<int, PQ> mq(pqs);
    mq.shuffle(shuffle_style, num_shuffle_rounds);

    std::vector<int> returned_elements;
    returned_elements.reserve(n);
    for (int i = 0; i < n; i++) {
        returned_elements.push_back(mq.pop());
    }

    auto rank_errors = calc_rank_errors(returned_elements);
    print(rank_errors, std::cout);
}

template <class PQ>
void benchmark_online(int m, int init_sz, double push_fraction, double zero_pq_push_fraction,
        const std::vector<int> &zero_pq_elements, const std::vector<int> &elements, bool shuffle,
        const std::string &name, std::ostream& ostream) {
    int n = (int)elements.size();
    multiqueue<int, PQ> mq(m);

    std::vector<int> ranks;
    ranks.reserve(n);

    std::mt19937 mt{ 0 };
    std::uniform_real_distribution<double> dist;
    auto dice = [&mt, &dist] { return dist(mt); };

    auto it = elements.begin();

    bool separate_zero_pq_elements = !zero_pq_elements.empty();
    auto it0 = zero_pq_elements.begin();

    std::uniform_int_distribution<int> choose_pq_dist(1, m - 1);  // []
    auto choose_pq_dice = [&mt, &choose_pq_dist] { return choose_pq_dist(mt); };

    int push_cnt = 0;
    int pop_cnt = 0;

    bool empty_pq_warning_given = false;

    while (true) {
        if (push_cnt < init_sz || dice() < push_fraction) {
            if (it == elements.end() || (separate_zero_pq_elements && it0 == zero_pq_elements.end())) {
                break;
            }
            if (dice() < zero_pq_push_fraction) {
                int element = separate_zero_pq_elements ? *(it0++) : *(it++);
                mq.push(element, 0);
            } else {
                mq.push(*(it++), choose_pq_dice());
            }
            push_cnt++;
        } else {
            if (mq.has_empty_pq() && !empty_pq_warning_given) {
                std::cout << name << ": empty pq detected at pop id: " << pop_cnt << std::endl;
                empty_pq_warning_given = true;
            }
            auto p = mq.pop(shuffle);
            ranks.push_back(p.second);
            pop_cnt++;
        }
    }
//    print_python(ranks, name);
    print(ranks, ostream);

    auto v = mq.get_max_sizes();
//    std::cout << v[0] << " " << *std::max_element(v.begin() + 1, v.end()) << std::endl;
//    std::cout << push_cnt << " " << pop_cnt << std::endl;
}

int main() {
    if (true) {
        AdjList adj_list = read_input("NY");
        std::cerr << dijkstra<multiqueue<QueueElement, priority_queue<QueueElement>>>(adj_list, 2) << std::endl;
        std::cerr << dijkstra<multiqueue<QueueElement, priority_queue<QueueElement>>>(adj_list, 10) << std::endl;
        std::cerr << dijkstra<multiqueue<QueueElement, priority_queue_with_buffer<QueueElement, 4>>>(adj_list, 2) << std::endl;
        std::cerr << dijkstra<multiqueue<QueueElement, priority_queue_with_buffer<QueueElement, 4>>>(adj_list, 10) << std::endl;
        std::cerr << dijkstra<multiqueue<QueueElement, priority_queue_with_buffer<QueueElement, 10>>>(adj_list, 2) << std::endl;
        std::cerr << dijkstra<multiqueue<QueueElement, priority_queue_with_buffer<QueueElement, 10>>>(adj_list, 10) << std::endl;
        exit(0);
    }

    int m = 16;
    int k0 = 1'000;
    int k1 = 100;
    int n = k0 + (m - 1) * k1;
    std::vector<int> adversarial_input(n);
    std::iota(adversarial_input.begin(), adversarial_input.end(), 0);

    std::vector<int> uniform_input(n);
    std::iota(uniform_input.begin(), uniform_input.end(), 0);
    std::shuffle(uniform_input.begin(), uniform_input.end(), std::mt19937(0));

//    benchmark_online<priority_queue>(m, .9, .9, {}, uniform_input, false, "uni_noshuf");
//    benchmark_online<priority_queue>(m, .9, .9, {}, uniform_input, true, "uni_shuf");
//    benchmark_online<priority_queue_with_buffer<10>>(m, .9, .9, {}, uniform_input, true, "uni_shuf_buf_10");
//    benchmark_online<priority_queue_with_buffer<15>>(m, .9, .9, {}, uniform_input, true, "uni_shuf_buf_15");
//    benchmark_online<priority_queue_with_buffer<25>>(m, .9, .9, {}, uniform_input, true, "uni_shuf_buf_25");

    m = 32;
    int k = 5000;
    std::vector<int> e0(k);
    std::iota(e0.begin(), e0.end(), 0);
    std::vector<int> e(k * (m - 1));
    std::iota(e.begin(), e.end(), k);
    std::shuffle(e.begin(), e.end(), std::mt19937(0));

    int init_sz = 2000;
    double push_fraction = 0.5;
    double zero_pq_push_fraction = 1.0 / m;

    std::ofstream file;
    file.open("input.txt");

    benchmark_online<priority_queue<int>>(m, init_sz, push_fraction, zero_pq_push_fraction, e0, e, false, "uni_noshuf", file);
    benchmark_online<priority_queue<int>>(m, init_sz, push_fraction, zero_pq_push_fraction, e0, e, true, "uni_shuf", file);
    benchmark_online<priority_queue_with_buffer<int, 4>>(m, init_sz, push_fraction, zero_pq_push_fraction, e0, e, true, "uni_shuf_buf_4", file);
    benchmark_online<priority_queue_with_buffer<int, 8>>(m, init_sz, push_fraction, zero_pq_push_fraction, e0, e, true, "uni_shuf_buf_8", file);
    benchmark_online<priority_queue_with_buffer<int, 10>>(m, init_sz, push_fraction, zero_pq_push_fraction, e0, e, true, "uni_shuf_buf_10", file);

    file.close();

//    std::cout << "uniform input, no shuffling" << std::endl;
//    std::cout << "ys1 = [";
//    benchmark<priority_queue>(m, k0, k1, uniform_input, none);
//    std::cout << "]" << std::endl;
//    std::cout << "adversarial input, no shuffling" << std::endl;
//    std::cout << "ys2 = [";
//    benchmark<priority_queue>(m, k0, k1, adversarial_input, none);
//    std::cout << "]" << std::endl;
//    std::cout << "adversarial input, tree shuffling, logM" << std::endl;
//    benchmark<priority_queue>(m, k0, k1, adversarial_input, tree_style, logM);
//    std::cout << "adversarial input, tree shuffling, logN" << std::endl;
//    std::cout << "ys3 = [";
//    benchmark<priority_queue>(m, k0, k1, adversarial_input, tree_style, logN);
//    std::cout << "]" << std::endl;
//    std::cout << "adversarial input, perm shuffling, logM" << std::endl;
//    benchmark<priority_queue>(m, k0, k1, adversarial_input, permutation_style, logM);
//    std::cout << "adversarial input, perm shuffling, logN" << std::endl;
//    std::cout << "ys4 = [";
//    benchmark<priority_queue>(m, k0, k1, adversarial_input, permutation_style, logN);
//    std::cout << "]" << std::endl;
//    int k = n / m;
//    std::cout << "ys5 = [";
//    benchmark<priority_queue>(m, k, k, uniform_input, none);
//    std::cout << "]" << std::endl;
//    std::cout << "delta-array, adversarial input, tree shuffling, logN" << std::endl;
//    benchmark<priority_queue_with_buffer>(m, k0, k1, adversarial_input, tree_style, logN);
    return 0;
}
