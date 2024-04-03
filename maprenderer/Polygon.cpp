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
const std::vector<glm::vec2> &Polygon::vertices() const
{
    return m_vertices;
}
const std::vector<uint32_t> &Polygon::indices() const
{
    return m_indices;
}
const std::vector<Coordinate> &Polygon::coordinates() const
{
    return m_coordinates;
}
void Polygon::setVertices(const std::vector<glm::vec2> &vertices)
{
    m_vertices = vertices;
}
void Polygon::setIndices(const std::vector<uint32_t> &indices)
{
    m_indices = indices;
    m_triangulated = true;
}
bool Polygon::isTriangulated() const
{
    return m_triangulated;
}
void Polygon::setOffset(unsigned int vertexOffset, unsigned int indexOffset)
{
    m_vertexOffset = vertexOffset;
    m_indexOffset = indexOffset;
}

unsigned int Polygon::vertexOffset() const
{
    return m_vertexOffset;
}
unsigned int Polygon::indexOffset() const
{
    return m_indexOffset;
}