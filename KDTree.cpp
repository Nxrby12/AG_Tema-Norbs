#include "KDTree.h"
#include <algorithm>
#include <cmath>
#include <limits>

KDTree::KDTree() : root(nullptr) {
}

void KDTree::build(const QVector<QPair<double, double>>& points, const QVector<int>& ids) {
    QVector<QPair<double, double>> pointsCopy = points;
    QVector<int> idsCopy = ids;
    root = buildTree(pointsCopy, idsCopy, 0);
}

std::shared_ptr<KDNode> KDTree::buildTree(QVector<QPair<double, double>>& points, QVector<int>& ids, int depth) {
    if (points.isEmpty()) {
        return nullptr;
    }
    
    int axis = depth % 2;
    
    QVector<QPair<QPair<double, double>, int>> combined;
    for (int i = 0; i < points.size(); ++i) {
        combined.append(qMakePair(points[i], ids[i]));
    }
    
    if (axis == 0) {
        std::sort(combined.begin(), combined.end(), 
                  [](const QPair<QPair<double, double>, int>& a, 
                     const QPair<QPair<double, double>, int>& b) {
            return a.first.first < b.first.first;
        });
    } else {
        std::sort(combined.begin(), combined.end(), 
                  [](const QPair<QPair<double, double>, int>& a, 
                     const QPair<QPair<double, double>, int>& b) {
            return a.first.second < b.first.second;
        });
    }
    
    for (int i = 0; i < combined.size(); ++i) {
        points[i] = combined[i].first;
        ids[i] = combined[i].second;
    }
    
    int median = points.size() / 2;
    auto node = std::make_shared<KDNode>(points[median].first, points[median].second, ids[median]);
    
    QVector<QPair<double, double>> leftPoints = points.mid(0, median);
    QVector<int> leftIds = ids.mid(0, median);
    node->left = buildTree(leftPoints, leftIds, depth + 1);
    
    QVector<QPair<double, double>> rightPoints = points.mid(median + 1);
    QVector<int> rightIds = ids.mid(median + 1);
    node->right = buildTree(rightPoints, rightIds, depth + 1);
    
    return node;
}

int KDTree::findNearest(double x, double y) {
    if (!root) {
        return -1;
    }
    
    std::shared_ptr<KDNode> best = root;
    double bestDist = distance(x, y, root->x, root->y);
    findNearestHelper(root, x, y, 0, best, bestDist);
    
    return best->nodeId;
}

void KDTree::findNearestHelper(std::shared_ptr<KDNode> node, double x, double y, int depth,
                               std::shared_ptr<KDNode>& best, double& bestDist) {
    if (!node) {
        return;
    }
    
    double dist = distance(x, y, node->x, node->y);
    if (dist < bestDist) {
        bestDist = dist;
        best = node;
    }
    
    int axis = depth % 2;
    double diff = (axis == 0) ? (x - node->x) : (y - node->y);
    
    std::shared_ptr<KDNode> nearSide = (diff < 0) ? node->left : node->right;
    std::shared_ptr<KDNode> farSide = (diff < 0) ? node->right : node->left;
    
    findNearestHelper(nearSide, x, y, depth + 1, best, bestDist);
    
    if (diff * diff < bestDist) {
        findNearestHelper(farSide, x, y, depth + 1, best, bestDist);
    }
}

double KDTree::distance(double x1, double y1, double x2, double y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}
