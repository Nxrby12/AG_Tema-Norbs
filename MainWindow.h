#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include "Graph.h"
#include "KDTree.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(Graph* graph, KDTree* kdtree, QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    Graph* graph;
    KDTree* kdtree;
    
    double minLat, maxLat, minLon, maxLon;
    double scale;
    double offsetX, offsetY;
    double zoomFactor;
    
    int selectedNode1;
    int selectedNode2;
    QVector<int> shortestPath;
    
    void calculateBounds();
    void updateScale();
    QPointF mapToScreen(double lat, double lon);
    QPair<double, double> screenToMap(int x, int y);
};

#endif
