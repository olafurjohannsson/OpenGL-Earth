#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include <iostream>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QJsonObject>
#include <vector>
#include <proj.h>
#include <glm/glm.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Coordinate.h"
#include "Polygon.h"

class DataLoader : public QObject
{
    Q_OBJECT

public:
    DataLoader(QObject *parent = nullptr);

    std::vector<std::vector<Coordinate>> getVertices() const { return m_vertices; }
    std::vector<Polygon> getPolygons() const { return m_polygons; }

private:
    std::vector<std::vector<Coordinate>> m_vertices;
    std::vector<Polygon> m_polygons;
};

#endif // DATA_LOADER_H