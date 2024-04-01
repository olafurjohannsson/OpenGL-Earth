#include "GeographicLoader.h"
#include "Earcut.h"
#include <array>

GeographicLoader::GeographicLoader(const QString &filename)
{
    QFile jsonFile(filename);
    if (!jsonFile.open(QIODevice::ReadOnly))
    {
        qWarning("Couldn't open save file.");
    }

    assert(jsonFile.exists());

    const auto &data = jsonFile.readAll();
    const auto &doc = QJsonDocument::fromJson(data);
    const auto &features = doc.object().value("features").toArray();
    
    
    m_polygons.reserve(features.size());
    

    for (const QJsonValue &feature : features)
    {
        const auto &properties = feature.toObject().value("properties").toObject();
        const auto &name = properties.value("name").toString();

        const auto &geometry = feature.toObject().value("geometry").toObject();
        const auto &type = geometry.value("type").toString();
        const auto &coordinates = geometry.value("coordinates").toArray();

        if (type == "MultiPolygon")
        {
            for (const QJsonValue &coordinate : coordinates)
            {
                for (const QJsonValue &innerCoordinates : coordinate.toArray())
                {
                    m_polygons.push_back(Polygon::fromJson(innerCoordinates));
                }
            }
        }
        else if (type == "Polygon")
        {
            for (const QJsonValue &coordinate : coordinates)
            {
                m_polygons.push_back(Polygon::fromJson(coordinate));
            }
        }
        else
        {
            std::cout << "Unknown type: " << type.toStdString() << "\n";
        }
    }
}

const std::vector<Polygon> &GeographicLoader::polygons() const
{
    return m_polygons;
}

void GeographicLoader::project(const Projection &projection)
{
    using Coord = double;
    using N = uint32_t;
    using Point = std::array<Coord, 2>;

    for (Polygon &polygon : m_polygons)
    {
        std::vector<glm::vec2> vertices;
        std::vector<std::vector<Point>> triangulatedPolygon;
        std::vector<Point> polygonVertices;
        for (Coordinate const &coordinate : polygon.coordinates())
        {
            const auto radians(glm::radians(glm::vec2(coordinate.getLongitude(), coordinate.getLatitude())));
            vertices.push_back(radians);
            polygonVertices.push_back(Point{radians.x, radians.y});
        }
        triangulatedPolygon.push_back(polygonVertices);
        polygon.setVertices(vertices);
        std::vector<N> indices = mapbox::earcut<N>(triangulatedPolygon);
        polygon.setIndices(indices);
    }
}