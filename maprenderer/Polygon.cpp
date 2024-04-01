#include "Polygon.h"

Polygon::Polygon(const std::vector<Coordinate> &coordinates) : m_coordinates(coordinates)
{
}
Polygon Polygon::fromJson(const QJsonValue &json)
{
    std::vector<Coordinate> coordinates;
    for (const auto &coordinate : json.toArray())
    {
        coordinates.push_back(Coordinate::fromJson(coordinate));
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
void Polygon::setVertices(const std::vector<glm::vec2> &vertices)
{
    m_vertices = vertices;
}
void Polygon::triangulate()
{
    // todo: implement triangulation
    m_triangulated = true;
}
bool Polygon::isTriangulated() const
{
    return m_triangulated;
}