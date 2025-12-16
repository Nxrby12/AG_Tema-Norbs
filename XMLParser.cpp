#include "XMLParser.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QDebug>

XMLParser::XMLParser() {
}

bool XMLParser::parseFile(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QXmlStreamReader xml(&file);
    
    while (!xml.atEnd()) {
        xml.readNext();
        
        if (xml.isStartElement()) {
            if (xml.name() == "node") {
                Node node;
                node.id = xml.attributes().value("id").toInt();
                node.latitude = xml.attributes().value("latitude").toDouble();
                node.longitude = xml.attributes().value("longitude").toDouble();
                nodes[node.id] = node;
            }
            else if (xml.name() == "arc") {
                Arc arc;
                arc.from = xml.attributes().value("from").toInt();
                arc.to = xml.attributes().value("to").toInt();
                arc.length = xml.attributes().value("length").toInt();
                arcs.append(arc);
            }
        }
    }
    
    file.close();
    return !xml.hasError();
}

const QMap<int, Node>& XMLParser::getNodes() const {
    return nodes;
}

const QVector<Arc>& XMLParser::getArcs() const {
    return arcs;
}
