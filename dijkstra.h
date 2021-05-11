#ifndef ADVERSARIAL_MQ_DIJKSTRA_H
#define ADVERSARIAL_MQ_DIJKSTRA_H

using Vertex = uint32_t;
using DistType = int;
using DistVector = std::vector<DistType>;

class Edge {
private:
    Vertex to;
    DistType weight;
public:
    Edge(Vertex to, DistType weight) : to(to), weight(weight) {}
    Vertex get_to() const {
        return to;
    }
    void set_to(Vertex to) {
        this->to = to;
    }
    DistType get_weight() const {
        return weight;
    }
};

using AdjList = std::vector<std::vector<Edge>>;

class QueueElement {
private:
    Vertex vertex;
    DistType dist;
public:
    QueueElement(Vertex vertex = 0, DistType dist = -1) : vertex(vertex), dist(dist) {}
    Vertex get_vertex() const {
        return vertex;
    }
    DistType get_dist() const {
        return dist;
    }
    bool operator==(const QueueElement & o) const {
        return o.get_vertex() == vertex && o.get_dist() == dist;
    }
    bool operator!=(const QueueElement & o) const {
        return !operator==(o);
    }
    bool operator<(const QueueElement & o) const {
        return dist < o.get_dist();
    }
    bool operator>(const QueueElement & o) const {
        return dist > o.get_dist();
    }
    friend std::ostream &operator<<(std::ostream &os, const QueueElement &queue_element) {
        return os << queue_element.get_dist() << std::endl;
    }
};

static const DistType EMPTY_ELEMENT_DIST = -1;
static const QueueElement EMPTY_ELEMENT = {0, EMPTY_ELEMENT_DIST};

template<typename MQ>
inline int dijkstra(const AdjList &graph, int num_pqs, Vertex start_vertex = 0) {
    MQ q(num_pqs);  // MQ<QueueElement>
    int iterations = 0;
    std::vector<int> dists(graph.size(), std::numeric_limits<DistType>::max());
    dists[start_vertex] = 0;
    q.push(start_vertex);
    while (!q.empty()) {
        QueueElement e = q.pop().first;
        for (const Edge &edge: graph[e.get_vertex()]) {
            Vertex to = edge.get_to();
            DistType new_dist = e.get_dist() + edge.get_weight();
            if (dists[to] > new_dist) {
                dists[to] = new_dist;
                q.push({to, new_dist});
            }
        }
        iterations++;
    }
    return iterations;
}

#endif //ADVERSARIAL_MQ_DIJKSTRA_H
