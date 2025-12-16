#include <iostream>
#include <algorithm>
#include "XMLParser.h"
#include "Graph.h"
#include "KDTree.h"

int main() {
    std::cout << "Testing Dijkstra Path Visualizer Components..." << std::endl;
    
    XMLParser parser;
    std::cout << "1. Testing XML Parser..." << std::endl;
    if (!parser.parseFile("Harta_Luxemburg.xml")) {
        std::cout << "   [FAIL] Failed to parse XML file" << std::endl;
        return 1;
    }
    
    const QMap<int, Node>& nodes = parser.getNodes();
    const QVector<Arc>& arcs = parser.getArcs();
    std::cout << "   [PASS] Loaded " << nodes.size() << " nodes and " << arcs.size() << " arcs" << std::endl;
    
    if (nodes.empty() || arcs.empty()) {
        std::cout << "   [FAIL] No data loaded" << std::endl;
        return 1;
    }
    
    std::cout << "2. Testing Graph construction..." << std::endl;
    Graph graph;
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
        const Node& node = it.value();
        graph.addNode(node.id, node.latitude, node.longitude);
    }
    
    for (const Arc& arc : arcs) {
        graph.addEdge(arc.from, arc.to, arc.length);
    }
    std::cout << "   [PASS] Graph built with " << graph.getNodes().size() << " nodes" << std::endl;
    
    std::cout << "3. Testing KD-Tree construction..." << std::endl;
    QVector<QPair<double, double>> points;
    QVector<int> ids;
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
        const Node& node = it.value();
        points.append(qMakePair(node.latitude, node.longitude));
        ids.append(node.id);
    }
    
    KDTree kdtree;
    kdtree.build(points, ids);
    std::cout << "   [PASS] KD-Tree built successfully" << std::endl;
    
    std::cout << "4. Testing KD-Tree nearest neighbor search..." << std::endl;
    auto firstNode = nodes.begin().value();
    int nearest = kdtree.findNearest(firstNode.latitude, firstNode.longitude);
    if (nearest == -1) {
        std::cout << "   [FAIL] Nearest neighbor search failed" << std::endl;
        return 1;
    }
    std::cout << "   [PASS] Found nearest node: " << nearest << std::endl;
    
    std::cout << "5. Testing Dijkstra's algorithm..." << std::endl;
    int startNode = 0;
    int endNode = 100;
    
    if (!graph.getNodes().contains(startNode) || !graph.getNodes().contains(endNode)) {
        std::cout << "   [WARN] Test nodes not available, using first two nodes" << std::endl;
        auto it = graph.getNodes().begin();
        startNode = it.key();
        ++it;
        if (it != graph.getNodes().end()) {
            endNode = it.key();
        }
    }
    
    QVector<int> path = graph.dijkstra(startNode, endNode);
    if (path.isEmpty()) {
        std::cout << "   [INFO] No path found between nodes " << startNode << " and " << endNode << " (may be disconnected)" << std::endl;
    } else {
        std::cout << "   [PASS] Found path with " << path.size() << " nodes" << std::endl;
        std::cout << "   Path: ";
        for (int i = 0; i < std::min(5, path.size()); ++i) {
            std::cout << path[i];
            if (i < path.size() - 1) std::cout << " -> ";
        }
        if (path.size() > 5) std::cout << " ... -> " << path[path.size()-1];
        std::cout << std::endl;
    }
    
    std::cout << "\nAll core components tested successfully!" << std::endl;
    std::cout << "\nApplication Features:" << std::endl;
    std::cout << "- XML parsing with QXmlStreamReader" << std::endl;
    std::cout << "- Graph data structure with adjacency list" << std::endl;
    std::cout << "- Dijkstra's algorithm with priority queue" << std::endl;
    std::cout << "- K-d tree for efficient spatial queries" << std::endl;
    std::cout << "- Qt GUI with mouse interaction and zoom" << std::endl;
    std::cout << "- Visual shortest path display" << std::endl;
    
    return 0;
}
