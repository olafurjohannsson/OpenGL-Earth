#include "Polygon.h"
#include <iostream>

Polygon::Polygon(const std::vector<Coordinate> &coordinates) : m_coordinates(coordinates)
{

    float minX = -1.13645;
    float maxX = 1.11244;
    float minY = -1.15383;
    float maxY = 1.1163;

    for (const auto &coord : coordinates)
    {
        // m_vertices.push_back(coord.normalizeAndMapTo2D(1280, 960));
        // (double minX, double maxX, double minY, double maxY, double width, double height) const
        m_vertices.push_back(coord.stereographicProjection(minX, maxX, minY, maxY, 1280, 960));
        // m_vertices.push_back(coord.stereographicProjection());
        // m_vertices.push_back(coord.mercatorProjection());
    }
}

Polygon Polygon::fromJson(const QJsonValue &json)
{
    std::vector<Coordinate> coordinates;
    for (const auto &coordinate : json.toArray())
    {
        Coordinate coord = Coordinate::fromJson(coordinate);
        coordinates.push_back(coord);
    }
    return Polygon(coordinates);
}
std::size_t Polygon::size() const
{
    return m_vertices.size();
}
const glm::vec2 *Polygon::data() const
{
    return m_vertices.data();
}
const std::vector<Coordinate> &Polygon::coordinates() const
{
    return m_coordinates;
}