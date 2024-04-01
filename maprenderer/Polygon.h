#ifndef POLYGON_H
#define POLYGON_H

#include "Coordinate.h"
#include <vector>
#include <QJsonValue>
#include <QJsonArray>
#include <glm/glm.hpp>

class Polygon
{
public:
    Polygon(const std::vector<Coordinate> &coordinates);

    static Polygon fromJson(const QJsonValue &json);

    const std::vector<Coordinate> &coordinates() const;

    void setVertices(const std::vector<glm::vec2> &vertices);
    void clearVertices()
    {
        m_vertices.clear();
    }
    void triangulate();

    bool isTriangulated() const;

    std::size_t size() const;

    const glm::vec2 *data() const;

private:
    std::vector<Coordinate> m_coordinates;
    std::vector<glm::vec2> m_vertices;
    bool m_triangulated = false;
};

#endif