#ifndef GRAPH_H
#define GRAPH_H

#include <QMap>
#include <QVector>
#include <QPair>
#include <cmath>

struct GraphNode {
    int id;
    double latitude;
    double longitude;
};

struct Edge {
    int to;
    double weight;
};

class Graph {
public:
    Graph();
    void addNode(int id, double lat, double lon);
    void addEdge(int from, int to, double weight);
    QVector<int> dijkstra(int start, int end);
    const QMap<int, GraphNode>& getNodes() const;
    const QMap<int, QVector<Edge>>& getAdjacencyList() const;

private:
    QMap<int, GraphNode> nodes;
    QMap<int, QVector<Edge>> adjacencyList;
    double calculateDistance(double lat1, double lon1, double lat2, double lon2);
};

#endif
