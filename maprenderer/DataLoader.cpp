#include "DataLoader.h"

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
        if (name == "Spain")
        {
            std::cout << "Iceland found\n";
            QJsonObject geometry = feature.toObject().value("geometry").toObject();
            QString type = geometry.value("type").toString();
            QJsonArray coordinates = geometry.value("coordinates").toArray();

            std::cout << "name: " << name.toStdString() << "\n";
            std::cout << "type: " << type.toStdString() << "\n";
            if (type == "MultiPolygon")
            {
                for (const QJsonValue &coordinate : coordinates)
                {
                    for (const QJsonValue &innerCoordinates : coordinate.toArray())
                    {
                        std::vector<glm::vec3> polygon;
                        for (const QJsonValue &point : innerCoordinates.toArray())
                        {
                            const QJsonArray pointArray = point.toArray();
                            // QJsonArray point = coordinate.toArray();
                            double lon = pointArray.at(0).toDouble();
                            double lat = pointArray.at(1).toDouble();

                            PJ_COORD a, b;
                            a = proj_coord(lon, lat, 0, 0);
                            b = proj_trans(P, PJ_FWD, a);
                            if (b.xyzt.x == HUGE_VAL)
                            {
                                std::cout << "Transformation failed\n";
                                return;
                            }

                            // Convert longitude and latitude to radians
                            float lonRad = glm::radians(lon);
                            float latRad = glm::radians(lat);

                            // Convert to Cartesian coordinates
                            float x = glm::cos(latRad) * glm::cos(lonRad);
                            float y = glm::cos(latRad) * glm::sin(lonRad);

                            // Scale and translate the coordinates so they fit in the range [-1, 1]
                            x = x / M_PI + 0.5f;
                            y = y / (2.0f * M_PI) + 0.5f;

                            std::cout << "x: " << x << " y: " << y << "\n";
                            glm::vec3 cartesian(x, y, 0.0f);


                            // Normalize longitude and latitude to the range [-1, 1]
                            double normalized_lon = lon / 180.0;
                            double normalized_lat = lat / 90.0;

                            // Map normalized coordinates to the range [0, width] for x and [0, height] for y
                            double mapped_lon = (normalized_lon + 1.0) * 0.5 * 1280;
                            double mapped_lat = (normalized_lat + 1.0) * 0.5 * 960;

                            // Create a glm::vec3 from your normalized coordinates
                            glm::vec3 opengl_coords(normalized_lon, normalized_lat, 0.0f);
                            glm::vec3 lonLat(lon, lat, 0.0f);

                            glm::vec3 mapped_coords(mapped_lon, mapped_lat, 0.0f);

                            // Now opengl_coords contains your OpenGL coordinates
                            // std::cout << "OpenGL coordinates: " << opengl_coords.x << " " << opengl_coords.y << "\n";
                            // std::cout << "Mapped coordinates: " << mapped_coords.x << " " << mapped_coords.y << "\n";
                            

                            polygon.push_back(mapped_coords);
                        }
                        m_vertices.push_back(polygon);
                    }
                }
            }
        }
    }
    proj_destroy(P);
}