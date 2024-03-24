#include "DataLoader.h"
#include "Polygon.h"

DataLoader::DataLoader(QObject *parent) : QObject(parent)
{
    PJ *P = proj_create_crs_to_crs(PJ_DEFAULT_CTX, "EPSG:4326", "EPSG:32632", NULL);
    if (P == NULL)
    {
        std::cout << "Failed to create transformation: " << proj_errno_string(proj_errno(P)) << "\n";
        return;
    }
    QFile jsonFile("../data/large.json");
    if (!jsonFile.open(QIODevice::ReadOnly))
    {
        qWarning("Couldn't open save file.");
    }
    assert(jsonFile.exists());
    QByteArray data = jsonFile.readAll();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonArray features = doc.object().value("features").toArray();
    std::cout << "total feature: " << features.size() << "\n";
    for (const QJsonValue &feature : features)
    {
        QJsonObject properties = feature.toObject().value("properties").toObject();
        QString name = properties.value("name").toString();

        QJsonObject geometry = feature.toObject().value("geometry").toObject();
        QString type = geometry.value("type").toString();
        QJsonArray coordinates = geometry.value("coordinates").toArray();
        
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
    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::lowest();
    for (const auto &polygon : m_polygons)
    {
        for (const auto &coord : polygon.coordinates())
        {
            glm::vec2 projected = coord.stereographicProjection();
            minX = std::min(minX, projected.x);
            maxX = std::max(maxX, projected.x);
            minY = std::min(minY, projected.y);
            maxY = std::max(maxY, projected.y);
        }
    }
    
}