#include "Graph.h"
#include <queue>
#include <limits>
#include <algorithm>

Graph::Graph() {
}

void Graph::addNode(int id, double lat, double lon) {
    GraphNode node;
    node.id = id;
    node.latitude = lat;
    node.longitude = lon;
    nodes[id] = node;
}

void Graph::addEdge(int from, int to, double weight) {
    Edge edge;
    edge.to = to;
    edge.weight = weight;
    adjacencyList[from].append(edge);
}

double Graph::calculateDistance(double lat1, double lon1, double lat2, double lon2) {
    double dx = lat2 - lat1;
    double dy = lon2 - lon1;
    return std::sqrt(dx * dx + dy * dy);
}

QVector<int> Graph::dijkstra(int start, int end) {
    QMap<int, double> distances;
    QMap<int, int> previous;
    QMap<int, bool> visited;
    
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
        distances[it.key()] = std::numeric_limits<double>::infinity();
        visited[it.key()] = false;
    }
    distances[start] = 0;
    
    std::priority_queue<QPair<double, int>, std::vector<QPair<double, int>>, std::greater<QPair<double, int>>> pq;
    pq.push(qMakePair(0.0, start));
    
    while (!pq.empty()) {
        int current = pq.top().second;
        pq.pop();
        
        if (visited[current]) continue;
        visited[current] = true;
        
        if (current == end) break;
        
        if (adjacencyList.contains(current)) {
            for (const Edge& edge : adjacencyList[current]) {
                if (!visited[edge.to]) {
                    double newDist = distances[current] + edge.weight;
                    if (newDist < distances[edge.to]) {
                        distances[edge.to] = newDist;
                        previous[edge.to] = current;
                        pq.push(qMakePair(newDist, edge.to));
                    }
                }
            }
        }
    }
    
    QVector<int> path;
    if (distances[end] == std::numeric_limits<double>::infinity()) {
        return path;
    }
    
    int current = end;
    while (current != start) {
        path.prepend(current);
        if (!previous.contains(current)) {
            return QVector<int>();
        }
        current = previous[current];
    }
    path.prepend(start);
    
    return path;
}

const QMap<int, GraphNode>& Graph::getNodes() const {
    return nodes;
}

const QMap<int, QVector<Edge>>& Graph::getAdjacencyList() const {
    return adjacencyList;
}
