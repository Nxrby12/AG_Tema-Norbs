#include "MainWindow.h"
#include <QDebug>
#include <limits>

MainWindow::MainWindow(Graph* g, KDTree* kd, QWidget* parent)
    : QMainWindow(parent), graph(g), kdtree(kd), 
      scale(1.0), offsetX(0), offsetY(0), zoomFactor(1.0),
      selectedNode1(-1), selectedNode2(-1) {
    
    setWindowTitle("Dijkstra Path Visualizer - Luxembourg Map");
    resize(1200, 800);
    
    calculateBounds();
    updateScale();
}

MainWindow::~MainWindow() {
}

void MainWindow::calculateBounds() {
    minLat = std::numeric_limits<double>::max();
    maxLat = std::numeric_limits<double>::lowest();
    minLon = std::numeric_limits<double>::max();
    maxLon = std::numeric_limits<double>::lowest();
    
    const QMap<int, GraphNode>& nodes = graph->getNodes();
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
        const GraphNode& node = it.value();
        if (node.latitude < minLat) minLat = node.latitude;
        if (node.latitude > maxLat) maxLat = node.latitude;
        if (node.longitude < minLon) minLon = node.longitude;
        if (node.longitude > maxLon) maxLon = node.longitude;
    }
}

void MainWindow::updateScale() {
    double latRange = maxLat - minLat;
    double lonRange = maxLon - minLon;
    
    double scaleX = (width() - 40) / lonRange;
    double scaleY = (height() - 40) / latRange;
    
    scale = std::min(scaleX, scaleY) * zoomFactor;
    
    offsetX = 20;
    offsetY = 20;
}

QPointF MainWindow::mapToScreen(double lat, double lon) {
    double x = (lon - minLon) * scale + offsetX;
    double y = (maxLat - lat) * scale + offsetY;
    return QPointF(x, y);
}

QPair<double, double> MainWindow::screenToMap(int x, int y) {
    double lon = (x - offsetX) / scale + minLon;
    double lat = maxLat - (y - offsetY) / scale;
    return qMakePair(lat, lon);
}

void MainWindow::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    painter.fillRect(rect(), Qt::white);
    
    const QMap<int, QVector<Edge>>& adjacencyList = graph->getAdjacencyList();
    const QMap<int, GraphNode>& nodes = graph->getNodes();
    
    painter.setPen(QPen(QColor(200, 200, 200), 1));
    for (auto it = adjacencyList.begin(); it != adjacencyList.end(); ++it) {
        int fromId = it.key();
        if (!nodes.contains(fromId)) continue;
        
        const GraphNode& fromNode = nodes[fromId];
        QPointF fromPoint = mapToScreen(fromNode.latitude, fromNode.longitude);
        
        for (const Edge& edge : it.value()) {
            if (!nodes.contains(edge.to)) continue;
            
            const GraphNode& toNode = nodes[edge.to];
            QPointF toPoint = mapToScreen(toNode.latitude, toNode.longitude);
            
            painter.drawLine(fromPoint, toPoint);
        }
    }
    
    if (!shortestPath.isEmpty()) {
        painter.setPen(QPen(Qt::red, 3));
        for (int i = 0; i < shortestPath.size() - 1; ++i) {
            int fromId = shortestPath[i];
            int toId = shortestPath[i + 1];
            
            if (nodes.contains(fromId) && nodes.contains(toId)) {
                const GraphNode& fromNode = nodes[fromId];
                const GraphNode& toNode = nodes[toId];
                
                QPointF fromPoint = mapToScreen(fromNode.latitude, fromNode.longitude);
                QPointF toPoint = mapToScreen(toNode.latitude, toNode.longitude);
                
                painter.drawLine(fromPoint, toPoint);
            }
        }
    }
    
    if (selectedNode1 != -1 && nodes.contains(selectedNode1)) {
        const GraphNode& node = nodes[selectedNode1];
        QPointF point = mapToScreen(node.latitude, node.longitude);
        painter.setBrush(Qt::green);
        painter.setPen(QPen(Qt::darkGreen, 2));
        painter.drawEllipse(point, 6, 6);
    }
    
    if (selectedNode2 != -1 && nodes.contains(selectedNode2)) {
        const GraphNode& node = nodes[selectedNode2];
        QPointF point = mapToScreen(node.latitude, node.longitude);
        painter.setBrush(Qt::blue);
        painter.setPen(QPen(Qt::darkBlue, 2));
        painter.drawEllipse(point, 6, 6);
    }
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        QPair<double, double> mapCoords = screenToMap(event->x(), event->y());
        int nearestNode = kdtree->findNearest(mapCoords.first, mapCoords.second);
        
        if (nearestNode != -1) {
            if (selectedNode1 == -1) {
                selectedNode1 = nearestNode;
                selectedNode2 = -1;
                shortestPath.clear();
            } else if (selectedNode2 == -1) {
                selectedNode2 = nearestNode;
                shortestPath = graph->dijkstra(selectedNode1, selectedNode2);
            } else {
                selectedNode1 = nearestNode;
                selectedNode2 = -1;
                shortestPath.clear();
            }
            
            update();
        }
    }
}

void MainWindow::wheelEvent(QWheelEvent* event) {
    double delta = event->angleDelta().y() / 120.0;
    double zoomChange = 1.0 + (delta * 0.1);
    zoomFactor *= zoomChange;
    
    if (zoomFactor < 0.1) zoomFactor = 0.1;
    if (zoomFactor > 10.0) zoomFactor = 10.0;
    
    updateScale();
    update();
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    updateScale();
    QMainWindow::resizeEvent(event);
}
