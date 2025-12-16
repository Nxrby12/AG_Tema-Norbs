#include <QApplication>
#include <QDebug>
#include "MainWindow.h"
#include "XMLParser.h"
#include "Graph.h"
#include "KDTree.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    
    XMLParser parser;
    if (!parser.parseFile("Harta_Luxemburg.xml")) {
        qDebug() << "Failed to parse XML file";
        return 1;
    }
    
    const QMap<int, Node>& nodes = parser.getNodes();
    const QVector<Arc>& arcs = parser.getArcs();
    
    qDebug() << "Loaded" << nodes.size() << "nodes and" << arcs.size() << "arcs";
    
    Graph graph;
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
        const Node& node = it.value();
        graph.addNode(node.id, node.latitude, node.longitude);
    }
    
    for (const Arc& arc : arcs) {
        graph.addEdge(arc.from, arc.to, arc.length);
    }
    
    QVector<QPair<double, double>> points;
    QVector<int> ids;
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
        const Node& node = it.value();
        points.append(qMakePair(node.latitude, node.longitude));
        ids.append(node.id);
    }
    
    KDTree kdtree;
    kdtree.build(points, ids);
    
    qDebug() << "Graph and KD-tree built successfully";
    
    MainWindow window(&graph, &kdtree);
    window.show();
    
    return app.exec();
}
