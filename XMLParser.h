#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QString>
#include <QMap>
#include <QVector>
#include <QPair>

struct Node {
    int id;
    double latitude;
    double longitude;
};

struct Arc {
    int from;
    int to;
    int length;
};

class XMLParser {
public:
    XMLParser();
    bool parseFile(const QString& filename);
    const QMap<int, Node>& getNodes() const;
    const QVector<Arc>& getArcs() const;

private:
    QMap<int, Node> nodes;
    QVector<Arc> arcs;
};

#endif
