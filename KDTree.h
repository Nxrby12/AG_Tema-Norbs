#ifndef KDTREE_H
#define KDTREE_H

#include <QVector>
#include <QPair>
#include <memory>

struct KDNode {
    double x;
    double y;
    int nodeId;
    std::shared_ptr<KDNode> left;
    std::shared_ptr<KDNode> right;
    
    KDNode(double x_, double y_, int id) : x(x_), y(y_), nodeId(id), left(nullptr), right(nullptr) {}
};

class KDTree {
public:
    KDTree();
    void build(const QVector<QPair<double, double>>& points, const QVector<int>& ids);
    int findNearest(double x, double y);

private:
    std::shared_ptr<KDNode> root;
    std::shared_ptr<KDNode> buildTree(QVector<QPair<double, double>>& points, QVector<int>& ids, int depth);
    void findNearestHelper(std::shared_ptr<KDNode> node, double x, double y, int depth, 
                          std::shared_ptr<KDNode>& best, double& bestDist);
    double distance(double x1, double y1, double x2, double y2);
};

#endif
