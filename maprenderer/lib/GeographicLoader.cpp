#include "GeographicLoader.h"

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
    for (Polygon &polygon : m_polygons)
    {
        std::vector<glm::vec2> vertices;
        for (Coordinate const &coordinate : polygon.coordinates())
        {
            if (auto project = projection.project(coordinate.getLongitude(), coordinate.getLatitude()); project.has_value())
            {
                vertices.push_back(project.value());
            }
        }
        polygon.setVertices(vertices);
    }
}