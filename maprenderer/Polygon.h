#ifndef POLYGON_H
#define POLYGON_H

#include "Coordinate.h"
#include <vector>
#include <QJsonValue>
#include <QJsonArray>

class Polygon
{
public:
    Polygon(const std::vector<Coordinate> &coordinates);

    static Polygon fromJson(const QJsonValue &json);

    const std::vector<Coordinate> &coordinates() const;

    std::size_t size() const;

    const glm::vec2 *data() const;

private:
    std::vector<Coordinate> m_coordinates;
    std::vector<glm::vec2> m_vertices;
};

#endif