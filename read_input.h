#ifndef ADVERSARIAL_MQ_READ_INPUT_H
#define ADVERSARIAL_MQ_READ_INPUT_H

#include "dijkstra.h"

inline AdjList read_edges_into_adj_list(std::istream & istream) {
    const int VERTEX_NUMERATION_OFFSET = -1;
    std::size_t num_verticies, num_edges;
    istream >> num_verticies >> num_edges;
    AdjList adj_list(num_verticies);
    for (std::size_t i = 0; i < num_edges; i++) {
        Vertex from, to;
        DistType weight;
        istream >> from >> to >> weight;
        if (weight <= 0) continue;
        adj_list[from + VERTEX_NUMERATION_OFFSET].emplace_back(to + VERTEX_NUMERATION_OFFSET, weight);
    }
    return adj_list;
}

inline AdjList read_input(const std::string& filename) {
    std::ifstream input(filename + ".in");
    std::cerr << "Reading " << filename << ".in... ";
    auto adj_list = read_edges_into_adj_list(input);
    std::cerr << "Done." << std::endl;
    return adj_list;
}

#endif //ADVERSARIAL_MQ_READ_INPUT_H
